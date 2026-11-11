#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // getopt para obter argumentos
#include <fcntl.h>

#include "estrutura.h"

int main(int argc, char *argv[]) {
    Configs cfg;

    // define os valores normais do estrutura.h
    cfg.max_estafetas = MAX_ESTAFETAS;
    cfg.prazo_aceita = PRAZO_ACEITA;
    strcpy(cfg.bindir, DEFAULT_BINDIR);

    // se o utilizador definiu variaveis, entao substitui
    char *env_m = getenv("MAXESTAFETAS");
    if (env_m) cfg.max_estafetas = atoi(env_m);

    char *env_p = getenv("PRAZOACEITA");
    if (env_p) cfg.prazo_aceita = atoi(env_p);

    char *env_b = getenv("BINDIR");
    if (env_b) strcpy(cfg.bindir, env_b);

    // getopt procura os numeros a seguir aos argumentos "p"/"m" e substitui
    int opt;
    while ((opt = getopt(argc, argv, "p:m:")) != -1) {
        if (opt == 'p') cfg.prazo_aceita = atoi(optarg);
        if (opt == 'm') cfg.max_estafetas = atoi(optarg);
    }

    // Cria um ficheiro, mas se ja existir falha e dá erro  (-1 já existe) e fecha logo
    int fd_lock = open(LOCK_FILE, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd_lock == -1) {
        printf("Erro: Já existe uma central a correr!\n");
        exit(1);
    }

    printf("Central está a iniciar\n");

    // unlink apaga o ficheiro
    close(fd_lock);
    unlink(LOCK_FILE);

    return 0;
}