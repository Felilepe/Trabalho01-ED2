#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hte.h"
#include "svg.h"
#include "quadra.h"
#include "parse_geo.h"
#include "parse_pm.h"
#include "parse_qry.h"
 
#define PATH_LEN      256
#define FILE_NAME_LEN 256
 
/* ── Extrai o nome-base de um caminho (sem diretório, sem extensão) ── */
static void base_name(const char *path, char *out, size_t out_max)
{
    const char *slash = strrchr(path, '/');
    const char *backslash = strrchr(path, '\\');
    const char *last_sep = (slash > backslash) ? slash : backslash;
    const char *start = last_sep ? last_sep + 1 : path;
 
    const char *dot = strrchr(start, '.');
    size_t len = dot ? (size_t)(dot - start) : strlen(start);
    if (len >= out_max) len = out_max - 1;
 
    strncpy(out, start, len);
    out[len] = '\0';
}
 
/* ── Remove barra final de um path, se houver ── */
static void strip_trailing_slash(char *path)
{
    int len = (int)strlen(path);
    if (len > 1 && path[len - 1] == '/')
        path[len - 1] = '\0';
}
 
/* ── Callback: desenha uma quadra do hashfile no SVG ── */
typedef struct {
    FILE *svg;
    Hash  h_quadras;
} DrawCtx;
 
static void cb_draw_quadra(char *key, void *data, size_t data_size, void *aux)
{
    (void)data; (void)data_size;
    DrawCtx *ctx = (DrawCtx *)aux;
 
    double x, y, w, h, sw;
    char cfill[30], cstrk[30];
 
    if (!geoGetQuadra(ctx->h_quadras, key, &x, &y, &w, &h, &sw, cfill, cstrk))
        return;
 
    Quadra q = quadraCreate(key, x, y, w, h);
    if (q == NULL) return;
 
    quadraSetBordaWidth(q, sw);
    quadraSetCorBorda(q, cstrk);
    quadraSetCorPreench(q, cfill);
 
    svgInsertQuadra(ctx->svg, q);
    svgMarcaCep(ctx->svg, q);
 
    quadraDestroy(q);
}
 


