#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "hte.h"
 
#define RECORDS_PER_BUCKET  50
#define MAX_KEY_LENGTH      32   
#define MAX_PATH_LENGTH     512
 


typedef struct {
    char   key[MAX_KEY_LENGTH];
    char   data[MAX_DATA_SIZE];
    size_t data_size;            
    bool   is_occupied;
} record;
 
typedef struct {
    int    local_depth;
    int    record_count;
    record records[RECORDS_PER_BUCKET];
} bucket;
 
typedef struct {
    FILE   *disk_file;
    char    hf_filename[MAX_PATH_LENGTH];
    char    hfc_filename[MAX_PATH_LENGTH];
    int     global_depth;
    size_t  directory_size;
    long   *bucket_offsets;
    int     total_expansions;
} hte_directory;
 
//Funções privadas
 
static void build_aux_name(const char *base, const char *ext,
                            char *out, size_t out_max)
{
    const char *dot  = strrchr(base, '.');
    size_t base_len  = (dot != NULL) ? (size_t)(dot - base) : strlen(base);
 
    if (base_len > out_max - strlen(ext) - 2)
        base_len = out_max - strlen(ext) - 2;
 
    strncpy(out, base, base_len);
    out[base_len] = '.';
    strcpy(out + base_len + 1, ext);
}
 
