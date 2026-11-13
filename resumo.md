# Estrutura de Ficheiros para o Checkpoint (14 Nov)
## Makefile
O que faz: Compila o projeto.
O que precisa de ter: Os targets exatos: `all`, `central`, `estafeta`, `gerador` e `clean`.

## estruturas.h
O que faz: Centraliza os dados partilhados.
O que precisa de ter:
Constantes com os valores por omissão (`PRAZOACEITA`, `MAXESTAFETAS`, etc).
A `struct` da Central (para guardar as configurações lidas no arranque).
A `struct` das mensagens, garantindo que o primeiro campo é o `char tipo[12]`.

## central.c (O único que trabalha nesta fase)
O que faz: Gere a consola e o tarifador.
O que precisa de ter:
Arranque: Ler variáveis de ambiente e argumentos (`-p`, `-m`).
Instância Única: Código para impedir que duas "centrais" corram ao mesmo tempo.
Tarifador: Lançar o processo do tarifador (`fork` + `exec`) e criar dois pipes anónimos para comunicar com ele.
Consola: Um ciclo para ler comandos do utilizador:
`pedido <origem> <destino> <peso>`: Enviar os dados via pipe para o tarifador e imprimir a resposta.
`stats`: Imprimir as estatísticas base.
`encerra`: Quebrar o ciclo para iniciar o encerramento.
Fim: Função que reage ao comando `encerra` ou ao `SIGINT` (Ctrl+C) -> avisa o tarifador para fechar, faz `wait` por ele e limpa recursos.

## estafeta.c e gerador.c
O que fazem: Nada nesta fase. Não há código de named pipes nem lógicas de clientes.
O que precisam de ter: Apenas um `int main() { return 0; }` básico. Servem apenas para o `Makefile` conseguir criar os executáveis sem dar erro, cumprindo os requisitos de entrega.
