#include "FunzioniUnix.h"
#include "MyError.h"

void ErrExit(char * s) {
	perror(s);
	exit(EXIT_FAILURE);
}