static uint32_t hash_function_32(const char *key)
{
    uint32_t hash = 5381;
    int c;
    while ((c = (unsigned char)*key++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}
  
static void split_bucket(hte_directory *dir,
                          uint32_t       index,
                          bucket        *old_bucket,
                          long           old_offset)
{
    if (old_bucket->local_depth == dir->global_depth) {
        size_t old_size = dir->directory_size;
 
        dir->global_depth++;
        dir->directory_size = (size_t)1 << dir->global_depth;
        dir->total_expansions++;
 
        long *tmp = realloc(dir->bucket_offsets,
                            dir->directory_size * sizeof(long));
        if (tmp == NULL) {
            fprintf(stderr, "ERRO CRITICO: falha de realloc em split_bucket\n");
            return;
        }
        dir->bucket_offsets = tmp;
 
        for (size_t i = 0; i < old_size; i++)
            dir->bucket_offsets[i + old_size] = dir->bucket_offsets[i];
    }
 
    record temp[RECORDS_PER_BUCKET];
    memcpy(temp, old_bucket->records, sizeof(temp));
 
    old_bucket->record_count = 0;
    for (int i = 0; i < RECORDS_PER_BUCKET; i++)
        old_bucket->records[i].is_occupied = false;
 
    bucket new_bucket;
    new_bucket.record_count = 0;
    for (int i = 0; i < RECORDS_PER_BUCKET; i++)
        new_bucket.records[i].is_occupied = false;
 
    old_bucket->local_depth++;
    new_bucket.local_depth = old_bucket->local_depth;
 
    fseek(dir->disk_file, 0, SEEK_END);
    long new_offset = ftell(dir->disk_file);
 
    uint32_t split_bit = (uint32_t)1 << (old_bucket->local_depth - 1);
 
    for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
        if (!temp[i].is_occupied) continue;
 
        uint32_t h  = hash_function_32(temp[i].key);
        bucket *dst = (h & split_bit) ? &new_bucket : old_bucket;
 
        for (int j = 0; j < RECORDS_PER_BUCKET; j++) {
            if (!dst->records[j].is_occupied) {
                strncpy(dst->records[j].key, temp[i].key, MAX_KEY_LENGTH - 1);
                dst->records[j].key[MAX_KEY_LENGTH - 1] = '\0';
                memcpy(dst->records[j].data, temp[i].data, MAX_DATA_SIZE);
                dst->records[j].data_size   = temp[i].data_size;
                dst->records[j].is_occupied = true;
                dst->record_count++;
                break;
            }
        }
    }
 
    uint32_t shared_mask = split_bit - 1;
    uint32_t shared_bits = index & shared_mask;
 
    for (size_t i = 0; i < dir->directory_size; i++) {
        if ((i & shared_mask) == shared_bits)
            dir->bucket_offsets[i] = (i & split_bit) ? new_offset : old_offset;
    }
 
    fseek(dir->disk_file, old_offset, SEEK_SET);
    fwrite(old_bucket, sizeof(bucket), 1, dir->disk_file);
 
    fseek(dir->disk_file, new_offset, SEEK_SET);
    fwrite(&new_bucket, sizeof(bucket), 1, dir->disk_file);
}
 
//API pública
 
Hash hash_openFile(const char *filename)
{
    if (filename == NULL) return NULL;
 
    hte_directory *dir = malloc(sizeof(hte_directory));
    if (dir == NULL) return NULL;
 
    strncpy(dir->hf_filename, filename, MAX_PATH_LENGTH - 1);
    dir->hf_filename[MAX_PATH_LENGTH - 1] = '\0';
    build_aux_name(filename, "hfc", dir->hfc_filename, MAX_PATH_LENGTH);
 
    dir->total_expansions = 0;
 
    FILE *f = fopen(filename, "rb+");
 
    if (f == NULL) {
        /* ── Arquivo novo ── */
        f = fopen(filename, "wb+");
        if (f == NULL) {
            fprintf(stderr, "ERRO: nao foi possivel criar '%s'\n", filename);
            free(dir);
            return NULL;
        }
 
        dir->disk_file      = f;
        dir->global_depth   = 0;
        dir->directory_size = 1;
 
        dir->bucket_offsets = malloc(sizeof(long));
        if (dir->bucket_offsets == NULL) { free(dir); fclose(f); return NULL; }
        dir->bucket_offsets[0] = 0;
 
        bucket init;
        init.local_depth  = 0;
        init.record_count = 0;
        for (int i = 0; i < RECORDS_PER_BUCKET; i++)
            init.records[i].is_occupied = false;
 
        fseek(f, 0, SEEK_SET);
        fwrite(&init, sizeof(bucket), 1, f);
 
    } else {
        /* ── Arquivo existente: carrega diretório do .hfc ── */
        dir->disk_file = f;
 
        FILE *hfc = fopen(dir->hfc_filename, "rb");
        if (hfc == NULL) {
            fprintf(stderr, "ERRO CRITICO: '%s' nao encontrado\n",
                    dir->hfc_filename);
            free(dir);
            fclose(f);
            return NULL;
        }
 
        fread(&dir->global_depth,     sizeof(int),    1, hfc);
        fread(&dir->directory_size,   sizeof(size_t), 1, hfc);
        fread(&dir->total_expansions, sizeof(int),    1, hfc);
 
        dir->bucket_offsets = malloc(dir->directory_size * sizeof(long));
        if (dir->bucket_offsets == NULL) {
            free(dir); fclose(f); fclose(hfc); return NULL;
        }
        fread(dir->bucket_offsets, sizeof(long), dir->directory_size, hfc);
        fclose(hfc);
    }
 
    return dir;
}
 
bool hash_insertReg(Hash h, char *key, void *data, size_t data_size)
{
    if (h == NULL || key == NULL || data == NULL) return false;
    if (data_size == 0 || data_size > MAX_DATA_SIZE)       return false;
 
    hte_directory *dir = (hte_directory *)h;
 
    uint32_t hash_val = hash_function_32(key);
    uint32_t mask     = (dir->global_depth == 0) ? 0
                        : ((uint32_t)1 << dir->global_depth) - 1;
    uint32_t index    = hash_val & mask;
    long     offset   = dir->bucket_offsets[index];
 
    bucket cur;
    fseek(dir->disk_file, offset, SEEK_SET);
    fread(&cur, sizeof(bucket), 1, dir->disk_file);
 
    /* Atualiza se chave já existe */
    for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
        if (cur.records[i].is_occupied &&
            strncmp(cur.records[i].key, key, MAX_KEY_LENGTH) == 0) {
            memcpy(cur.records[i].data, data, data_size);
            cur.records[i].data_size = data_size;
            fseek(dir->disk_file, offset, SEEK_SET);
            fwrite(&cur, sizeof(bucket), 1, dir->disk_file);
            return true;
        }
    }
 
    /* Insere em slot livre */
    if (cur.record_count < RECORDS_PER_BUCKET) {
        for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
            if (!cur.records[i].is_occupied) {
                strncpy(cur.records[i].key, key, MAX_KEY_LENGTH - 1);
                cur.records[i].key[MAX_KEY_LENGTH - 1] = '\0';
                memcpy(cur.records[i].data, data, data_size);
                cur.records[i].data_size   = data_size;
                cur.records[i].is_occupied = true;
                cur.record_count++;
                fseek(dir->disk_file, offset, SEEK_SET);
                fwrite(&cur, sizeof(bucket), 1, dir->disk_file);
                return true;
            }
        }
    }
 
    /* Bucket cheio: divide e reinsere */
    split_bucket(dir, index, &cur, offset);
    return hash_insertReg(h, key, data, data_size);
}
 
bool hash_removeReg(Hash h, char *key)
{
    if (h == NULL || key == NULL) return false;
 
    hte_directory *dir = (hte_directory *)h;
 
    uint32_t hash_val = hash_function_32(key);
    uint32_t mask     = (dir->global_depth == 0) ? 0
                        : ((uint32_t)1 << dir->global_depth) - 1;
    long     offset   = dir->bucket_offsets[hash_val & mask];
 
    bucket cur;
    fseek(dir->disk_file, offset, SEEK_SET);
    if (fread(&cur, sizeof(bucket), 1, dir->disk_file) != 1) return false;
 
    for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
        if (cur.records[i].is_occupied &&
            strncmp(cur.records[i].key, key, MAX_KEY_LENGTH) == 0) {
            cur.records[i].is_occupied = false;
            cur.record_count--;
            fseek(dir->disk_file, offset, SEEK_SET);
            fwrite(&cur, sizeof(bucket), 1, dir->disk_file);
            return true;
        }
    }
    return false;
}
 
