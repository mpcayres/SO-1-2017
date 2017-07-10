#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "estrutura.h"

#include <vector>
#include <queue>

/** Identificador da fila de recebimento de mensagens*/
int msgqid_up;

/** Identificador da fila de envio de mensagens*/
int msgqid_down;

/** Process Id do filho*/
int pid_filho;

/** Contador de jobs que finalizaram a execução*/
int cont_termino;

/** Variável para impedir o lançamento de novos jobs enquanto há outro em execução **/
int ocupado;

/** Vetor de estruturas de execução para as execuções pendentes*/
std::vector<struct exec> execucoes_pendentes;

/** Vetor de estruturas de execução para execuções terminadas*/
std::vector<struct exec> execucoes_terminadas;


/** Obtém a hora atual do sistema baseado em um cálculo com a data (01/01/1970)
* @param void
* @return quantidade de segundos desde que começou o dia (00:00)
*/
int obterHorarioAtual();

/** Adiciona ao final do vetor de execuções pendentes uma nova 
* estrutura de execução do tipo 1 baseado na mensagem recebida
* @param recebido, uma mensagem recebida pelo escalonador
* @return void
*/
void adicionaExecucaoPostergada(struct mensagem recebido);

/** Itera o vetor de execuções pendentes se ainda houver itens nele
* e envia uma mensagem de job para o filho se a estrutura de excução corrente
* tiver um tempo de solicitação + delay menor ou igual do que a hora atual
* @param void 
* @return void
*/
void checaEscalonador();

/** Cria a árvore de gerentes de acordo com os seus respectivos níveis
* @param void 
* @return void
*/
void criarGerentes();

/** Imprime um job que foi executado com êxito
* com a ciência do escalonador
* @param finalizado, estrutura que armazena os dados do job a ser impresso
* @return void
*/
void imprimeConcluido(struct exec finalizado);

/** Imprime os jobs restantes no vetor de execuções pendentes,
* ou seja, que não foram executadas
* @param void  
* @return void
*/
void imprimeRestante();

/** Imprime todos os jobs que foram executados com êxito
* com a ciência do escalonador
* @param void
* @return void
*/
void imprimeExecutados();

/** Mata os processos filhos, imprime os jobs que foram e não
* foram executados, e remove do kernel as filas de 
* envio e recebimento de mensagens.
* @param sig
* @return void
*/
void shutdown(int sig);

/** Função principal da execução do escalonador:
*	- Prepara as filas de envio e recebimento de mensagens
* 	- Cria os gerentes
*	- Ouve as mensagens recebidas e toma decisões de acordo com elas
*	- Checa o escalonador
* @param void 
* @return void
*/
void executaEscalonador();

#endif