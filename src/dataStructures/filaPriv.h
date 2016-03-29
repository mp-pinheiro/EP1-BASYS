#include "filaPub.h"

typedef struct noFDDE{
	void *dados;
	struct noFDDE *prox;
	struct noFDDE *ant;
}No, *pNo;

typedef struct FDDE{
	pNo cauda;
	pNo frente;
	int tamInfo;
}Fila, *pFila, **ppFila;
