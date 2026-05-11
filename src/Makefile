PROJ_NAME = ted
ALUNO     = Felipe Augusto Castilho Sepulveda

CC      = gcc
CFLAGS  = -ggdb -O0 -std=c99 -fstack-protector-all \
          -Werror=implicit-function-declaration \
          -DUNITY_INCLUDE_DOUBLE \
          -I$(SRC_DIR) -I$(UNITY_DIR)
LDFLAGS = -O0

SRC_DIR   = .
TST_DIR   = tst
UNITY_DIR = .

# ── Módulos do projeto ──────────────────────────────────────────────
MODULOS = quadra habitante morador hte svg parse_geo parse_pm parse_qry

OBJETOS = $(patsubst %, $(SRC_DIR)/%.o, $(MODULOS)) $(SRC_DIR)/ted.o

# ════════════════════════════════════════════════════════════════════
# Alvo principal
# ════════════════════════════════════════════════════════════════════

$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $@ $(LDFLAGS) $(OBJETOS)

# ── Regra genérica para objetos ──────────────────────────────────────
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

# ── Dependências de cada módulo ──────────────────────────────────────
$(SRC_DIR)/ted.o:       $(SRC_DIR)/ted.c \
                        $(SRC_DIR)/hte.h $(SRC_DIR)/svg.h \
                        $(SRC_DIR)/quadra.h \
                        $(SRC_DIR)/parse_geo.h $(SRC_DIR)/parse_pm.h \
                        $(SRC_DIR)/parse_qry.h

$(SRC_DIR)/quadra.o:    $(SRC_DIR)/quadra.c    $(SRC_DIR)/quadra.h
$(SRC_DIR)/habitante.o: $(SRC_DIR)/habitante.c $(SRC_DIR)/habitante.h
$(SRC_DIR)/morador.o:   $(SRC_DIR)/morador.c   $(SRC_DIR)/morador.h
$(SRC_DIR)/hte.o:       $(SRC_DIR)/hte.c        $(SRC_DIR)/hte.h
$(SRC_DIR)/svg.o:       $(SRC_DIR)/svg.c        $(SRC_DIR)/svg.h \
                        $(SRC_DIR)/quadra.h     
$(SRC_DIR)/parse_geo.o: $(SRC_DIR)/parse_geo.c  $(SRC_DIR)/parse_geo.h \
                        $(SRC_DIR)/hte.h        $(SRC_DIR)/quadra.h \
                        $(SRC_DIR)/svg.h
$(SRC_DIR)/parse_pm.o:  $(SRC_DIR)/parse_pm.c   $(SRC_DIR)/parse_pm.h \
                        $(SRC_DIR)/hte.h
$(SRC_DIR)/parse_qry.o: $(SRC_DIR)/parse_qry.c  $(SRC_DIR)/parse_qry.h \
                        $(SRC_DIR)/parse_geo.h  $(SRC_DIR)/parse_pm.h \
                        $(SRC_DIR)/hte.h        $(SRC_DIR)/quadra.h \
                        $(SRC_DIR)/svg.h

# ════════════════════════════════════════════════════════════════════
# Testes unitários
# ════════════════════════════════════════════════════════════════════

UNITY_OBJ = $(UNITY_DIR)/unity.c

# ── hte ─────────────────────────────────────────────────────────────
t_hte: $(TST_DIR)/t_hte.c $(SRC_DIR)/hte.o
	$(CC) $(CFLAGS) $^ $(UNITY_OBJ) -o $(TST_DIR)/t_hte
	$(TST_DIR)/t_hte

# ── quadra ──────────────────────────────────────────────────────────
t_quadra: $(TST_DIR)/t_quadra.c $(SRC_DIR)/quadra.o
	$(CC) $(CFLAGS) $^ $(UNITY_OBJ) -o $(TST_DIR)/t_quadra
	$(TST_DIR)/t_quadra

# ── habitante ───────────────────────────────────────────────────────
t_habitante: $(TST_DIR)/t_habitante.c $(SRC_DIR)/habitante.o
	$(CC) $(CFLAGS) $^ $(UNITY_OBJ) -o $(TST_DIR)/t_habitante
	$(TST_DIR)/t_habitante

# ── morador ─────────────────────────────────────────────────────────
t_morador: $(TST_DIR)/t_morador.c $(SRC_DIR)/morador.o
	$(CC) $(CFLAGS) $^ $(UNITY_OBJ) -o $(TST_DIR)/t_morador
	$(TST_DIR)/t_morador

# ── parse_geo ───────────────────────────────────────────────────────
t_parse_geo: $(TST_DIR)/t_parse_geo.c $(SRC_DIR)/parse_geo.o \
             $(SRC_DIR)/hte.o $(SRC_DIR)/quadra.o $(SRC_DIR)/svg.o
	$(CC) $(CFLAGS) $^ $(UNITY_OBJ) -o $(TST_DIR)/t_parse_geo
	$(TST_DIR)/t_parse_geo

# ── parse_pm ────────────────────────────────────────────────────────
t_parse_pm: $(TST_DIR)/t_parse_pm.c $(SRC_DIR)/parse_pm.o \
            $(SRC_DIR)/hte.o $(SRC_DIR)/habitante.o $(SRC_DIR)/morador.o
	$(CC) $(CFLAGS) $^ $(UNITY_OBJ) -o $(TST_DIR)/t_parse_pm
	$(TST_DIR)/t_parse_pm

# ── svg ─────────────────────────────────────────────────────────────
t_svg: $(TST_DIR)/t_svg.c $(SRC_DIR)/svg.o $(SRC_DIR)/quadra.o 
	$(CC) $(CFLAGS) $^ $(UNITY_OBJ) -o $(TST_DIR)/t_svg
	$(TST_DIR)/t_svg

# ── parse_qry ───────────────────────────────────────────────────────
t_parse_qry: $(TST_DIR)/t_parse_qry.c $(SRC_DIR)/parse_qry.o \
             $(SRC_DIR)/parse_geo.o $(SRC_DIR)/parse_pm.o \
             $(SRC_DIR)/hte.o $(SRC_DIR)/quadra.o $(SRC_DIR)/svg.o 
	$(CC) $(CFLAGS) $^ $(UNITY_OBJ) -o $(TST_DIR)/t_parse_qry
	$(TST_DIR)/t_parse_qry

# ── Agregador de testes (roda todos em uma chamada) ──────────────────
tests: $(TST_DIR)/test_runner.c $(OBJETOS)
	$(CC) $(CFLAGS) $^ $(UNITY_OBJ) -o $(TST_DIR)/test_runner
	$(TST_DIR)/test_runner

# ── Roda todos os testes individualmente ──────────────────────────────
tstall: t_hte t_quadra t_habitante t_morador t_parse_geo \
        t_parse_pm t_svg t_parse_qry

# ════════════════════════════════════════════════════════════════════
# Limpeza
# ════════════════════════════════════════════════════════════════════
clean:
	rm -f $(OBJETOS) $(SRC_DIR)/$(PROJ_NAME)
	rm -f $(TST_DIR)/t_hte $(TST_DIR)/t_quadra \
	      $(TST_DIR)/t_habitante $(TST_DIR)/t_morador \
	      $(TST_DIR)/t_parse_geo $(TST_DIR)/t_parse_pm \
	      $(TST_DIR)/t_svg $(TST_DIR)/t_parse_qry \
	      $(TST_DIR)/test_runner

.PHONY: tstall tests t_hte t_quadra t_habitante t_morador \
        t_parse_geo t_parse_pm t_svg t_parse_qry clean
