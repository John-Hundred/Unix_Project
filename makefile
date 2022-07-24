Hospital_OS.exe:  MyError.o  MyFunction.o Hospital-OS.o
	gcc  -o Hospital_OS.exe  Hospital-OS.o  MyFunction.o  MyError.o
Hospital-OS.o:   Hospital-OS.c  FileConf.h  MyFunction.h  MyError.h  Struct.h  MyTypes.h 
	gcc -Wall -pedantic -c Hospital-OS.c  
MyFunction.o:	MyFunction.c  FunzioniUnix.h  MyTypes.h  FileConf.h  Struct.h  MyError.h  MyFunction.h
	gcc  -Wall -pedantic -c MyFunction.c	  
MyError.o:   MyError.c  FunzioniUnix.h MyError.h
	gcc -Wall -pedantic -c MyError.c 
clean:
	rm -f *.o

 
