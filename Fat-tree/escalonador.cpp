#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <vector>

#include <unistd.h>
#include <time.h>

#include "estrutura.h"
#include "escalonador.h"


int obterHorarioAtual(){
	time_t t = time(NULL);
	struct tm *horario = localtime(&t);
	//Obtem o horário em segundos
	return ((horario->tm_hour*60*60 + horario->tm_min*60 + horario->tm_sec) % 86400);
}

void adicionaExecucaoPostergada(struct mensagem recebido){
	int horario_atual = obterHorarioAtual();
	struct exec novo;

	//Constroi a mensagem na estrutura exec de forma a comporta os tempos de inicio, solicitacao, e futuramente termino
	novo.tipo = ID_1; //Sempre irá enviar para o filho direto na árvore
	novo.job = recebido.job;
	strcpy(novo.arq,recebido.arq);
	novo.tempo = horario_atual + recebido.delay;
	novo.tempo_solicitacao = horario_atual;
	novo.tempo_termino = 0;
	novo.tempo_inicio = 0;

	//Adiciona esse novo job com tais informações de execucao na fila de pendencias, de forma, ao se completar o delay
	//ele seja executado
	execucoes_pendentes.push_back(novo);	
}

void checaEscalonador(){
	int horarioAtual = obterHorarioAtual();

	if(!execucoes_pendentes.empty()){
		std::vector<struct exec>::iterator it = execucoes_pendentes.begin();
		//Verifica o vetor de execucoes pendentes 
		for( ; it != execucoes_pendentes.end(); ){
			//Caso haja algum job que tem seu horário de execucao programa disponivel realiza as seguintes operacoes
			if(it->tempo <= horarioAtual && ocupado != 1){
				ocupado = 1;
				struct exec aux = execucoes_pendentes[std::distance(execucoes_pendentes.begin(), it)];
				execucoes_pendentes.erase(it); //Retira ele do vetor de pendencias

				//Salva a informacao do inicio da execucao na estrutura da mensagem
				int msgsize = sizeof(struct exec) - sizeof(long);
				aux.tempo_inicio = obterHorarioAtual();
				msgsnd(msgqid_down, &aux, msgsize ,0); //Envia a estrutura com o job para o primeiro filho 1
			} else {
				it++;
			}
		}
	}
}

