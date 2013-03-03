OBJECTS = fileGetTest.o fileGet.o
CC = gcc
CFLAGS = -g -Wall

test: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o testrun

fileGetTest.o: fileGetTest.c ../../fileGet.h ../../definitions.h
	$(CC) -c fileGetTest.c

fileGet.o: ../../fileGet.c ../../fileGet.h ../../definitions.h
	$(CC) -c ../../fileGet.c

clean:	
	rm -rf *.o testrun
