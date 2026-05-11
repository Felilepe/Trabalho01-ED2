#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "parse_qry.h"
#include "parse_geo.h"
#include "parse_pm.h"
#include "hte.h"
#include "quadra.h"
#include "svg.h"

#define MAX_LINHA  256
#define MAX_CPF     20
#define MAX_CEP     20
#define MAX_NOME    50
#define MAX_COMP    50

/*
 * ATENCAO (Bug 2): QuadraReg, HabitanteReg e MoradorReg sao definidas
 * tambem em parse_geo.c e parse_pm.c. Idealmente deveriam estar em um
 * unico header compartilhado (ex: registros.h) para evitar dessincronizacao.
 */

typedef struct {
    char   cep[MAX_CEP];
    double x, y, w, h, sw;
    char   cfill[30];
    char   cstrk[30];
} QuadraReg;

typedef struct {
    char cpf[MAX_CPF];
    char nome[MAX_NOME];
    char sobrenome[MAX_NOME];
    char sexo;
    char nascimento[12];
} HabitanteReg;

typedef struct {
    char cpf[MAX_CPF];
    char cep[MAX_CEP];
    char face;
    int  num;
    char complemento[MAX_COMP];
} MoradorReg;

/* ─── Helpers de posicionamento ──────────────────────────────────── */

/*
 * Calcula a posicao SVG aproximada de uma casa dado o endereco.
 * O ponto de ancoragem e o canto sudeste da quadra, ou seja,
 * (x + w, y + h) no sistema SVG (y cresce para baixo).
 * O numero e a distancia a partir da ancora ao longo da face.
 */
static void calcPosEndereco(double qx, double qy, double qw, double qh,
                             char face, int num,
                             double *px, double *py)
{
    double ax = qx + qw;   /* ancora X — canto sudeste */
    double ay = qy + qh;   /* ancora Y — canto sudeste */

    switch (face) {
        case 'S': *px = ax - num; *py = ay;       break; /* face sul   */
        case 'N': *px = ax - num; *py = qy;       break; /* face norte */
        case 'L': *px = qx;       *py = ay - num; break; /* face leste */
        case 'O': *px = ax;       *py = ay - num; break; /* face oeste */
        default:  *px = ax;       *py = ay;        break;
    }
}

/* Cria um Quadra temporario a partir de uma QuadraReg para uso no SVG */
static Quadra quadraFromReg(QuadraReg *reg)
{
    return quadraCreate(reg->cep, reg->x, reg->y, reg->w, reg->h);
}

/* ─── Structs auxiliares para callbacks do hashForEach ─────────── */

typedef struct {
    char cep[MAX_CEP];    /* filtro por CEP               */
    int  n, s, l, o;     /* contagens por face            */
    int  total;
} ContFaceAux;


typedef struct {
    char  cep[MAX_CEP];
    char **cpfs;      /* array dinamico de strings */
    int    count;
    int    capacity;
} MoradoresCepAux;

typedef struct {
    int total_hab;
    int total_mor;
    int homens;
    int mulheres;
    int mor_homens;
    int mor_mulheres;
    Hash h_mor;           /* para checar se e morador     */
} CensoAux;

/* ─── Callbacks para hashForEach ───────────────────────────────── */

static void cb_contar_face(char *key, void *data, size_t data_size, void *aux)
{
    (void)key; (void)data_size;
    MoradorReg   *m   = (MoradorReg *)data;
    ContFaceAux  *ctx = (ContFaceAux *)aux;

    if (strncmp(m->cep, ctx->cep, MAX_CEP) != 0) return;

    ctx->total++;
    switch (m->face) {
        case 'N': ctx->n++; break;
        case 'S': ctx->s++; break;
        case 'L': ctx->l++; break;
        case 'O': ctx->o++; break;
    }
}

