CC = gcc
CFLAGS = -c -Wall
SRCPATH = src
EXEC = bin/BASYS
OBJECTS = fila.o scheduler.o main.o

all: link

link: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

fila.o:
	$(CC) $(CFLAGS) $(SRCPATH)/dataStructures/fila.c

scheduler.o:
	$(CC) $(CFLAGS) $(SRCPATH)/systemStructures/scheduler.c
	
main.o:
	$(CC) $(CFLAGS) $(SRCPATH)/main.c
	
clean:
	erase *.o
