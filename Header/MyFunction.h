#ifndef MY_FUNCTION_H
	#define  MY_FUNCTION_H
	void  handler(int sig);   /* cambia la disposizione dei segnali da assegnare a  sa_handler */
	
	char * sintomo(void);    /* resituisce un puntatore contenente il  sintomo letto da file   * E'  necessario fare una FREE *   */
	
	void scriviSintomo(void); /* Scrive un sintomo casuale su file */
	
	int gravita(char *s);  /* prende in input la stringa contenente il sintomo  e  ne restituisce la gravita   */
#endif