static void cb_coletar_moradores_cep(char *key, void *data, size_t data_size, void *aux)
{
    (void)data_size;
    MoradorReg      *m   = (MoradorReg *)data;
    MoradoresCepAux *ctx = (MoradoresCepAux *)aux;

    if (strncmp(m->cep, ctx->cep, MAX_CEP) != 0) return;

    /* Cresce o array se necessario */
    if (ctx->count == ctx->capacity) {
        int nova_cap = ctx->capacity * 2;
        char **tmp = realloc(ctx->cpfs, nova_cap * sizeof(char *));
        if (tmp == NULL) {
            fprintf(stderr, "ERRO: falha de realloc em cb_coletar_moradores_cep\n");
            return;
        }
        ctx->cpfs     = tmp;
        ctx->capacity = nova_cap;
    }

    ctx->cpfs[ctx->count] = malloc(MAX_CPF);
    if (ctx->cpfs[ctx->count] == NULL) {
        fprintf(stderr, "ERRO: falha de malloc em cb_coletar_moradores_cep\n");
        return;
    }

    strncpy(ctx->cpfs[ctx->count], key, MAX_CPF - 1);
    ctx->cpfs[ctx->count][MAX_CPF - 1] = '\0';
    ctx->count++;
}

static void cb_censo_habitantes(char *key, void *data, size_t data_size, void *aux)
{
    (void)key; (void)data_size;
    HabitanteReg *hab = (HabitanteReg *)data;
    CensoAux     *ctx = (CensoAux *)aux;

    ctx->total_hab++;

    if (hab->sexo == 'M') ctx->homens++;
    else                  ctx->mulheres++;

    if (hashExists(ctx->h_mor, hab->cpf)) {
        ctx->total_mor++;
        if (hab->sexo == 'M') ctx->mor_homens++;
        else                  ctx->mor_mulheres++;
    }
}

/* ─── Implementacao de cada comando ─────────────────────────────── */

static void processar_rq(const char *linha, Hash h_quadras, Hash h_hab, Hash h_mor, FILE *svg, FILE *txt)
{
    char cep[MAX_CEP];
    if (sscanf(linha, "%*s %19s", cep) != 1) {
        fprintf(stderr, "AVISO: linha 'rq' malformada: %s", linha);
        return;
    }

    fprintf(txt, "[*] rq %s\n", cep);

    /* Recupera a quadra para o SVG */
    QuadraReg qreg;
    if (!hashGetRegistry(h_quadras, cep, &qreg, sizeof(QuadraReg))) {
        fprintf(txt, "ERRO: quadra '%s' nao encontrada\n", cep);
        return;
    }

    
    MoradoresCepAux ctx;
    strncpy(ctx.cep, cep, MAX_CEP);
    ctx.count    = 0;
    ctx.capacity = 16;
    ctx.cpfs     = malloc(ctx.capacity * sizeof(char *));
    if (ctx.cpfs == NULL) {
        fprintf(stderr, "ERRO: falha de malloc em processar_rq\n");
        return;
    }

    hashForEach(h_mor, cb_coletar_moradores_cep, &ctx);

    /* TXT: CPF e nome de cada morador que perde o endereco */
    for (int i = 0; i < ctx.count; i++) {
        HabitanteReg hreg;
        if (hashGetRegistry(h_hab, ctx.cpfs[i], &hreg, sizeof(HabitanteReg)))
            fprintf(txt, "%s %s %s\n", ctx.cpfs[i], hreg.nome, hreg.sobrenome);

        /* Remove do hashfile de moradores — viram sem-teto */
        hashRemoveReg(h_mor, ctx.cpfs[i]);
    }

    /* Libera o array dinamico */
    for (int i = 0; i < ctx.count; i++)
        free(ctx.cpfs[i]);
    free(ctx.cpfs);

    /* SVG: pequeno X vermelho na ancora da quadra removida */
    double ax = qreg.x + qreg.w;
    double ay = qreg.y + qreg.h;
    svgMarcaRedCross(svg, ax, ay);

    /* Remove a quadra */
    hashRemoveReg(h_quadras, cep);
}

