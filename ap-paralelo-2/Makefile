ap-paralelo-2: main.o generate-watermark.o generate-thumbnail.o generate-resize.o image-lib.o
    
	gcc -Wall -pedantic -std=c99 -o ap-paralelo-2 main.o generate-watermark.o generate-thumbnail.o generate-resize.o image-lib.o -pthread -lpthread -lgd -g

Main.o: main.c generate-resize.c generate-thumbnail.c generate-watermark.c image-lib.h 
    
	gcc -Wall -pedantic -std=c99 -c main.c -pthread -lpthread -lgd -g

generate-resize.o: generate-resize.c image-lib.h

	gcc -Wall -pedantic -std=c99 -c generate-resize.c -pthread -lpthread -lgd -g

generate-thumbnail.o: generate-thumbnail.c image-lib.h

	gcc -Wall -pedantic -std=c99 -c generate-thumbnail.c -pthread -lpthread -lgd -g
	
generate-watermark.o: generate-watermark.c image-lib.h

	gcc -Wall -pedantic -std=c99 -c generate-watermark.c -pthread -lpthread -lgd -g

image-lib.o: image-lib.c image-lib.h
    
	gcc -Wall -pedantic -std=c99 -c image-lib.c -pthread -lpthread -lgd -g
