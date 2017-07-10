#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#define N 200

//definição de numeros necessários à execução da fat tree
#define RANGE_REQUISICOES -50
#define ADICIONA_JOB 49
#define SHUTDOWN 50
#define CONFIRMACAO_EXECUCAO 51

#define N_NOS 15 //numero de nós da fat tree

// IDs dos nós
#define ID_1 1
#define ID_2 2
#define ID_3 3
#define ID_4 4
#define ID_5 5
#define ID_6 6
#define ID_7 7
#define ID_8 8
#define ID_9 9
#define ID_10 10
#define ID_11 11
#define ID_12 12
#define ID_13 13
#define ID_14 14
#define ID_15 15

/** Fator multiplicador ou somador para a execução da fat tree*/
int fator_round_robin[4] = {1,10,100,150};

/** Contador do index usado no round robin*/
int index_round_robin;

/** Estrutura que armazena solicitações simples, como adição de novos jobs ao escalonador e shutdown*/
struct mensagem {
	long tipo; // tipo da mensagem, para o escalonador ser usado = 49
	int job; // número do job determinado pelo executa_postergado
	char arq[N]; // nome do arquivo
	int delay; // segundos da postergacao
};

/** Estrutura usada para determinar o último job executado*/
struct job {
	long tipo;
	int job;
};

/** Estrutura que, além de possuir as informações da mensagem, terá outras acerca de execução dos jobs*/
struct exec{
	long tipo; // tipo da mensagem
	int job; //número do job determinado pelo executa_postergado
	char arq[N]; // nome do arquivo
	int tempo; // delay
	int tempo_solicitacao; // tempo de solicitação
	int tempo_inicio; // tempo de início da execução
	int tempo_termino; // tempo de finalização da execução
};

#endif