static void processar_pq(const char *linha, Hash h_quadras, Hash h_mor, FILE *svg, FILE *txt)
{
    char cep[MAX_CEP];
    if (sscanf(linha, "%*s %19s", cep) != 1) {
        fprintf(stderr, "AVISO: linha 'pq' malformada: %s", linha);
        return;
    }

    fprintf(txt, "\n--- COMANDO PQ --- argumentos: %s ---\n\n", cep);

    QuadraReg qreg;
    if (!hashGetRegistry(h_quadras, cep, &qreg, sizeof(QuadraReg))) {
        fprintf(txt, "ERRO: quadra '%s' nao encontrada\n", cep);
        return;
    }

    /* Conta moradores por face */
    ContFaceAux ctx;
    strncpy(ctx.cep, cep, MAX_CEP);
    ctx.n = ctx.s = ctx.l = ctx.o = ctx.total = 0;
    hashForEach(h_mor, cb_contar_face, &ctx);

    /* TXT */
    fprintf(txt, "Face N: %d\n", ctx.n);
    fprintf(txt, "Face S: %d\n", ctx.s);
    fprintf(txt, "Face L: %d\n", ctx.l);
    fprintf(txt, "Face O: %d\n", ctx.o);
    fprintf(txt, "Total : %d\n", ctx.total);

    /* SVG */
    Quadra q = quadraFromReg(&qreg);
    if (q != NULL) {
        svgMarcaContagemFace(svg, q, 'N', ctx.n);
        svgMarcaContagemFace(svg, q, 'S', ctx.s);
        svgMarcaContagemFace(svg, q, 'L', ctx.l);
        svgMarcaContagemFace(svg, q, 'O', ctx.o);
        svgMarcaContagemTotal(svg, q, ctx.total);
        quadraDestroy(q);
    }
}

static void processar_censo(Hash h_hab, Hash h_mor, FILE *txt)
{
    fprintf(txt, "[*] censo\n");

    CensoAux ctx;
    ctx.total_hab    = 0;
    ctx.total_mor    = 0;
    ctx.homens       = 0;
    ctx.mulheres     = 0;
    ctx.mor_homens   = 0;
    ctx.mor_mulheres = 0;
    ctx.h_mor        = h_mor;

    hashForEach(h_hab, cb_censo_habitantes, &ctx);

    int sem_teto          = ctx.total_hab - ctx.total_mor;
    int sem_teto_homens   = ctx.homens    - ctx.mor_homens;
    int sem_teto_mulheres = ctx.mulheres  - ctx.mor_mulheres;

    double prop   = ctx.total_hab > 0
                    ? (double)ctx.total_mor / ctx.total_hab * 100.0 : 0.0;
    double pct_h  = ctx.total_hab > 0
                    ? (double)ctx.homens    / ctx.total_hab * 100.0 : 0.0;
    double pct_m  = ctx.total_hab > 0
                    ? (double)ctx.mulheres  / ctx.total_hab * 100.0 : 0.0;
    double pct_mh = ctx.total_mor > 0
                    ? (double)ctx.mor_homens   / ctx.total_mor * 100.0 : 0.0;
    double pct_mm = ctx.total_mor > 0
                    ? (double)ctx.mor_mulheres / ctx.total_mor * 100.0 : 0.0;
    double pct_sh = sem_teto > 0
                    ? (double)sem_teto_homens   / sem_teto * 100.0 : 0.0;
    double pct_sm = sem_teto > 0
                    ? (double)sem_teto_mulheres / sem_teto * 100.0 : 0.0;

    fprintf(txt, "- Total de habitantes: %d\n", ctx.total_hab);
    fprintf(txt, "- Total de moradores: %d\n", ctx.total_mor);
    fprintf(txt, "- Proporção moradores/habit.: %.2f%%\n", prop);
    fprintf(txt, "- Número de homens: %d\n", ctx.homens);
    fprintf(txt, "- Número de mulheres: %d\n", ctx.mulheres);
    fprintf(txt, "- %% habitantes homens: %.2f%%\n", pct_h);
    fprintf(txt, "- %% habitantes mulheres: %.2f%%\n", pct_m);
    fprintf(txt, "- %% moradores homens: %.2f%%\n", pct_mh);
    fprintf(txt, "- %% moradores mulheres: %.2f%%\n", pct_mm);
    fprintf(txt, "- Total de sem-tetos: %d\n", sem_teto);
    fprintf(txt, "- %% sem-tetos homens: %.2f%%\n", pct_sh);
    fprintf(txt, "- %% sem-tetos mulheres: %.2f%%\n", pct_sm);
}

