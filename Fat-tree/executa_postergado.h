#ifndef EXECUTA_POSTERGADO_H
#define EXECUTA_POSTERGADO_H

#include "estrutura.h"

#include <vector>
#include <queue>

/** Identificador de job*/
int job_id;

/** Identificador da fila de mensagens*/
int msgqid;

/** Identificador da fila de mensagens para identificadores de jobs*/
int msgqidjob;

/** Checa se o número de parametros passados está correto
* @param quantidade, número de parâmetros passados (argc)
* @return void
*/
void checaQuantidade(int quantidade);

/** Checa se o parâmetro passado de delay de execução é um número
* @param seg, valor a ser testado se é um número
* @return void
*/
void numOk(const char * seg);

/** Checa se é possível abrir o arquivo executavel
* @param arqExe, string com o nome do arquivo
* @return void
*/
void arquivoOK(char * arqExe);

/** Recebe via mensagem o identificador do último job do escalonador
* @param void  
* @return valor inteiro do último job encontrado
*/
int encontraUltimoJob();

/** Função principal da execução postergada:
*	- Prepara as filas de mensagens para identificadores de jobs
*	- Prepara as filas de mensagens 
*	- Prepara a estrutuda da mensagem do job a ser enviada
*	- Enviar o novo job inserido pelo usuário via mensagem
* @param seg, tempo em segundos para o delay de execução
* @param arq_executavel[N], string contendo o nome do arquivo executável
* @return void
*/
void executaPostergado(int seg, char arq_executavel[N]);

#endif