int main(int argc, char *argv[])
{
    /* ── Valores padrão dos parâmetros ── */
    char dir_entrada[PATH_LEN]   = ".";
    char arq_geo[FILE_NAME_LEN]  = "";
    char arq_qry[FILE_NAME_LEN]  = "";
    char arq_pm[FILE_NAME_LEN]   = "";
    char dir_saida[PATH_LEN]     = "";
 
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-e") == 0) {
            i++;
            assert(i < argc);
            strncpy(dir_entrada, argv[i], PATH_LEN - 1);
            dir_entrada[PATH_LEN - 1] = '\0';
            strip_trailing_slash(dir_entrada);
        }
        else if (strcmp(argv[i], "-f") == 0) {
            i++;
            assert(i < argc);
            strncpy(arq_geo, argv[i], FILE_NAME_LEN - 1);
            arq_geo[FILE_NAME_LEN - 1] = '\0';
        }
        else if (strcmp(argv[i], "-q") == 0) {
            i++;
            assert(i < argc);
            strncpy(arq_qry, argv[i], FILE_NAME_LEN - 1);
            arq_qry[FILE_NAME_LEN - 1] = '\0';
        }
        else if (strcmp(argv[i], "-pm") == 0) {
            i++;
            assert(i < argc);
            strncpy(arq_pm, argv[i], FILE_NAME_LEN - 1);
            arq_pm[FILE_NAME_LEN - 1] = '\0';
        }
        else if (strcmp(argv[i], "-o") == 0) {
            i++;
            assert(i < argc);
            strncpy(dir_saida, argv[i], PATH_LEN - 1);
            dir_saida[PATH_LEN - 1] = '\0';
            strip_trailing_slash(dir_saida);
        }
        i++;
    }
 
    if (arq_geo[0] == '\0') {
        fprintf(stderr, "ERRO: parametro -f (arquivo .geo) e obrigatorio\n");
        return 1;
    }
    if (dir_saida[0] == '\0') {
        fprintf(stderr, "ERRO: parametro -o (diretorio de saida) e obrigatorio\n");
        return 1;
    }
 
    char path_geo[PATH_LEN];
    snprintf(path_geo, PATH_LEN, "%s/%s", dir_entrada, arq_geo);
 
    char base_geo[FILE_NAME_LEN];
    base_name(arq_geo, base_geo, FILE_NAME_LEN);
 
    char path_hf_quad[PATH_LEN], path_hf_hab[PATH_LEN], path_hf_mor[PATH_LEN];
    snprintf(path_hf_quad, PATH_LEN, "%s/%s_blocks.hf",    dir_saida, base_geo);
    snprintf(path_hf_hab,  PATH_LEN, "%s/%s_people.hf", dir_saida, base_geo);
    snprintf(path_hf_mor,  PATH_LEN, "%s/%s_addresses.hf",  dir_saida, base_geo);
 
    remove(path_hf_quad); remove(path_hf_hab); remove(path_hf_mor);
 
    char aux[PATH_LEN];
    snprintf(aux, PATH_LEN, "%s/%s_blocks.hfc",    dir_saida, base_geo); remove(aux);
    snprintf(aux, PATH_LEN, "%s/%s_people.hfc", dir_saida, base_geo); remove(aux);
    snprintf(aux, PATH_LEN, "%s/%s_addresses.hfc",  dir_saida, base_geo); remove(aux);
 
    Hash h_quadras = hashOpenFile(path_hf_quad);
    Hash h_hab     = hashOpenFile(path_hf_hab);
    Hash h_mor     = hashOpenFile(path_hf_mor);
 
    if (!h_quadras || !h_hab || !h_mor) {
        fprintf(stderr, "ERRO: falha ao abrir hashfiles\n");
        return 1;
    }
 
    char path_svg_geo[PATH_LEN];
    snprintf(path_svg_geo, PATH_LEN, "%s/%s.svg", dir_saida, base_geo);
 
    FILE *svg_geo = startSVG(path_svg_geo);
    if (svg_geo == NULL) {
        fprintf(stderr, "ERRO: nao foi possivel criar '%s'\n", path_svg_geo);
        return 1;
    }
 
    parseGeo(path_geo, h_quadras, svg_geo);
 
    if (arq_pm[0] != '\0') {
        char path_pm[PATH_LEN];
        snprintf(path_pm, PATH_LEN, "%s/%s", dir_entrada, arq_pm);
        parsePm(path_pm, h_hab, h_mor);
    }
 
    stopSVG(svg_geo);
 
    if (arq_qry[0] != '\0') {
        char path_qry[PATH_LEN];
        snprintf(path_qry, PATH_LEN, "%s/%s", dir_entrada, arq_qry);
 
        char base_qry[FILE_NAME_LEN];
        base_name(arq_qry, base_qry, FILE_NAME_LEN);
 
        char path_svg_qry[PATH_LEN], path_txt_qry[PATH_LEN];
        snprintf(path_svg_qry, PATH_LEN, "%s/%s-%s.svg", dir_saida, base_geo, base_qry);
        snprintf(path_txt_qry, PATH_LEN, "%s/%s-%s.txt", dir_saida, base_geo, base_qry);
 
        FILE *svg_qry = startSVG(path_svg_qry);
        FILE *txt_qry = fopen(path_txt_qry, "w");
 
        if (svg_qry == NULL || txt_qry == NULL) {
            fprintf(stderr, "ERRO: nao foi possivel criar arquivos de saida da query\n");
        } else {
            DrawCtx draw_ctx = { svg_qry, h_quadras };
            hashForEach(h_quadras, cb_draw_quadra, &draw_ctx);
 
            parseQry(path_qry, h_quadras, h_hab, h_mor, svg_qry, txt_qry);
 
            stopSVG(svg_qry);
            fclose(txt_qry);
        }
    }
 
    char path_hfd[PATH_LEN];
    snprintf(path_hfd, PATH_LEN, "%s/%s_blocks.hfd",    dir_saida, base_geo);
    hashDumpFile(h_quadras, path_hfd);
    snprintf(path_hfd, PATH_LEN, "%s/%s_people.hfd", dir_saida, base_geo);
    hashDumpFile(h_hab, path_hfd);
    snprintf(path_hfd, PATH_LEN, "%s/%s_addresses.hfd",  dir_saida, base_geo);
    hashDumpFile(h_mor, path_hfd);
 
    hashCloseFile(h_quadras);
    hashCloseFile(h_hab);
    hashCloseFile(h_mor);
 
    return 0;
}