static void processar_h(const char *linha,
                         Hash h_hab, Hash h_mor, FILE *txt)
{
    char cpf[MAX_CPF];
    if (sscanf(linha, "%*s %19s", cpf) != 1) {
        fprintf(stderr, "AVISO: linha 'h?' malformada: %s", linha);
        return;
    }

    fprintf(txt, "[*] h? %s\n", cpf);

    HabitanteReg hreg;
    if (!hashGetRegistry(h_hab, cpf, &hreg, sizeof(HabitanteReg))) {
        fprintf(txt, "ERRO: habitante '%s' nao encontrado\n", cpf);
        return;
    }

    fprintf(txt, "- CPF: %s\n",    hreg.cpf);
    fprintf(txt, "- Nome: %s %s\n", hreg.nome, hreg.sobrenome);
    fprintf(txt, "- Sexo: %c\n",    hreg.sexo);
    fprintf(txt, "- Nascimento: %s\n", hreg.nascimento);

    MoradorReg mreg;
    if (hashGetRegistry(h_mor, cpf, &mreg, sizeof(MoradorReg))) {
        fprintf(txt, "- Endereco: %s / Face.%c / %d / %s\n",
                mreg.cep, mreg.face, mreg.num, mreg.complemento);
    } else {
        fprintf(txt, "- Sem endereço (sem-teto)\n");
    }
}

static void processar_nasc(const char *linha, Hash h_hab, FILE *txt)
{
    HabitanteReg reg;
    char sexo_str[4];

    int lidos = sscanf(linha,
                       "%*s %19s %49s %49s %3s %11s",
                       reg.cpf, reg.nome, reg.sobrenome,
                       sexo_str, reg.nascimento);

    if (lidos != 5) {
        fprintf(stderr, "AVISO: linha 'nasc' malformada: %s", linha);
        return;
    }

    reg.sexo = sexo_str[0];
    hashInsertReg(h_hab, reg.cpf, &reg, sizeof(HabitanteReg));
    
    fprintf(txt, "[*] nasc %s %s %s %s %s\n",
        reg.cpf, reg.nome, reg.sobrenome, sexo_str, reg.nascimento);
}

