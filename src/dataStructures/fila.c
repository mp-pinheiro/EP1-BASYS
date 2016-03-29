#include <stdio.h>
#include "filaPriv.h"

int criaFila(ppFila fpp, int tamInfo){
	if((*fpp = (pFila) malloc(sizeof(Fila)))==NULL) return FRACASSO;
	(*fpp)->tamInfo = tamInfo;
	(*fpp)->cauda = NULL;
	(*fpp)->frente = NULL;
	return SUCESSO;
}

void destroiNoFila(pNo *aux){
	if(*aux != NULL){
		destroiNoFila(&(*aux)->prox);
		free((*aux)->dados);
		free(*aux);
	}
}

void destroiFila(ppFila fpp){
	destroiNoFila(&(*fpp)->frente);
	free(*fpp);
}

int buscaNaFrenteFila(pFila fp, void *destino){
	if(!testaVaziaFila(fp)){
		memcpy(destino, fp->frente->dados, fp->tamInfo);
		return SUCESSO;
	}
	return FRACASSO;
}

int buscaNaCaudaFila(pFila fp, void *destino){
	if(!testaVaziaFila(fp)){
		memcpy(destino, fp->cauda->dados, fp->tamInfo);
		return SUCESSO;
	}
	return FRACASSO;
}

int inserePrioridadeFila(pFila fp, void *novo, int (*callback)(void *dados1, void *dados2)){
	pNo *aux, temp1, iterador, registro;
	if((registro = (pNo) malloc(sizeof(No)))==NULL) return FRACASSO;
	if((registro->dados = (void*) malloc(fp->tamInfo))==NULL) return FRACASSO;
	iterador = fp->frente;
	
	while(iterador && callback(novo, iterador->dados)){
		iterador = iterador->prox;
	}
	
	if(iterador){
		if(iterador->ant){
			temp1 = iterador->ant;
			aux = &iterador->ant->prox;
			*aux = registro;
			(*aux)->prox = iterador;
			iterador->ant = *aux;
			(*aux)->ant = temp1;
			temp1->prox = *aux;
			memcpy((*aux)->dados, novo, fp->tamInfo);
		}else{
			aux = &fp->frente;
			*aux = registro;
			(*aux)->prox = iterador;
			iterador->ant = *aux;
			(*aux)->ant = NULL;
			memcpy((*aux)->dados, novo, fp->tamInfo);
		}
	}else{
		if(fp->frente==NULL){ //Fila vazia
			aux = &fp->frente;
			*aux = registro;
			fp->cauda = *aux;
			(*aux)->prox = NULL;
			(*aux)->ant = NULL;
			memcpy((*aux)->dados, novo, fp->tamInfo);
		}else{ //Fila com pelo menos um elemento
			temp1 = fp->cauda;
			aux = &fp->cauda->prox;
			*aux = registro;
			fp->cauda = *aux;
			(*aux)->prox = NULL;
			(*aux)->ant = temp1;
			temp1->prox = *aux;
			memcpy((*aux)->dados, novo, fp->tamInfo);
		}
	}
	
	return SUCESSO;
}

int insere(pFila fp, void *novo){
	pNo *aux, temp1;
	if(fp->frente==NULL){ //Fila vazia
		if((fp->frente = (pNo) malloc(sizeof(No)))==NULL) return FRACASSO;
		fp->cauda = fp->frente;
		aux = &fp->frente;
		if(((*aux)->dados = (void*) malloc(fp->tamInfo))==NULL) return FRACASSO;
		(*aux)->prox = NULL;
		(*aux)->ant = NULL;
		memcpy((*aux)->dados, novo, fp->tamInfo);
	}else{ //Fila com pelo menos um elemento
		temp1 = fp->cauda;
		aux = &fp->cauda->prox;
		if(((*aux) = (pNo) malloc(sizeof(No)))==NULL) return FRACASSO;
		fp->cauda = *aux;
		if(((*aux)->dados = (void*) malloc(fp->tamInfo))==NULL) return FRACASSO;
		(*aux)->prox = NULL;
		(*aux)->ant = temp1;
		temp1->prox = *aux;
		memcpy((*aux)->dados, novo, fp->tamInfo);
	}
	return SUCESSO;
}

int retiraFila(pFila fp, void *novo){
	pNo aux;
	if(!testaVaziaFila(fp)){
		aux = fp->frente;
		memcpy(novo, aux->dados, fp->tamInfo);
		fp->frente = fp->frente->prox;
		if(fp->frente!=NULL) fp->frente->ant = NULL;
		free(aux->dados);
		free(aux);
		return SUCESSO;
	}
	return FRACASSO;
}

int testaVaziaFila(pFila fp){
	if(fp->frente==NULL) return SUCESSO;
	return FRACASSO;
}

int reiniciaFila(pFila fp){
	destroiNoFila(&fp->frente);
	fp->frente = NULL;
	fp->cauda = NULL;
	return SUCESSO;
}

int numElemDeAltaPriorFila(pFila fp, int (*funcAux)(void *inf1)){
	pNo aux;
	int cont=0;
	aux = fp->frente;
	while(aux!=NULL){
		if(funcAux(aux->dados)) cont++;
		aux = aux->prox;
	}
	return cont;
}

int tamanhoDaFila(pFila fp){
	pNo aux;
	int tam=0;
	aux = fp->frente;
	while(aux!=NULL){
		tam++;
		aux = aux->prox;
	}
	return tam;
}

int imprimeFila(pFila fp, void (*imprimeNo)(void *inf1)){
	pNo aux;
	aux = fp->frente;
	while(aux!=NULL){
		imprimeNo(aux->dados);
		aux = aux->prox;
	}
	printf("\n");
	return SUCESSO;
}
