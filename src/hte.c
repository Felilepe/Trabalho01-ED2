#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "hte.h"

#define RECORDS_PER_BUCKET 50

typedef struct
{
    char key[MAX_KEY_LENGTH];
    int value;
    bool is_occupied;
}record;

typedef struct
{
    int local_depth;
    int record_count;
    record records[RECORDS_PER_BUCKET];
}bucket;

typedef struct
{
    FILE *disk_file;
    int global_depth;
    size_t directory_size;
    long *bucket_offsets;
}hte_directory;

static uint32_t hash_function_32(const char* key) {
    uint32_t hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

static void split_bucket(hte_directory* dir, uint32_t index, bucket* old_bucket, long old_offset) 
{

    if (old_bucket->local_depth == dir->global_depth) {
        size_t old_size = dir->directory_size;
        dir->global_depth++;
        dir->directory_size = 1 << dir->global_depth;
        
        dir->bucket_offsets = realloc(dir->bucket_offsets, dir->directory_size * sizeof(long));
        
        for (size_t i = 0; i < old_size; i++) {
            dir->bucket_offsets[i + old_size] = dir->bucket_offsets[i];
        }
    }


    record temp_records[RECORDS_PER_BUCKET];
    memcpy(temp_records, old_bucket->records, sizeof(temp_records));

    old_bucket->record_count = 0;
    for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
        old_bucket->records[i].is_occupied = false;
    }

    bucket new_bucket;
    new_bucket.record_count = 0;
    for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
        new_bucket.records[i].is_occupied = false;
    }

    old_bucket->local_depth++;
    new_bucket.local_depth = old_bucket->local_depth;


    fseek(dir->disk_file, 0, SEEK_END);
    long new_offset = ftell(dir->disk_file);


    uint32_t split_bit = 1 << (old_bucket->local_depth - 1);

    for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
        if (temp_records[i].is_occupied) {
            
            uint32_t hash_val = hash_function_32(temp_records[i].key);
            

            bucket* target_bucket = (hash_val & split_bit) ? &new_bucket : old_bucket;

            for (int j = 0; j < RECORDS_PER_BUCKET; j++) {
                if (!target_bucket->records[j].is_occupied) {
                    strncpy(target_bucket->records[j].key, temp_records[i].key, MAX_KEY_LENGTH);
                    target_bucket->records[j].value = temp_records[i].value;
                    target_bucket->records[j].is_occupied = true;
                    target_bucket->record_count++;
                    break; 
                }
            }
        }
    }


    uint32_t shared_bits_mask = split_bit - 1; 
    
    uint32_t shared_bits = index & shared_bits_mask; 

    for (size_t i = 0; i < dir->directory_size; i++) {
        
        if ((i & shared_bits_mask) == shared_bits) {
            
            if ((i & split_bit) != 0) {
                dir->bucket_offsets[i] = new_offset; 
            } else {
                dir->bucket_offsets[i] = old_offset; 
            }
        }
    }

    fseek(dir->disk_file, old_offset, SEEK_SET);
    fwrite(old_bucket, sizeof(bucket), 1, dir->disk_file);

    fseek(dir->disk_file, new_offset, SEEK_SET);
    fwrite(&new_bucket, sizeof(bucket), 1, dir->disk_file);
}

Hash hash_openFile(const char* filename)
{
    hte_directory* dir = malloc(sizeof(hte_directory));
    if(dir == NULL) return NULL;

    FILE* f = fopen(filename, "rb+");

    if(f == NULL){
        f = fopen(filename,"wb+");
        if(f == NULL){
            free(dir);
            printf("ERRO ao criar ficheiro no disco\n");
            return NULL;
        }
        printf("Nenhum fichario com o nome '%s' encontrado. Criando novo fichario...\n", filename);

        dir->disk_file = f;
        dir->global_depth = 0;
        dir->directory_size = 1;
        
        dir->bucket_offsets = malloc(dir->directory_size * sizeof(long));
        dir->bucket_offsets[0] = 0;

        bucket initial_bucket;
        initial_bucket.local_depth = 0;
        initial_bucket.record_count = 0;

        for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
            initial_bucket.records[i].is_occupied = false;
        }

        fseek(f, 0, SEEK_SET);
        fwrite(&initial_bucket, sizeof(bucket), 1, f);

        printf("Novo fichario Hash '%s' criado\n", filename);
    }
    else {
        dir->disk_file = f;
        printf("Ficheiro Hash '%s' aberto com sucesso. Carregando diretorio...\n", filename);
        
        FILE* dir_file = fopen("diretorio.bin", "rb");
        if (dir_file != NULL) {
            fread(&(dir->global_depth), sizeof(int), 1, dir_file);
            fread(&(dir->directory_size), sizeof(size_t), 1, dir_file);
            
            dir->bucket_offsets = malloc(dir->directory_size * sizeof(long));
            
            fread(dir->bucket_offsets, sizeof(long), dir->directory_size, dir_file);
            
            fclose(dir_file);
        } else {
            printf("ERRO CRITICO: Ficheiro 'diretorio.bin' nao encontrado!\n");
            free(dir);
            fclose(f);
            return NULL;
        }
    }

    return dir;
}