static void processar_rip(const char *linha,
                           Hash h_quadras, Hash h_hab, Hash h_mor,
                           FILE *svg, FILE *txt)
{
    char cpf[MAX_CPF];
    if (sscanf(linha, "%*s %19s", cpf) != 1) {
        fprintf(stderr, "AVISO: linha 'rip' malformada: %s", linha);
        return;
    }

    fprintf(txt, "\n--- COMANDO RIP --- argumentos: %s ---\n\n", cpf);

    HabitanteReg hreg;
    if (!hashGetRegistry(h_hab, cpf, &hreg, sizeof(HabitanteReg))) {
        fprintf(txt, "ERRO: habitante '%s' nao encontrado\n", cpf);
        return;
    }

    fprintf(txt, "- CPF: %s\n",    hreg.cpf);
    fprintf(txt, "- Nome: %s %s\n", hreg.nome, hreg.sobrenome);
    fprintf(txt, "- Sexo: %c\n",    hreg.sexo);
    fprintf(txt, "- Nascimento: %s\n",    hreg.nascimento);

    /* Se era morador, reporta endereco e marca no SVG */
    MoradorReg mreg;
    if (hashGetRegistry(h_mor, cpf, &mreg, sizeof(MoradorReg))) {
        fprintf(txt, "- Endereco: %s / Face.%c / %d / %s\n",
                mreg.cep, mreg.face, mreg.num, mreg.complemento);

        QuadraReg qreg;
        if (hashGetRegistry(h_quadras, mreg.cep, &qreg, sizeof(QuadraReg))) {
            double px, py;
            calcPosEndereco(qreg.x, qreg.y, qreg.w, qreg.h,
                            mreg.face, mreg.num, &px, &py);
            svgMarcaRedCross(svg, px, py);
        }

        hashRemoveReg(h_mor, cpf);
    }

    hashRemoveReg(h_hab, cpf);
}

static void processar_mud(const char *linha,
                           Hash h_quadras, Hash h_hab, Hash h_mor,
                           FILE *svg, FILE *txt)
{
    char cpf[MAX_CPF], cep[MAX_CEP], face_str[16], comp[MAX_COMP];
    int  num;

    int lidos = sscanf(linha,
                       "%*s %19s %19s %15s %d %49s",
                       cpf, cep, face_str, &num, comp);

    if (lidos != 5) {
        fprintf(stderr, "AVISO: linha 'mud' malformada: %s", linha);
        return;
    }

    /* Extract the arguments part (everything after 'mud') */
    const char *args = linha;
    while (*args && !isspace(*args)) args++; /* skip 'mud' command */
    while (*args && isspace(*args)) args++;  /* skip whitespace */
    
    /* Create args string without trailing newline */
    char args_buf[256];
    strncpy(args_buf, args, sizeof(args_buf) - 1);
    args_buf[sizeof(args_buf) - 1] = '\0';
    size_t len = strlen(args_buf);
    if (len > 0 && args_buf[len - 1] == '\n') {
        args_buf[len - 1] = '\0';
    }
    
    fprintf(txt, "\n--- COMANDO MUD --- argumentos: %s ---\n\n", args_buf);

    if (!hashExists(h_hab, cpf)) {
        fprintf(txt, "ERRO: habitante '%s' nao encontrado\n", cpf);
        return;
    }

    if (!hashExists(h_mor, cpf)) {
        fprintf(txt, "ERRO: '%s' nao e morador\n", cpf);
        return;
    }

    /* Monta novo registro de morador */
    MoradorReg mreg;
    strncpy(mreg.cpf, cpf, MAX_CPF - 1); mreg.cpf[MAX_CPF - 1] = '\0';
    strncpy(mreg.cep, cep, MAX_CEP - 1); mreg.cep[MAX_CEP - 1] = '\0';
    strncpy(mreg.complemento, comp, MAX_COMP - 1);
    mreg.complemento[MAX_COMP - 1] = '\0';
    mreg.num = num;

    char *ponto = strchr(face_str, '.');
    mreg.face   = ponto ? *(ponto + 1) : face_str[0];

    /* Insere ou atualiza no hashfile de moradores */
    hashInsertReg(h_mor, cpf, &mreg, sizeof(MoradorReg));

    /* SVG: quadrado vermelho com CPF no destino */
    QuadraReg qreg;
    if (hashGetRegistry(h_quadras, cep, &qreg, sizeof(QuadraReg))) {
        double px, py;
        calcPosEndereco(qreg.x, qreg.y, qreg.w, qreg.h,
                        mreg.face, mreg.num, &px, &py);
        svgMarcaRedSquare(svg, px, py, cpf);
    }
}

