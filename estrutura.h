#ifndef ESTRUTURA_H
#define ESTRUTURA_H

// valores por defeito
#define MAX_ESTAFETAS 10
#define PRAZO_ACEITA  30
#define DEFAULT_BINDIR "."

// ficheiro para garantir uma so central a correr
#define LOCK_FILE "/tmp/entrega_so_central.lock"

#define TAM_TIPO  12
#define TAM_CORPO 50
#define TAM_NOME  16
#define TAM_LOCAL 16
#define TAM_MAX   256

// cabecalho comum a todas as mensagens
typedef struct {
    char tipo[TAM_TIPO];
} cabecalho;

// mensagem enviada pelo gerador
typedef struct {
    char tipo[TAM_TIPO];
    char corpo[TAM_CORPO];
} msg_pedido;

// registo de um estafeta
typedef struct {
    char tipo[TAM_TIPO];
    char nome[TAM_NOME];
    char fifo_in[TAM_MAX];
    char fifo_out[TAM_MAX];
} msg_registo;

// configuracoes da central
typedef struct {
    int max_estafetas;
    int prazo_aceita;
    char bindir[TAM_MAX];
} configs;

// estatisticas pedidas no comando stats
typedef struct {
    int recebidos;
    int atribuidos;
    int concluidos;
    int expirados;
    int perdidos;
    double valor_total;
} estatisticas;

#endif