void criarGerentes(){
	int pid_filho_aux[2];
	char parametro1[10];
	char parametro2[10];
	char identificador[5];

	//Criação da árvore manual, de forma que os filhos de cada nó na árvore lógica, sejam filhos também dos processos
	//Cada nós irá receber um identificador com seus respectivos números na árvore
	//Todos os nós exceto os nós do último nível (os que não possuem filhos) possuem o PID de seus filhos
	//Os sprintf a seguir são utilizados para transformar valores inteiros em strings de forma a servirem de parametros

	if((pid_filho = fork()) < 0 ){ printf("Erro na criação de árvore Nó 1\n"); exit(1); }
	if(pid_filho == 0){ //Cria o primeiro nó da árvore

		if((pid_filho_aux[0] = fork()) < 0 ){ printf("Erro na criação de árvore Nó 2\n"); exit(1); }
		if(pid_filho_aux[0] == 0){ //Cria a primeira ramificação do nó 1 dito por nó 2 (Caminho 1->2)

			if((pid_filho_aux[0] = fork()) < 0){ printf("Erro na criação de árvore Nó 4\n"); exit(1); }
			if(pid_filho_aux[0] == 0){ //Cria ramificação inerente ao nó 2, para o nó 4 (1->2->4)

				if((pid_filho_aux[0] = fork()) < 0){ printf("Erro na criação de árvore Nó 8\n"); exit(1); }
				if(pid_filho_aux[0] == 0){ //Cria ramificação inerente ao nó 4, para o nó 8 (1->2->4->8)
					sprintf (identificador, "%d", ID_8);
					execl("gerente", "gerente", identificador,  (char*) NULL);
				}
				if((pid_filho_aux[1] = fork()) < 0){ printf("Erro na criação de árvore nó 9\n"); exit(1); }
				if(pid_filho_aux[1] == 0){ //Cria ramificação inerente ao nó 4, para o nó 9 (1->2->4->9)
					sprintf (identificador, "%d", ID_9);
					execl("gerente", "gerente", identificador, (char*) NULL);
				}

				//Após criar seus filhos, executa nó 4
				sprintf (identificador, "%d", ID_4);
				sprintf (parametro1, "%d", pid_filho_aux[0]); sprintf (parametro2, "%d", pid_filho_aux[1]);
				execl("gerente", "gerente", identificador, parametro1, parametro2, (char*) NULL);
			}

			if((pid_filho_aux[1] = fork()) < 0){ printf("Erro na criação de árvore nó 5\n"); exit(1); }
			if(pid_filho_aux[1] == 0){ //Cria ramificação inerente ao nó 2, para o nó 5 (1->2->5)

				if((pid_filho_aux[0] = fork()) < 0){ printf("Erro na criação de árvore nó 10\n"); exit(1); }
				if(pid_filho_aux[0] == 0){ //Cria ramificação inerente ao nó 5, para o nó 10 (1->2->5->10)
					sprintf (identificador, "%d", ID_10);
					execl("gerente", "gerente", identificador, (char*) NULL);
				}
				if((pid_filho_aux[1] = fork()) < 0){ printf("Erro na criação de árvore nó 11\n"); exit(1); }
				if(pid_filho_aux[1] == 0){ //Cria ramificação inerente ao nó 5, para o nó 11 (1->2->5->11)
					sprintf (identificador, "%d", ID_11);
					execl("gerente", "gerente", identificador, (char*) NULL);
				}
				
				//Após criar seus filhos, executa nó 5
				sprintf (identificador, "%d", ID_5);
				sprintf (parametro1, "%d", pid_filho_aux[0]); sprintf (parametro2, "%d", pid_filho_aux[1]);
				execl("gerente", "gerente", identificador, parametro1, parametro2, (char*) NULL);			
			}

			//Após criar seus filhos, executa nó 2
			sprintf (identificador, "%d", ID_2);
			sprintf (parametro1, "%d", pid_filho_aux[0]); sprintf (parametro2, "%d", pid_filho_aux[1]);
			execl("gerente", "gerente", identificador, parametro1, parametro2, (char*) NULL);
		}



		if((pid_filho_aux[1] = fork()) < 0 ){printf("Erro na criação de árvore nó 3\n"); exit(1); }
		if(pid_filho_aux[1] == 0){ //Cria a segunda ramificação do nó 1 dito por nó 3 (Caminho 1->3)

			if((pid_filho_aux[0] = fork()) < 0){ printf("Erro na criação de árvore nó 6\n"); exit(1); }
			if(pid_filho_aux[0] == 0){ //Cria ramificação inerente ao nó 3, para o nó 6 (1->3->6)
				if((pid_filho_aux[0] = fork()) < 0){ printf("Erro na criação de árvore nó 12\n"); exit(1); }
				if(pid_filho_aux[0] == 0){ //Cria ramificação inerente ao nó 6 para o nó 12 (1->3->6->12)
					sprintf (identificador, "%d", ID_12);
					execl("gerente", "gerente", identificador, (char*) NULL);
				}
				if((pid_filho_aux[1] = fork()) < 0){ printf("Erro na criação de árvore nó 13\n"); exit(1); }
				if(pid_filho_aux[1] == 0){ //Cria ramificação inerente ao nó 6, para o nó 13 (1->3->6->13)
					sprintf (identificador, "%d", ID_13);
					execl("gerente", "gerente", identificador, (char*) NULL);
				}

				//Após criar seus filhos, executa nó 6
				sprintf (identificador, "%d", ID_6);
				sprintf (parametro1, "%d", pid_filho_aux[0]); sprintf (parametro2, "%d", pid_filho_aux[1]);
				execl("gerente", "gerente", identificador, parametro1, parametro2, (char*) NULL);
			}

			if((pid_filho_aux[1] = fork()) < 0){ printf("Erro na criação de árvore nó 7\n"); exit(1); }
			if(pid_filho_aux[1] == 0){ //Cria ramificação inerente ao nó 3, para o nó 7 (1->3->7)
				if((pid_filho_aux[0] = fork()) < 0){ printf("Erro na criação de árvore nó 14\n"); exit(1); }
				if(pid_filho_aux[0] == 0){ //Cria ramificação inerente ao nó 7, para o nó 14 (1->3->7->14)
					sprintf (identificador, "%d", ID_14);
					execl("gerente", "gerente", identificador, (char*) NULL);
				}
				if((pid_filho_aux[1] = fork()) < 0){ printf("Erro na criação de árvore nó 15\n"); exit(1); }
				if(pid_filho_aux[1] == 0){ //Cria ramificação inerente ao nó 7, para o nó 15 (1->3->7->15)
					sprintf (identificador, "%d", ID_15);
					execl("gerente", "gerente", identificador, (char*) NULL);
				}

				//Após criar seus filhos, executa nó 7
				sprintf (identificador, "%d", ID_7);
				sprintf (parametro1, "%d", pid_filho_aux[0]); sprintf (parametro2, "%d", pid_filho_aux[1]);
				execl("gerente", "gerente", identificador, parametro1, parametro2, (char*) NULL);
			}

			//Após criar seus filhos, executa nó 3
			sprintf (identificador, "%d", ID_3);
			sprintf (parametro1, "%d", pid_filho_aux[0]); sprintf (parametro2, "%d", pid_filho_aux[1]);
			execl("gerente", "gerente", identificador, parametro1, parametro2, (char*) NULL);
		}

		//Após criar seus filhos, executa nó 1
		sprintf (identificador, "%d", ID_1);
		sprintf (parametro1, "%d", pid_filho_aux[0]); sprintf (parametro2, "%d", pid_filho_aux[1]);
		execl("gerente", "gerente", identificador, parametro1, parametro2, (char*) NULL);
	}
}

