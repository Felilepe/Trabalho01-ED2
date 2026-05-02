#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