bool hash_exists(Hash h, char *key)
{
    if (h == NULL || key == NULL) return false;
 
    hte_directory *dir = (hte_directory *)h;
 
    uint32_t hash_val = hash_function_32(key);
    uint32_t mask     = (dir->global_depth == 0) ? 0
                        : ((uint32_t)1 << dir->global_depth) - 1;
    long     offset   = dir->bucket_offsets[hash_val & mask];
 
    bucket cur;
    fseek(dir->disk_file, offset, SEEK_SET);
    if (fread(&cur, sizeof(bucket), 1, dir->disk_file) != 1) return false;
 
    for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
        if (cur.records[i].is_occupied &&
            strncmp(cur.records[i].key, key, MAX_KEY_LENGTH) == 0)
            return true;
    }
    return false;
}
 
bool hash_getRegistry(Hash h, char *key, void *out, size_t out_size)
{
    if (h == NULL || key == NULL || out == NULL) return false;
 
    hte_directory *dir = (hte_directory *)h;
 
    uint32_t hash_val = hash_function_32(key);
    uint32_t mask     = (dir->global_depth == 0) ? 0
                        : ((uint32_t)1 << dir->global_depth) - 1;
    long     offset   = dir->bucket_offsets[hash_val & mask];
 
    bucket cur;
    fseek(dir->disk_file, offset, SEEK_SET);
    if (fread(&cur, sizeof(bucket), 1, dir->disk_file) != 1) return false;
 
    for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
        if (cur.records[i].is_occupied &&
            strncmp(cur.records[i].key, key, MAX_KEY_LENGTH) == 0) {
            size_t copy_size = cur.records[i].data_size < out_size
                               ? cur.records[i].data_size : out_size;
            memcpy(out, cur.records[i].data, copy_size);
            return true;
        }
    }
    return false;
}
 
int hash_getSize(Hash h)
{
    if (h == NULL) return -1;
    return (int)((hte_directory *)h)->directory_size;
}
 
void hash_dumpFile(Hash h, const char *filename)
{
    if (h == NULL || filename == NULL) return;
 
    hte_directory *dir = (hte_directory *)h;
 
    FILE *out = fopen(filename, "w");
    if (out == NULL) {
        fprintf(stderr, "ERRO: nao foi possivel criar dump '%s'\n", filename);
        return;
    }
 
    fprintf(out, "=== DUMP DO HASHFILE: %s ===\n", dir->hf_filename);
    fprintf(out, "Profundidade global : %d\n", dir->global_depth);
    fprintf(out, "Tamanho do diretorio: %zu entradas\n", dir->directory_size);
    fprintf(out, "Total de expansoes  : %d\n\n", dir->total_expansions);
 
    fprintf(out, "--- Entradas do Diretorio ---\n");
    for (size_t i = 0; i < dir->directory_size; i++)
        fprintf(out, "  dir[%3zu] -> offset %ld\n", i, dir->bucket_offsets[i]);
    fprintf(out, "\n");
 
    /* Imprime cada bucket único apenas uma vez */
    long visited[1 << 16];
    int  n_visited = 0;
 
    fprintf(out, "--- Conteudo dos Buckets ---\n");
    for (size_t i = 0; i < dir->directory_size; i++) {
        long off = dir->bucket_offsets[i];
 
        bool already = false;
        for (int v = 0; v < n_visited; v++) {
            if (visited[v] == off) { already = true; break; }
        }
        if (already) continue;
        visited[n_visited++] = off;
 
        bucket cur;
        fseek(dir->disk_file, off, SEEK_SET);
        if (fread(&cur, sizeof(bucket), 1, dir->disk_file) != 1) continue;
 
        fprintf(out, "Bucket @ offset %ld | prof_local=%d | registros=%d/%d\n",
                off, cur.local_depth, cur.record_count, RECORDS_PER_BUCKET);
 
        for (int j = 0; j < RECORDS_PER_BUCKET; j++) {
            if (cur.records[j].is_occupied)
                fprintf(out, "    [%2d] chave=\"%s\" | %zu bytes de dados\n",
                        j, cur.records[j].key, cur.records[j].data_size);
        }
        fprintf(out, "\n");
    }
 
    fclose(out);
}
 
void hash_closeFile(Hash h)
{
    if (h == NULL) return;
 
    hte_directory *dir = (hte_directory *)h;
 
    FILE *hfc = fopen(dir->hfc_filename, "wb");
    if (hfc != NULL) {
        fwrite(&dir->global_depth,     sizeof(int),    1, hfc);
        fwrite(&dir->directory_size,   sizeof(size_t), 1, hfc);
        fwrite(&dir->total_expansions, sizeof(int),    1, hfc);
        fwrite(dir->bucket_offsets, sizeof(long), dir->directory_size, hfc);
        fclose(hfc);
    } else {
        fprintf(stderr, "ERRO CRITICO: nao foi possivel salvar '%s'\n",
                dir->hfc_filename);
    }
 
    if (dir->disk_file     != NULL) fclose(dir->disk_file);
    if (dir->bucket_offsets != NULL) free(dir->bucket_offsets);
    free(dir);
}