bool hash_insertReg(Hash h, char* key, int value)
{
    if (h == NULL || key == NULL) return false;
    hte_directory* dir = (hte_directory*)h;

    uint32_t hash_val = hash_function_32(key);
    uint32_t mask = (1 << dir->global_depth) - 1;
    uint32_t index = hash_val & mask;
    long offset = dir->bucket_offsets[index];

    bucket current_bucket;
    fseek(dir -> disk_file, offset, SEEK_SET);
    fread(&current_bucket, sizeof(bucket),1, dir -> disk_file);

    for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
        if (current_bucket.records[i].is_occupied && 
            strncmp(current_bucket.records[i].key, key, MAX_KEY_LENGTH) == 0) {
            current_bucket.records[i].value = value;
            
            fseek(dir->disk_file, offset, SEEK_SET);
            fwrite(&current_bucket, sizeof(bucket), 1, dir->disk_file);
            return true;
        }
    }

    if (current_bucket.record_count < RECORDS_PER_BUCKET) {
        for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
            if (!current_bucket.records[i].is_occupied) {
                strncpy(current_bucket.records[i].key, key, MAX_KEY_LENGTH - 1);
                current_bucket.records[i].key[MAX_KEY_LENGTH - 1] = '\0'; // Garante terminação
                current_bucket.records[i].value = value;
                current_bucket.records[i].is_occupied = true;
                current_bucket.record_count++;

                fseek(dir->disk_file, offset, SEEK_SET);
                fwrite(&current_bucket, sizeof(bucket), 1, dir->disk_file);
                return true;
            }
        }
    }

    split_bucket(dir, index, &current_bucket, offset);

    return hash_insertReg(h, key, value);
}

bool hash_removeReg(Hash h, char* key)
{
    if (h == NULL || key == NULL) return false;

    hte_directory* dir = (hte_directory*)h;

    uint32_t hash_val = hash_function_32(key);
    uint32_t mask = (1 << dir->global_depth) - 1;
    uint32_t index = hash_val & mask;

    long offset = dir->bucket_offsets[index];

    bucket current_bucket;

    fseek(dir->disk_file, offset, SEEK_SET);
    if (fread(&current_bucket, sizeof(bucket), 1, dir->disk_file) != 1) {
        return false; 
    }

    for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
        
        if (current_bucket.records[i].is_occupied) {
            
            if (strncmp(current_bucket.records[i].key, key, MAX_KEY_LENGTH) == 0) {
                current_bucket.records[i].is_occupied = false;
                
                current_bucket.record_count--;

                fseek(dir->disk_file, offset, SEEK_SET);
                fwrite(&current_bucket, sizeof(bucket), 1, dir->disk_file);

                return true; 
            }
        }
    }

    return false;
}

int hash_getSize(Hash h){return (((hte_directory*)h) -> directory_size);}

bool hash_exists(Hash h, char* key)
{
    if (h == NULL || key == NULL) return false;

    hte_directory* dir = (hte_directory*)h;

    uint32_t hash_val = hash_function_32(key);

    uint32_t mask = (1 << dir->global_depth) - 1;
    uint32_t index = hash_val & mask;

    long offset = dir->bucket_offsets[index];

    bucket current_bucket;

    fseek(dir->disk_file, offset, SEEK_SET);
    

    if (fread(&current_bucket, sizeof(bucket), 1, dir->disk_file) != 1) {
        return false;
    }

    for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
        
        if (current_bucket.records[i].is_occupied) {
            
            if (strncmp(current_bucket.records[i].key, key, MAX_KEY_LENGTH) == 0) {
                return true; 
            }
        }
    }

    return false;
}
int hash_getRegistry(Hash h, char* key)
{
    if (h == NULL || key == NULL) return -1;

    hte_directory* dir = (hte_directory*)h;

    uint32_t hash_val = hash_function_32(key);

    uint32_t mask = (1 << dir->global_depth) - 1;
    uint32_t index = hash_val & mask;

    long offset = dir->bucket_offsets[index];

    bucket current_bucket;

    fseek(dir->disk_file, offset, SEEK_SET);
    fread(&current_bucket, sizeof(bucket), 1, dir->disk_file);

    for (int i = 0; i < RECORDS_PER_BUCKET; i++) {
        
        if (current_bucket.records[i].is_occupied) {
            
            if (strncmp(current_bucket.records[i].key, key, MAX_KEY_LENGTH) == 0) {
                return current_bucket.records[i].value;
            }
        }
    }

    return -1;
}

void hash_closeFile(Hash h)
{
    if (h == NULL) return;

    hte_directory* dir = (hte_directory*)h;


    FILE* dir_file = fopen("diretorio.bin", "wb");
    
    if (dir_file != NULL) {
        fwrite(&(dir->global_depth), sizeof(int), 1, dir_file);
        
        fwrite(&(dir->directory_size), sizeof(size_t), 1, dir_file);
        

        fwrite(dir->bucket_offsets, sizeof(long), dir->directory_size, dir_file);
        
        fclose(dir_file);
        printf("Estado do diretorio salvo com sucesso em 'diretorio.bin'.\n");
    } else {
        printf("ERRO CRÍTICO: Nao foi possivel guardar o estado do diretorio!\n");
    }

    if (dir->disk_file != NULL) {
        fclose(dir->disk_file);
    }

    if (dir->bucket_offsets != NULL) {
        free(dir->bucket_offsets);
    }
    
    free(dir);

    printf("Ficheiro Hash fechado e memoria libertada em seguranca.\n");
}