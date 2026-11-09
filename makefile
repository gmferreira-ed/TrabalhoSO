
# Primeira regra/acao do make
# Podia ter qualquer outro nome, mas o make corre sempre a primeira regra caso nada seja definido a seguir no comando 
# (eg: make run nao corre istp)
# Abaixo definimos acoes que o "all" precisa que sejam executadas para que este acabe
all: central #estafeta gerador

# Nome acao
program: central.c # Nome ficheiro (so corre se este mesmo mudar)
	gcc central.c -o central 
# ^ Comandos acossiados

# Acoes que correm sempre que sao chamadas
#.PHONY: run -- Nao foi necessario

run: central
	./central