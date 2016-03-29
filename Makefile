CC = gcc
CFLAGS = -c -Wall
SRCPATH = src
EXEC = bin/BASYS
OBJECTS = fila.o main.o

all: link

link: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

fila.o:
	$(CC) $(CFLAGS) $(SRCPATH)/dataStructures/fila.c

main.o:
	$(CC) $(CFLAGS) $(SRCPATH)/main.c
	
clean:
	rm -rf *.o
