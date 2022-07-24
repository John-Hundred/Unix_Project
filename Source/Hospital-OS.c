#include "FileConf.h"
#include "MyFunction.h"
#include "MyError.h"
#include "MyTypes.h"
#include "Struct.h"


int main(void) {
	
	
	struct mymsgPri priMsg;
	struct mymsgPri2 priMsg2;
	struct sigaction sa;
	struct queue msg;	   
	struct sembuf sop;	
		
    int idSem, idMsg, num_reparto, casuale, client, server,
		numTurno, idmsgPri, idmsgPri2, error, soglia, cod_gravita;
	
	int block = FALSE;		
		    
	char *strg_gravita=NULL, *str_gravita=NULL , *eptr=NULL;
	
		
	FILE *pr=NULL;		
	pid_t pid;	
	
	register int i;	
	
	
	
	if(mkfifo("1reparto",0400|0200)==-1)   /* creo fifo */
		ErrExit(" first Fifo failed");
	if(mkfifo("2reparto",0400|0200)==-1)   /* "     "	*/
		ErrExit(" second Fifo failed");
	
		
	if((idmsgPri=msgget(KEY1,IPC_CREAT|0400|0200))==-1)    /*  creo coda  */
		ErrExit("msgget failed ");	
	if((idmsgPri2=msgget(KEY2,IPC_CREAT|0400|0200))==-1)   /*  "    "	*/
		ErrExit("msgget failed ");	
	if((idMsg=msgget(KEY3,IPC_CREAT|0400|0200))==-1)      /*   "    "    */
		ErrExit("msgget failed ");
		
		
		
	if((idSem=semget(KEY4,5,IPC_CREAT|0400|0200))==-1)   /* creo un set di semafori (5) */
		ErrExit(" semGet failed ");	
		
			
			
	arg.val= ACCESSI;   
		
	if(semctl(idSem,0,SETVAL,arg)==-1)  /* inizializzo il n di accessi fissato da FIleConf.h per il  primo semaforo  */
		ErrExit("semctl failed");
		
	arg.val=1;
	
	if(semctl(idSem,1,SETVAL,arg)==-1) 
		ErrExit("semctl failed");
	
	arg.val=0;
	
	if(semctl(idSem,2,SETVAL,arg)==-1)  
		ErrExit("semctl failed");
	
	arg.val=ACCESSI;
	
	if(semctl(idSem,3,SETVAL,arg)==-1)  
		ErrExit("semctl failed");
		
	
	arg.val=0;
	
	if(semctl(idSem,4,SETVAL,arg)==-1)   
		ErrExit("semctl failed");
		
		
	
	sa.sa_handler=handler;               /* associo la funzione handler alla struttura	*/
	sa.sa_flags= SA_RESTART;       
	
	if(sigaction(SIGQUIT,&sa,NULL)==-1)   /* asssociazione handler al segnale SIGQUIT	*/
		ErrExit("sigaction error");
	
	if(sigaction(SIGALRM,&sa,NULL)==-1)    /*     "                      "      SIGALRM    */
		ErrExit("sigaction error");
		
				
	
	alarm(CLOSE);	 /*  imposta il timer fissato da FileConf.h  */
	
		
	
	
	for(i=0;i<22;++i) {       /*      GENERATORE DI PROCESSI  */
		switch(pid=fork()) {
			case -1:			
					ErrExit("Errore fork");
			case 0:
			
																				
					sop.sem_num=0;
					sop.sem_op=-1;       
					sop.sem_flg=0;
				
					error=semop(idSem,&sop,1);
				      if(error==-1)
						ErrExit("Errore semop ");
					
					
					soglia=semctl(idSem,4,GETVAL);  /* chiusura accesso  ospedale al segnale siglarm  */
					if(soglia==50) {
						printf(" \n Paziente n [%d] : Non posso  entrare al ospedale !! Me ne vado ! \n",getpid());
						exit(EXIT_SUCCESS);
					}
												
		/* ******************************************  ACCESSO AL OSPEDALE */
									
					sop.sem_num=1;
					sop.sem_op=-1;       
					sop.sem_flg=0;
					if(semop(idSem,&sop,1)==-1)   
						ErrExit(" errore semop accessi \n");   
				
				/* sezione  critica */
					scriviSintomo();
				    strg_gravita=sintomo();
				    
				/* fine sezione  */
				
				  
					sop.sem_num=1;                       
					sop.sem_op=1;       
					sop.sem_flg=0;
					if(semop(idSem,&sop,1)==-1)                 
						ErrExit(" errore semop accessi \n"); 
						
				
					msg.mytype=getppid();
					strcpy(msg.mtext,strg_gravita);
					msg.pid=getpid();      /* numero paziente   */		 		 
					printf(" \n il paziente num  %d accede al TRIAGE lamentandosi per il sintomo %s \n",getpid(),strg_gravita);
				  
				                        
					free(strg_gravita);  /* libero il puntatore restituito da sintomo  */
				  
				  
					error=msgsnd(idMsg,&msg,sizeof(msg),0);  /* invia i dati al triage tramite messaggio */
					if(errno==EINTR) {                       /* controllo errore in caso di segnale */
						errno=0;                             /* reset errno */
						printf(" msgsnd fallita per un segnale ci riprovo \n ");
						if(msgsnd(idMsg,&msg,sizeof(msg),0)<0)
							ErrExit(" msgnd fallita ");
					}
					else if(error==-1)
						ErrExit("msgnd fallita \n");
					
								
				
					sop.sem_num=2;
					sop.sem_op=-1;       
					sop.sem_flg=0;
					if(semop(idSem,&sop,1)==-1)      /* in attesa della valutazione triage */
						ErrExit("Errore semop");
				
				
					
				/* ******** ACCESSO REPARTI********************** */
				
					pr=fopen("reparto.txt","r");    /* cerco il numero del reparto sul file  */
					if(pr==NULL)
						ErrExit("Errore apertura file reparto.txt \n");
					
					strg_gravita=(char *)malloc(sizeof(char)*5);
					if(strg_gravita==NULL)
						ErrExit("Malloc fallita \n ");
					
					while(fscanf(pr,"%s",strg_gravita)!=EOF);
					fclose(pr);
												
					num_reparto=(int)strtol(strg_gravita,&eptr,10);   
					if(*eptr!='\0')
						ErrExit(" strtol fallita \n ");
					
					free(strg_gravita); /* libero str  */
				
				
	/* *************************** REPARTO 1 *********************************************** */
					if(num_reparto==1) {                    
						printf(" \n Paziente n[%d]: Sono nel primo reparto ! \n",getpid());
						
						client=open("1reparto",O_RDONLY);     
						
						strg_gravita=(char *)malloc(sizeof(char)*20);
						if(strg_gravita==NULL)
							ErrExit("malloc fallita");
							
						if(read(client,strg_gravita,20)<0)               /* leggo la gravita dal fifo  */
							ErrExit("Errore read");
						
						close(client); 
											
						cod_gravita=(int)strtol(strg_gravita,&eptr,10);     
						if(*eptr!='\0')
							ErrExit(" strtol fallita \n ");
						
						free(strg_gravita);
						printf("  Paziente n[%d]: Ho una  gravita %d ! \n",getpid(),cod_gravita);
						
											
					
					
						switch(cod_gravita) {     
												
							case 1:
								numTurno=10;
								break;
						
							case 2:
								numTurno=9;
								break;
						
							case 3:
								numTurno=8;
								break;
						
							case 4:
								numTurno=7;
								break;
						
							case 5:
								numTurno=6;
								break;
							
							case 6:
								numTurno=5;
								break;
						
							case 7:
								numTurno=4;
								break;
							
							case 8:
								numTurno=3;
								break;
							
							case 9:
								numTurno=2;
								break;
								
							case 10:
								numTurno=1;
								break;
							
							default:
								ErrExit("Gravita inesistente ");
						}
					
						
						printf(" \n REPARTO 1: il paziente n[%d] ha numero di turno %d \n",getpid(),numTurno);
						
						priMsg.mytype=(long)numTurno;                         
						strcpy(priMsg.mtext," \n GESTORE PRESTAZIONI(REP 1): Trattamento sintomo del paziente");
						priMsg.pid=getpid();			
					
					
						error=msgsnd(idmsgPri,&priMsg,sizeof(priMsg),0);     /* invio il msg alla coda di priorita */
						if(errno==EINTR) {
							errno=0;
							printf(" msgsnd fallita per un segnale ci riprovo \n ");
							if(msgsnd(idMsg,&msg,sizeof(msg),0)<0)
								ErrExit(" msgnd fallita ");
						}
						else if(error==-1)
							ErrExit("msgnd fallita \n");
							
					
						sop.sem_num=3;          /* semaforo per indicare l 'invio del messaggio  */
						sop.sem_op=-1;  
						sop.sem_flg=IPC_NOWAIT;
						error=semop(idSem,&sop,1);
						if(errno==EAGAIN)                
							errno=0;
						else if(error==-1)
							ErrExit(" semop fallita \n");								
					
					
							
						kill(getpid(),SIGSTOP);			/* il paziente aspetta il trattamento */
					
					
						printf(" \n  PAZIENTE n[%d]: esco dal ospedale \n",getpid());
					
										
			
						sop.sem_num=0;
						sop.sem_op=1;      /* libero un accesso al ospedale  */
						sop.sem_flg=0;
			
							
						error=semop(idSem,&sop,1);   
						 if(errno==EINTR) {
							errno=0;
							exit(EXIT_SUCCESS);
							
						}else if(errno==EINVAL){
							errno=0;
							exit(EXIT_SUCCESS);
						}else if(error==-1)
							ErrExit("Errore semop \n ");
					
					
						exit(EXIT_SUCCESS);
						
									
					} else {
					
	/* *********************REPARTO 2******************************************************	 */			
									
						printf(" \n Paziente n[%d]: Sono nel secondo reparto !\n",getpid());
						
						client=open("2reparto",O_RDONLY);           /*  apro FIFO */
						strg_gravita=(char *)malloc(sizeof(char)*20);    
						
						if(strg_gravita==NULL)                           /*  come da primo reparto */
							ErrExit("malloc faliita");
						
						if(read(client,strg_gravita,20)<0) 
							ErrExit("Errore read");
						
						close(client);					             /* chiudo FIFO  */
						
											
						cod_gravita=(int)strtol(strg_gravita,&eptr,10);
						if(*eptr!='\0')
							ErrExit(" strtol fallita \n ");
							           
						free(strg_gravita);
						printf("  Paziente n[%d]: Ho una  gravita %d ! \n",getpid(),cod_gravita);
						
											
							
					
						switch(cod_gravita) {   /*    calcolo num turno  */
													
							case 1:
								numTurno=10;
								break;
							
							case 2:
								numTurno=9;
								break;
							
							case 3:
								numTurno=8;
								break;
							
							case 4:
								numTurno=7;
								break;
							
							case 5:
								numTurno=6;
								break;
							
							case 6:
								numTurno=5;
								break;
							
							case 7:
								numTurno=4;
								break;
							
							case 8:
								numTurno=3;
								break;
							
							case 9:
								numTurno=2;
								break;
								
							case 10:
								numTurno=1;
								break;
							
							default:
								ErrExit("Gravita inesistente ");
						}
					
						printf(" \n REPARTO 2: il paziente n[%d] ha numero di turno %d \n",getpid(),numTurno);  
							
						priMsg2.mytype=(long)numTurno;           /* prepraro il msg */
						strcpy(priMsg2.mtext," \n GESTORE PRESTAZIONI(REP 2): Trattamento sintomo del paziente");
						priMsg2.pid=getpid();			
					
					
						error=msgsnd(idmsgPri2,&priMsg2,sizeof(priMsg2),0);     /* invio msg a coda di priorita */
						if(errno==EINTR) {
							errno=0;
							printf(" msgnd fallita per un segnale ci riprovo \n ");
							if(msgsnd(idmsgPri2,&priMsg2,sizeof(priMsg2),0)<0)
								ErrExit("msgnd fallita");
						}else if(error==-1)
								ErrExit("msgnd fallita");
					
						sop.sem_num=3;
						sop.sem_op=-1;              
						sop.sem_flg=IPC_NOWAIT;
						error=semop(idSem,&sop,1);
						if(errno==EAGAIN)                
							errno=0;
						else if(error==-1)
							ErrExit(" semop fallita \n");
									
										
						kill(getpid(),SIGSTOP);  /*     attendo il completamento della prestazione */
						
						
						printf(" \n PAZIENTE n[%d]: esco dal ospedale \n",getpid());
									
			
						sop.sem_num=0;
						sop.sem_op=1;      /* libero un accesso al ospedale  */
						sop.sem_flg=0;
			
							
						error=semop(idSem,&sop,1);   
						 if(errno==EINTR) {
							errno=0;
							exit(EXIT_SUCCESS);
						} else if(errno==EINVAL) {
							errno=0;
							exit(EXIT_SUCCESS);
						 }else if(error==-1)
							ErrExit("Errore semop  ");
							
											
					
						exit(EXIT_SUCCESS);		
					
					}
						
			   }
		} /*  FINE GENERATORE   */
		 
	
	/* ZONA PADRE */
	
	pid=fork();
		if(pid==-1) {
			ErrExit(" fork fallita ");
		
		}else if(pid==0) {		   
		   pidGestore=getpid();   
		   
		   
		   /*  GESTORE PRESTAZIONI OSPEDALIERE (pidGestore)  */
			while(TRUE) {
				
				if(!block) {
					block=TRUE;
					
					sop.sem_num=3;       /* semaforo in  attesa della coda piena(ACCESSI)  */
					sop.sem_op=0;  
					sop.sem_flg=0;
				
					error=semop(idSem,&sop,1);
					if(errno==EINTR) {
						errno=0;
						printf("semop fallita per segnale ci riprovo\n ");
						if(semop(idSem,&sop,1)==-1)
							ErrExit("Errore semop \n");
					}
					else if(error==-1)
						ErrExit(" semop fallita \n ");
				}
				
				  
				error=msgrcv(idmsgPri,&priMsg,sizeof(priMsg),-400,IPC_NOWAIT);		/* ricezione msg 1° coda di priorita	 */	 
				if(errno==EINTR) {
					errno=0;
					printf("msgrcv fallita  per segnale \n");
					if(msgrcv(idmsgPri,&priMsg,sizeof(priMsg),-400,IPC_NOWAIT)==-1)
						ErrExit("errore msgrcv \n ");
				}else if(errno==ENOMSG) {
					errno=0;
				}else if(error==-1) {
					ErrExit(" msgrcv fallita\n");
						
				}else {
					printf("%s num [%d] \n",priMsg.mtext,priMsg.pid);						
					sleep(1);   										/*  tempo dell erogazione  */
					kill(priMsg.pid,SIGCONT);                            /* ***** libero il paziente in attessa della prestazione */
				}
					
			
				error=msgrcv(idmsgPri2,&priMsg2,sizeof(priMsg2),-400,IPC_NOWAIT);    /* ricezione msg 2° coda di priorita */
				if(errno==EINTR) {													 /*  controllo errore segnale  */
					errno=0;
					printf("msgrcv fallita  per segnale \n");
					if(msgrcv(idmsgPri2,&priMsg2,sizeof(priMsg2),-400,IPC_NOWAIT)==-1)
						ErrExit("Errore msgrcv \n");       
				} else if(errno==ENOMSG) {
					errno=0;
				} else if(error==-1){
					ErrExit(" msgrcv fallita \n ");
				}else {
					printf("%s num [%d] \n",priMsg2.mtext,priMsg2.pid);	
					sleep(1);
					kill(priMsg2.pid,SIGCONT);                          /*  libero il paziente in attessa della prestazione */
			
				}
			}  /*  FINE GESTORE PRESTAZIONI  */
					
		} else { 	/* *************GESTORE TRIAGE*****************   */
		
		
		  while(TRUE) {
			
			error=msgrcv(idMsg,&msg,sizeof(msg),0,0);   /*  ricevo il sintomo dalla coda di messaggi   */
			if(errno==EINTR) {
				errno=0;
				if(msgrcv(idMsg,&msg,sizeof(msg),0,0)==-1)
					ErrExit("Errore msgrcv \n ");
			}else if(error==-1)
				ErrExit(" errore msgrcv \n ");
				
			cod_gravita=gravita(msg.mtext);		           /*  asssocio la gravita al sintomo   */
		
			printf("  TRIAGE: il paziente n %d  con sintomo %s gli viene data una gravita %d \n",msg.pid,msg.mtext,cod_gravita);
		
			srand((unsigned int)time(NULL));	/*    pesco un numero casuale di reparto   */
			casuale=1+rand()% REPARTI;
		
			pr=fopen("reparto.txt","w");		/* scrivo il numero di reparto su file verra letto dal paziente    */
			if(pr==NULL)
				ErrExit("Errore apertura file reparto.txt \n");
						
			fprintf(pr,"%d",casuale);
			fclose(pr);                        
						
			
			
			sop.sem_num=2;        /*   libero il paziente in atessa della valutazione del TRIAGE    */
			sop.sem_op=1;       
			sop.sem_flg=0;
			
			if(semop(idSem,&sop,1)==-1)   
				ErrExit("Errore semop");	
		
				
			/* X REPARTO 1		  */
			if(casuale==1) {                                        /* scrittura del codice di gravita   */        
				if((server=open("1reparto",O_WRONLY))<0)
					ErrExit("Error server");
					
				str_gravita=(char *)malloc(sizeof(char)*20);
				if(str_gravita==NULL)
					ErrExit("Malloc fallita \n ");
					
				sprintf(str_gravita,"%d",cod_gravita);
				write(server,str_gravita,20);
				free(str_gravita);
				close(server);                  
					
			}
			/*  x REPARTO 2  */
			else{                                                      
				if((server=open("2reparto",O_WRONLY))<0)
					ErrExit("Error server");
					
				str_gravita=(char *)malloc(sizeof(char)*20);
				if(str_gravita==NULL)
					ErrExit("Malloc fallita \n ");
					
				sprintf(str_gravita,"%d",cod_gravita);
				write(server,str_gravita,20);
				free(str_gravita);
				close(server);										 
			
			}	
					
		}  
	}/* FINE GESTORE TRIAGE	 */
		
	exit(EXIT_SUCCESS);
			
}   /*  fine main  */