static void processar_dspj(const char *linha,
                            Hash h_quadras, Hash h_hab, Hash h_mor,
                            FILE *svg, FILE *txt)
{
    char cpf[MAX_CPF];
    if (sscanf(linha, "%*s %19s", cpf) != 1) {
        fprintf(stderr, "AVISO: linha 'dspj' malformada: %s", linha);
        return;
    }

    fprintf(txt, "[*] dspj %s\n", cpf);

    HabitanteReg hreg;
    if (!hashGetRegistry(h_hab, cpf, &hreg, sizeof(HabitanteReg))) {
        fprintf(txt, "ERRO: habitante '%s' nao encontrado\n", cpf);
        return;
    }

    MoradorReg mreg;
    if (!hashGetRegistry(h_mor, cpf, &mreg, sizeof(MoradorReg))) {
        fprintf(txt, "Habitante não é morador.\n");
        return;
    }

    /* TXT: dados do habitante e endereco do despejo */
    fprintf(txt, "- CPF: %s\n",    hreg.cpf);
    fprintf(txt, "- Nome: %s %s\n", hreg.nome, hreg.sobrenome);
    fprintf(txt, "- Sexo: %c\n", hreg.sexo);
    fprintf(txt, "- Nascimento: %s\n", hreg.nascimento);
    fprintf(txt, "- Endereço do despejo: %s / Face.%c / %d / %s\n",
            mreg.cep, mreg.face, mreg.num, mreg.complemento);

    /* SVG: circulo preto no local do despejo */
    QuadraReg qreg;
    if (hashGetRegistry(h_quadras, mreg.cep, &qreg, sizeof(QuadraReg))) {
        double px, py;
        calcPosEndereco(qreg.x, qreg.y, qreg.w, qreg.h,
                        mreg.face, mreg.num, &px, &py);
        svgMarcaBlackCircle(svg, px, py);
    }

    /* Remove do hashfile de moradores — vira sem-teto */
    hashRemoveReg(h_mor, cpf);
}

/* ─── Interface publica ──────────────────────────────────────────── */

void parseQry(const char *caminho,
              Hash h_quadras, Hash h_hab, Hash h_mor,
              FILE *svg, FILE *txt)
{
    if (caminho == NULL || h_quadras == NULL || h_hab == NULL
        || h_mor == NULL || svg == NULL || txt == NULL) {
        fprintf(stderr, "ERRO: parametro nulo em parseQry\n");
        return;
    }

    FILE *qry = fopen(caminho, "r");
    if (qry == NULL) {
        fprintf(stderr, "ERRO: nao foi possivel abrir '%s'\n", caminho);
        return;
    }

    char linha[MAX_LINHA];

    while (fgets(linha, sizeof(linha), qry) != NULL) {
        if (linha[0] == '\n' || linha[0] == '#') continue;

        char cmd[8];
        if (sscanf(linha, "%7s", cmd) != 1) continue;

        if      (strcmp(cmd, "rq")    == 0)
            processar_rq(linha, h_quadras, h_hab, h_mor, svg, txt);
        else if (strcmp(cmd, "pq")    == 0)
            processar_pq(linha, h_quadras, h_mor, svg, txt);
        else if (strcmp(cmd, "censo") == 0)
            processar_censo(h_hab, h_mor, txt);
        else if (strcmp(cmd, "h?")    == 0)
            processar_h(linha, h_hab, h_mor, txt);
        else if (strcmp(cmd, "nasc")  == 0)
            processar_nasc(linha, h_hab, txt);
        else if (strcmp(cmd, "rip")   == 0)
            processar_rip(linha, h_quadras, h_hab, h_mor, svg, txt);
        else if (strcmp(cmd, "mud")   == 0)
            processar_mud(linha, h_quadras, h_hab, h_mor, svg, txt);
        else if (strcmp(cmd, "dspj")  == 0)
            processar_dspj(linha, h_quadras, h_hab, h_mor, svg, txt);
        else
            fprintf(stderr, "AVISO: comando desconhecido em .qry: '%s'\n", cmd);
    }

    fclose(qry);
}