#include <stdlib.h>
#include <string.h>

#define SUCESSO 1
#define FRACASSO 0

typedef struct FDDE *pF, **ppF;

int cria(ppF fpp, int tamInfo);
void destroi(ppF fpp);
int buscaNaFrente(pF fp, void *destino);
int buscaNaCauda(pF fp, void *destino);
int insere(pF fp, void *novo);
int inserePrioridade(pF fp, void *novo, int (*callback)(void *dados1, void *dados2));
int retira(pF fp, void *novo);
int testaVazia(pF fp);
int reinicia(pF fp);
int numElemDeAltaPrior(pF fp, int (*funcAux)(void *inf1));
int tamanhoDaFila(pF fp);
int imprimeFila(pF fp, void (*imprimeNo)(void *inf1));
