#include <stdlib.h>
#include <string.h>

#define SUCESSO 1
#define FRACASSO 0

typedef struct FDDE *pF, **ppF;

int criaFila(ppF fpp, int tamInfo);
void destroiFila(ppF fpp);
int buscaNaFrenteFila(pF fp, void *destino);
int buscaNaCaudaFila(pF fp, void *destino);
int insereFila(pF fp, void *novo);
int inserePrioridadeFila(pF fp, void *novo, int (*callback)(void *dados1, void *dados2));
int retiraFila(pF fp, void *novo);
int testaVaziaFila(pF fp);
int reiniciaFila(pF fp);
int numElemDeAltaPriorFila(pF fp, int (*funcAux)(void *inf1));
int tamanhoDaFila(pF fp);
int imprimeFila(pF fp, void (*imprimeNo)(void *inf1));
