ap-paralelo-1: main.o serial-basic.o image-lib.o
    
	gcc -Wall -pedantic -std=c99 -o ap-paralelo-1 main.o serial-basic.o image-lib.o -pthread -lpthread -lgd -g

Main.o: main.c serial-basic.h image-lib.h 
    
	gcc -Wall -pedantic -std=c99 -c main.c -pthread -lpthread -lgd -g
	
serial-basic.o: serial-basic.c serial-basic.h image-lib.h

	gcc -Wall -pedantic -std=c99 -c serial-basic.c -pthread -lpthread -lgd -g

image-lib.o: image-lib.c image-lib.h
    
	gcc -Wall -pedantic -std=c99 -c image-lib.c -pthread -lpthread -lgd -g
