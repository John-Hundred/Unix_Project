#include "FunzioniUnix.h"
#include "MyTypes.h"
#include "FileConf.h"
#include "Struct.h"
#include "MyError.h"
#include "MyFunction.h"


int gravita(char *s){

	FILE * pr=NULL;
	char *str=NULL,*eptr=NULL;
	int gravita;		
	
	str=(char *)malloc(sizeof(char)*5);  
	
		
	if(strcmp(s,"ACNE")==0) {              
		
		pr=fopen("Acne.txt","r");
		if(pr==NULL)
			ErrExit("Errore apertura file \n ");
			
		while(fscanf(pr,"%s",str)!=EOF);
		
		gravita=(int)strtol(str,&eptr,10);
		if(*eptr!='\0')
			ErrExit(" strtol fallita \n ");
			
	} else if(strcmp(s,"ANEMIA")==0) {
		
		pr=fopen("Anemia.txt","r");
		if(pr==NULL)
			ErrExit("Errore apertura file \n ");
			
		while(fscanf(pr,"%s",str)!=EOF);
			
		gravita=(int)strtol(str,&eptr,10);
		if(*eptr!='\0')
			ErrExit(" strtol fallita \n ");	
			
	}else if(strcmp(s,"FEBBRE")==0) {
		
		pr=fopen("Febbre.txt","r");
		if(pr==NULL)
			ErrExit("Errore apertura file \n ");
			
		while(fscanf(pr,"%s",str)!=EOF);
			
		gravita=(int)strtol(str,&eptr,10);
		if(*eptr!='\0')
			ErrExit(" strtol fallita \n ");
			
	}else if(strcmp(s,"ALLUCINAZIONI")==0) {
		
		pr=fopen("Allucinazioni.txt","r");
		if(pr==NULL)
			ErrExit("Errore apertura file \n ");
			
		while(fscanf(pr,"%s",str)!=EOF);
			
		gravita=(int)strtol(str,&eptr,10);
		if(*eptr!='\0')
			ErrExit(" strtol fallita \n ");
			
	}else if(strcmp(s,"DIARREA")==0) {
		
		pr=fopen("Diarrea.txt","r");
		if(pr==NULL)
			ErrExit("Errore apertura file \n ");
			
		while(fscanf(pr,"%s",str)!=EOF);
			
		gravita=(int)strtol(str,&eptr,10);
		if(*eptr!='\0')
			ErrExit(" strtol fallita \n ");
			
	}else if(strcmp(s,"TOSSE")==0) {
		
		pr=fopen("Tosse.txt","r");
		if(pr==NULL)
			ErrExit("Errore apertura file \n ");
			
		while(fscanf(pr,"%s",str)!=EOF);
			
		gravita=(int)strtol(str,&eptr,10);
		if(*eptr!='\0')
			ErrExit(" strtol fallita \n ");
			
	}else if(strcmp(s,"VOMITO")==0) {
		
		pr=fopen("Vomito.txt","r");
		if(pr==NULL)
			ErrExit("Errore apertura file \n ");
			
		while(fscanf(pr,"%s",str)!=EOF);
			
		gravita=(int)strtol(str,&eptr,10);
		if(*eptr!='\0')
			ErrExit(" strtol fallita \n ");
			
	}else if(strcmp(s,"TREMORI")==0) {
		
		pr=fopen("Tremori.txt","r");
		if(pr==NULL)
			ErrExit("Errore apertura file \n ");
			
		while(fscanf(pr,"%s",str)!=EOF);
			
		gravita=(int)strtol(str,&eptr,10);
		if(*eptr!='\0')
			ErrExit(" strtol fallita \n ");
			
	}else if(strcmp(s,"ERITEMA")==0) {
		
		pr=fopen("Eritema.txt","r");
		if(pr==NULL)
			ErrExit("Errore apertura file \n ");
			
		while(fscanf(pr,"%s",str)!=EOF);
			
		gravita=(int)strtol(str,&eptr,10);
		if(*eptr!='\0')
			ErrExit(" strtol fallita \n ");
			
	}else if(strcmp(s,"INSONNIA")==0) {
		
		pr=fopen("Insonnia.txt","r");
		if(pr==NULL)
			ErrExit("Errore apertura file \n ");
			
		while(fscanf(pr,"%s",str)!=EOF);
			
			
		gravita=(int)strtol(str,&eptr,10);
		if(*eptr!='\0')
			ErrExit(" strtol fallita \n ");
			
	}else
		printf("ERRORE INATTESO \n");
	
	free(str);
	fclose(pr);
	   
	
	return gravita;
}


