#ifndef GERENTE_H
#define GERENTE_H

#include "estrutura.h"
#include "executa_postergado.h"

#define OCUPADO 1
#define LIVRE 0

/** Variável global usado para calcular o identificador do pai ou do filho*/
int identificador;

/** Variáveis globais para armazenar o pid dos filhos criados para cada nó de forma a montar a estrutura da tree*/
int pid_filho_1, pid_filho_2;

/** Variáveis globais que servem como ID das filas de mensagens que vão do sentindo down->up e up->down respectivamente */
int msgqid_up, msgqid_down;

/** Variável global que define o estado do gerente para controle de acesso*/
int estado;

/** Cria estruturas de dados do tipo struct de formas globais. as estruturas das mesmas estão definidas em estrutura.h*/
struct exec msg, msgfilho;

/** Obtem horário atual em segundos
* @param void  
* @return valor do horário atual em inteiro
*/
int obterHorarioAtual();

/** Função de shutdown para os nós da arvore com recepção de sinais de término
* @param sig, sinal recebido
* @return void
*/
void shutdown(int sig);

/** Função principal do gerente.cpp que define a lógica de funcionamento da fat tree quanto ao envio de mensagens e seus tipos
* @param void  
* @return void
*/
void trabalha();

#endif
