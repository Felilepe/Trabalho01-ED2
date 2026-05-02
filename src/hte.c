#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "hte.h"

#define RECORDS_PER_BUCKET  50
#define MAX_KEY_LENGTH      32   // 31 caracteres úteis + '\0'
#define MAX_PATH_LENGTH     512



typedef struct {
    char key[MAX_KEY_LENGTH];
    int  value;
    bool is_occupied;
} record;

typedef struct {
    int    local_depth;
    int    record_count;
    record records[RECORDS_PER_BUCKET];
} bucket;

typedef struct {
    FILE   *disk_file;
    char    hf_filename[MAX_PATH_LENGTH];   //"quadras.hf"
    char    hfc_filename[MAX_PATH_LENGTH];  //"quadras.hfc"
    int     global_depth;
    size_t  directory_size;
    long   *bucket_offsets;
    int     total_expansions;   
} hte_directory;




static void build_aux_name(const char *base, const char *ext,
                            char *out, size_t out_max)
{
    const char *dot = strrchr(base, '.');
    size_t base_len = (dot != NULL) ? (size_t)(dot - base) : strlen(base);
 
    if (base_len > out_max - strlen(ext) - 2)
        base_len = out_max - strlen(ext) - 2;
 
    strncpy(out, base, base_len);
    out[base_len] = '.';
    strcpy(out + base_len + 1, ext);
}
 
/** Função de hash djb2 (32 bits). */
static uint32_t hash_function_32(const char *key)
{
    uint32_t hash = 5381;
    int c;
    while ((c = (unsigned char)*key++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}



Hash hash_openFile(const char *filename)
{
    if (filename == NULL) return NULL;
 
    hte_directory *dir = malloc(sizeof(hte_directory));
    if (dir == NULL) return NULL;
 
    /* Guarda nomes dos arquivos */
    strncpy(dir->hf_filename, filename, MAX_PATH_LENGTH - 1);
    dir->hf_filename[MAX_PATH_LENGTH - 1] = '\0';
    build_aux_name(filename, "hfc", dir->hfc_filename, MAX_PATH_LENGTH);
 
    dir->total_expansions = 0;
 
    /* Tenta abrir arquivo existente */
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
            fprintf(stderr, "ERRO CRITICO: arquivo de diretorio '%s' nao encontrado\n",
                    dir->hfc_filename);
            free(dir);
            fclose(f);
            return NULL;
        }
 
        fread(&dir->global_depth,    sizeof(int),    1, hfc);
        fread(&dir->directory_size,  sizeof(size_t), 1, hfc);
        fread(&dir->total_expansions,sizeof(int),    1, hfc);
 
        dir->bucket_offsets = malloc(dir->directory_size * sizeof(long));
        if (dir->bucket_offsets == NULL) {
            free(dir); fclose(f); fclose(hfc); return NULL;
        }
        fread(dir->bucket_offsets, sizeof(long), dir->directory_size, hfc);
        fclose(hfc);
    }
 
    return dir;
}

bool hashExists(Hash h, char *key)
{
    if (h == NULL || key == NULL) return false;
 
    hte_directory *dir = (hte_directory*)h;
 
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

