#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#define MAX_ESTAFETAS 10  // estafetas padrão
#define PRAZO_ACEITA  30 // prazo em segundos até expirar
#define DEFAULT_BINDIR        "."  // diretoria atual

// se este ficheiro existe em tmp entao o program está aberto
#define LOCK_FILE             "/tmp/entrega_so_central.lock"

#define TAM_TIPO  12
#define TAM_CORPO 50
#define TAM_MAX   256

typedef struct {
    char tipo[TAM_TIPO];
} Cabecalho;

typedef struct {
    char tipo[TAM_TIPO];
    char corpo[TAM_CORPO];
} Pedido;

// registo de um novo estafeta
typedef struct {
    char tipo[TAM_TIPO];
    char nome[TAM_CORPO];
    char fifo_in[TAM_MAX];
    char fifo_out[TAM_MAX];
} RegistoEst;

typedef struct {
    int max_estafetas;
    int prazo_aceita;
    char bindir[TAM_MAX];
} Configs;

typedef struct {
    int recebidos;
    int atribuidos;
    int concluidos;
    int expirados;
    int perdidos;
    double valor_total;
} Estados;

#endif