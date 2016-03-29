CC = gcc
CFLAGS = -c -Wall
SRCPATH = src
EXEC = bin/BASYS
OBJECTS = core.o reader.o

all: link

link: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

simulador.o: entrada.o
	$(CC) $(CFLAGS) $(SRCPATH)/core.c

entrada.o:
	$(CC) $(CFLAGS) $(SRCPATH)/reader.c

clean:
	rm -rf *.o