char * sintomo(void) {
	FILE *pr=NULL;
	char *str=NULL;	
	
		
	pr=fopen("Sintomo.txt","r");
	
	if(pr==NULL) 
		ErrExit("Errore apertura file \n ");
	
	
	str=(char *)malloc(sizeof(char)*20);		
	
	while(fscanf(pr,"%s",str)!=EOF);
	
	fclose(pr);
	
	
	return str;
	
}


void scriviSintomo(void) {
	int casuale;
	FILE *pr=NULL;
		
	sleep(1);
	
	srand((unsigned int)time(NULL));   /* genera il seed per cambiare i numeri casuali ad ogni esecuzione  */
	casuale=1+rand()%10;   /*  genera un numero casuale da 1 a 10   */
	pr=fopen("Sintomo.txt","w"); 
	if(pr==NULL) 
		ErrExit(" Errore scrittura file \n ");
	
	
	switch(casuale) {    /*  a seconda del numero scrivo il sintomo sul file  */
		case ACNE:
			fprintf(pr,"ACNE");
			break;
		case ANEMIA:
			fprintf(pr,"ANEMIA");
			break;
		case FEBBRE:
			fprintf(pr,"FEBBRE");
			break;
		case ALLUCINAZIONI:
			fprintf(pr,"ALLUCINAZIONI");
			break;
		case DIARREA:
			fprintf(pr,"DIARREA");
			break;
		case TOSSE:
			fprintf(pr,"TOSSE");
			break;
		case VOMITO:
			fprintf(pr,"VOMITO");
			break;
		case TREMORI:
			fprintf(pr,"TREMORI");
			break;
		case ERITEMA:
			fprintf(pr,"ERITEMA");
			break;
		case INSONNIA:
			fprintf(pr,"INSONNIA");
			break;
	}
	fclose(pr);
	
}		



void  handler(int sig) {
	
	int idSem, idMsg, idmsgPri ,idmsgPri2;
	
			
	if((idmsgPri=msgget(KEY1,0400|0200))==-1)    /*  ottengo gli identificatori   */
		ErrExit("msgget failed ");	
	if((idmsgPri2=msgget(KEY2,0400|0200))==-1)    /*  "   "      */
		ErrExit("msgget failed ");	
	
	
	if((idMsg=msgget(KEY3,0400|0200))==-1)  /*     "     "        */
		ErrExit("msgget failed ");
		
		
	if((idSem=semget(KEY4,4,0400|0200))==-1)   /*  "   "       */
		ErrExit(" semGet failed ");
	
	
		
	
	
	if(sig==SIGALRM) {     /* se ricevo SIGALRM    CHIUDO ACCESSI OSPEDALE	  */	
		
				
		arg.val=50;
		
		if(semctl(idSem,4,SETVAL,arg)==-1)   /*  semaforo di blocco    */
			ErrExit("Errore semctl  ");
				
			
		printf(" \n  L'ACCESSO AL OSPEDALE E STATO CHIUSO ! NON VERRANNO ACCETTATI ALTRI PAZIENTI ! \n");	
				
				
	}else if(sig==SIGQUIT) {   
		if(getpid()==pidGestore)   /* elimina il processo gestore  */
			exit(EXIT_SUCCESS);		
			
		
		unlink("1reparto");   /* cancello le fifo presenti */
		unlink("2reparto");	
		
		printf(" \n FIFO ELIMINATE \n ");
		
		
		if(semctl(idSem,0,IPC_RMID,0)==-1)   /* rimuovo il semaforo     */
			ErrExit("Errore semctl  ");
				
				
		printf(" \n SEMAFORI RIMOSSI \n ");
		
		if(msgctl(idMsg,IPC_RMID,NULL)==-1)    /*  rimuovo code di messaggo */
			ErrExit("errore msgctl");
		
		if(msgctl(idmsgPri,IPC_RMID,NULL)==-1)   /*    "   "     */
			ErrExit("errore msgctl");
		
		if(msgctl(idmsgPri2,IPC_RMID,NULL)==-1)   /*     "  "     */
			ErrExit("errore msgctl");
			
		printf(" \n CODE DI MESSAGGI ELIMINATE \n");
		
		printf(" \n ******************** L'OSPEDALE E CHIUSO ******************** \n");
				
		exit(EXIT_SUCCESS);
			   
		
      }
      
	}