void imprimeConcluido(struct exec finalizado){

	//Realiza conversão do valor absoluto de segundos do tempo nas estruturas para Horas, Minutos e segundos
	//De forma a facilitar a visualização pós impressão
	int hora_ini = (finalizado.tempo_inicio / (60*60));
	int minuto_ini = ((finalizado.tempo_inicio - (hora_ini*60*60))/60);
	int segundo_ini = (finalizado.tempo_inicio - hora_ini*60*60 - minuto_ini*60);
	int hora_fim = (finalizado.tempo_termino / (60*60));
	int minuto_fim = ((finalizado.tempo_termino - (hora_fim*60*60))/60);
	int segundo_fim = (finalizado.tempo_termino - hora_fim*60*60 - minuto_fim*60);
	printf("\nJob: %d terminado, arquivo: %s, inicio de execucao as %dh%dm%ds e fim %dh%dm%ds, com turnaround de: %d\n",
		finalizado.job, finalizado.arq, hora_ini,minuto_ini,segundo_ini,
		hora_fim,minuto_fim,segundo_fim, finalizado.tempo_termino - 
		finalizado.tempo_inicio);
}

void imprimeRestante(){
	if(!execucoes_pendentes.empty()){
		printf("\nExecucoes que ficaram pendentes:\n");
		//Percorre vetor de execucoes pendentes para imprimir os jobs que estavam esperando para serem executados
		for(std::vector<int>::size_type i = 0; i != execucoes_pendentes.size(); i++) {
   			printf("Job: %d, arquivo %s, delay %d\n", (int) execucoes_pendentes[i].job,
   			 execucoes_pendentes[i].arq, execucoes_pendentes[i].tempo - execucoes_pendentes[i].tempo_solicitacao );
		}
	}
	execucoes_pendentes.clear();
}

void imprimeExecutados(){
	if(!execucoes_terminadas.empty()){
		printf("\nExecucoes com exito:");
		//Percorre vetor de execucoes terminadas para imprimir as estatísticas corretamente de cada job finalizado
		for(std::vector<int>::size_type i = 0; i != execucoes_terminadas.size(); i++) {
   			imprimeConcluido(execucoes_terminadas[i]);
		}
	}
	execucoes_terminadas.clear();

}

