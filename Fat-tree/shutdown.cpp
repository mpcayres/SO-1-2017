#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <signal.h>

#include "estrutura.h"

//programa necessário para dar shutdown de forma que não sejam deixados processos abertos
int main(){
	key_t msgkey = 0x14002713;
	int msgqid;
	if((msgqid = msgget(msgkey, 0x1B6)) < 0){ //permissão: 110 110 110 (RWX)
		printf("Erro ao executar msggeta");
		exit(1);
	}

	key_t msgkey_job = 0x14001920;
	int msgqidjob;
	if((msgqidjob = msgget(msgkey_job, 0x1B6)) > 0){ //permissão: 110 110 110 (RWX)
		msgctl(msgqidjob, IPC_RMID, NULL);
	}
	

	struct mensagem msg = { SHUTDOWN, 0, ' ', 0, 0 }; //Envia mensagem com tipo Shutdown (presente em estrutura.h)
	int msg_tam = sizeof(struct mensagem) - sizeof(long);
    	if(msgsnd(msgqid, &msg, msg_tam, 0) < 0){
    		printf("Erro ao executar msgsnd: ");
    		exit(1);
    	}
	return 0;
}
