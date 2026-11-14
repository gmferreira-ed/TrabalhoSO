#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

#include "estrutura.h"

int fd_lock = -1;
int pid_tarifador = -1;
int pipe_envia[2];  // central -> tarifador
int pipe_recebe[2]; // tarifador -> centrale
estatisticas stats = {0, 0, 0, 0, 0, 0.0};

void fecha_tudo() {
    // fecha pipes para avisar o tarifador a terminar
    if (pipe_envia[1] != -1) {
        close(pipe_envia[1]);
        pipe_envia[1] = -1;
    }
    if (pipe_recebe[0] != -1) {
        close(pipe_recebe[0]);
        pipe_recebe[0] = -1;
    }

    // espera pelo tarifador sem deixar processos pendurados
    if (pid_tarifador > 0) {
        wait(NULL);
        pid_tarifador = -1;
    }

    // remove o lock de instancia unica
    if (fd_lock != -1) {
        close(fd_lock);
        unlink(LOCK_FILE);
        fd_lock = -1;
    }
}

void trata_sigint(int s) {
    (void)s;
    printf("\nA encerrar a central...\n");
    fecha_tudo();
    exit(0);
}

int main(int argc, char *argv[]) {
    configs cfg;

    // valores padrao
    cfg.max_estafetas = MAX_ESTAFETAS;
    cfg.prazo_aceita = PRAZO_ACEITA;
    strcpy(cfg.bindir, DEFAULT_BINDIR);

    // variaveis de ambiente
    char *env_m = getenv("MAXESTAFETAS");
    if (env_m) cfg.max_estafetas = atoi(env_m);

    char *env_p = getenv("PRAZOACEITA");
    if (env_p) cfg.prazo_aceita = atoi(env_p);

    char *env_b = getenv("BINDIR");
    if (env_b) strcpy(cfg.bindir, env_b);

    // argumentos opcionais -p e -m
    int opt;
    while ((opt = getopt(argc, argv, "p:m:")) != -1) {
        if (opt == 'p') cfg.prazo_aceita = atoi(optarg);
        if (opt == 'm') cfg.max_estafetas = atoi(optarg);
    }

    // instancia unica
    fd_lock = open(LOCK_FILE, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd_lock == -1) {
        printf("Erro: Ja existe uma central em execucao!\n");
        return 1;
    }

    // sinal SIGINT (Ctrl+C)
    signal(SIGINT, trata_sigint);

    // pipes anonimos para falar com o tarifador
    if (pipe(pipe_envia) == -1 || pipe(pipe_recebe) == -1) {
        printf("Erro ao criar pipes!\n");
        fecha_tudo();
        return 1;
    }

    pid_tarifador = fork();
    if (pid_tarifador == -1) {
        printf("Erro no fork!\n");
        fecha_tudo();
        return 1;
    }

    if (pid_tarifador == 0) {
        // processo filho: tarifador
        // redireciona stdin para ler do pipe_envia
        dup2(pipe_envia[0], 0);
        // redireciona stdout para escrever no pipe_recebe
        dup2(pipe_recebe[1], 1);

        close(pipe_envia[0]);
        close(pipe_envia[1]);
        close(pipe_recebe[0]);
        close(pipe_recebe[1]);

        char caminho[TAM_MAX];
        snprintf(caminho, sizeof(caminho), "%s/tarifador", cfg.bindir);
        execl(caminho, "tarifador", NULL);

        perror("Erro ao executar tarifador");
        exit(1);
    }

    // processo pai: fecha pontas nao usadas
    close(pipe_envia[0]);
    close(pipe_recebe[1]);

    printf("Central iniciada (max_estafetas=%d, prazo=%d, bindir=%s)\n",
           cfg.max_estafetas, cfg.prazo_aceita, cfg.bindir);

    // consola de comandos
    char linha[TAM_MAX];
    while (1) {
        printf("central> ");
        fflush(stdout);

        if (!fgets(linha, sizeof(linha), stdin)) {
            break;
        }

        linha[strcspn(linha, "\r\n")] = '\0';
        if (strlen(linha) == 0) continue;

        char cmd[32];
        sscanf(linha, "%s", cmd);

        if (strcmp(cmd, "encerra") == 0) {
            break;
        } else if (strcmp(cmd, "stats") == 0) {
            printf("--- Estatisticas ---\n");
            printf("Recebidos:  %d\n", stats.recebidos);
            printf("Atribuidos: %d\n", stats.atribuidos);
            printf("Concluidos: %d\n", stats.concluidos);
            printf("Expirados:  %d\n", stats.expirados);
            printf("Perdidos:   %d\n", stats.perdidos);
            printf("Valor total: %.2f euros\n", stats.valor_total);
        } else if (strcmp(cmd, "pedido") == 0) {
            char orig[TAM_LOCAL], dest[TAM_LOCAL];
            float peso;

            if (sscanf(linha, "%*s %s %s %f", orig, dest, &peso) != 3) {
                printf("Uso: pedido <origem> <destino> <peso>\n");
            } else {
                char buffer[TAM_MAX];
                sprintf(buffer, "%s %s %.2f\n", orig, dest, peso);
                write(pipe_envia[1], buffer, strlen(buffer));

                // ler resposta do tarifador (distancia e valor)
                int n = read(pipe_recebe[0], buffer, sizeof(buffer) - 1);
                if (n > 0) {
                    buffer[n] = '\0';
                    float dist, valor;
                    if (sscanf(buffer, "%f %f", &dist, &valor) == 2) {
    stats.recebidos++;
    stats.valor_total += valor;
    printf("Tarifador: distancia=%.2f km, valor=%.2f euros\n", dist, valor);
} else {
    printf("DEBUG ERRO: string recebida foi [%s]\n", buffer);
}
                } else {
                    printf("Erro ao ler resposta do tarifador!\n");
                }
            }
        } else {
            printf("Comando desconhecido: '%s'. Comandos: pedido, stats, encerra\n", cmd);
        }
    }

    fecha_tudo();
    printf("Central terminada.\n");
    return 0;
}
