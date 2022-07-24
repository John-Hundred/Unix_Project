#ifndef STRUCT_H
	#define STRUCT_H
	#include "FunzioniUnix.h"
	union semun{ 				
			
		int val; 
		struct semid_ds* buf;
		unsigned short* array;
		struct seminfo* __buf; 				
	};
	
	struct queue {
		
		long mytype;
		char mtext[100];  
		pid_t pid;
	};

	struct mymsgPri {   
		long mytype;
		char mtext[100];
		pid_t pid;
	};

	struct mymsgPri2{   
		
		long mytype;
		char mtext[100];
		pid_t pid;
	};


	union semun arg;
#endif


	