void shutdown(int sig){
	printf("\nServidor desligando...\n");
	kill(pid_filho, SIGUSR1); //Envia sinais para o Filho direto do escalonador (primeiro nó identificador 1)
	
	int status;
	wait(&status); //Espera o exit do primeiro nó

	imprimeRestante(); //Imprime os jobs que não foram executados
	imprimeExecutados(); //Imprime os jobs que foram executados com as devidas estatísticas

	//Exclui ambas filas utilizadas no escalonador multi processos
	msgctl(msgqid_up, IPC_RMID, NULL); 
	msgctl(msgqid_down, IPC_RMID, NULL);
	exit(0);
}


void executaEscalonador(){
	signal(SIGUSR1, shutdown); //Rotina de desligamento caso receba sinal
	cont_termino = 0; // Contador de processos terminados (min 0, max 15)
	ocupado = 0; // Valor de ocupação dos gerentes de processos

	//Fila de mensagens direcionadas ao escalonador ("Volta/Subida")
	key_t msgkey_up = 0x14002713;
	if((msgqid_up = msgget(msgkey_up, IPC_CREAT | 0x1B6)) < 0){ //permissão: 110 110 110 (RWX)
		printf("Erro na criação da fila a partir do msgget"); exit(1);
    }
   	//Fila de mensagens que saem do escalonador ("Ida/Descida")
   	key_t msgkey_down = 0x14003093;
	if((msgqid_down = msgget(msgkey_down, IPC_CREAT | 0x1B6)) < 0){ //permissão: 110 110 110 (RWX)
		printf("Erro na criação da fila a partir do msgget"); exit(1);
   	}

   	//Cria nós da fila
	criarGerentes();

	//Estrutura de mensagem simples para receber solicitações
	struct mensagem msg;
	//Estrutura de mensagem simples para tratar de jobs finalizados (contém tempos)
	struct exec finalizado;

	//Tamanho das estruturas acima
	int msgsize = sizeof(struct mensagem) - sizeof(long);
	int execsize = sizeof(struct exec) - sizeof(long);

	while(true){
		//Recebe mensagens abaixo do valor do RANGE_REQUISICOES (Valores menores que 50), os valores dos cases utilizados 
		//se encontram no arquivo estrutura.h
		if(msgrcv(msgqid_up, &msg, msgsize, RANGE_REQUISICOES, IPC_NOWAIT) != -1){
			//Caso receba a mensagem verifica qual ação deve ser tomada baseada em seu tipo.
			switch(msg.tipo){
				case ADICIONA_JOB:
					adicionaExecucaoPostergada(msg);
					break;
				case SHUTDOWN:
					raise(SIGUSR1);
				break;
			}	
		}

		//Recebe a estrutura de jobs finalizados dos nós ao final da execucao, para realizar as estatisticas.
		if(msgrcv(msgqid_up, &finalizado, execsize, CONFIRMACAO_EXECUCAO + fator_round_robin[0], IPC_NOWAIT) != -1){
			//printf("A\n");
			cont_termino++; 
		}
		if(msgrcv(msgqid_up, &finalizado, execsize, CONFIRMACAO_EXECUCAO + fator_round_robin[1], IPC_NOWAIT) != -1){
			//printf("B\n");
			cont_termino++; 
		}
		if(msgrcv(msgqid_up, &finalizado, execsize, CONFIRMACAO_EXECUCAO + fator_round_robin[2], IPC_NOWAIT) != -1){
			//printf("C\n");
			cont_termino++; 
		}
		if(msgrcv(msgqid_up, &finalizado, execsize, CONFIRMACAO_EXECUCAO + fator_round_robin[3], IPC_NOWAIT) != -1){
			//printf("D\n");
			cont_termino++; 
		}
		if(cont_termino == N_NOS){
			ocupado = 0; //Caso todos os processos tenham sido encerrados e com a devida confirmacao, libera para outro job
			cont_termino = 0; 
			finalizado.tempo_termino = obterHorarioAtual();
			execucoes_terminadas.push_back(finalizado); //Adiciona ao vetor de execucoes terminadas.
			finalizado = execucoes_terminadas.back(); //Imprime as informações do job que acabou de ser finalizado
			imprimeConcluido(finalizado);	
		}

		//Rotina para testar os tempos de delay e comandar a execucao de jobs
		checaEscalonador();
	}
}

int main(){
	executaEscalonador();
	return 0; 
}
