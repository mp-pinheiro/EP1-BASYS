#include <stdio.h>
#include "dataStructures/filaPub.h"
#include "systemStructures/structs.h"

int compareEvent(void *inf1, void *inf2){
	EVENT event1 = *((EVENT*) inf1);
	EVENT event2 = *((EVENT*) inf2);
	return event1.scheduledTime > event2.scheduledTime;
}

void printEvent(void *inf1){
	EVENT event1 = *((EVENT*) inf1);
	printf("%d ", event1.scheduledTime);
}

int main(void){
	pF queue; 
	cria(&queue, sizeof(EVENT));
	
	EVENT event;
	
	while(1){
		scanf("%d", &event.scheduledTime);
		if(event.scheduledTime==0) {
			break;
		}
		inserePrioridade(queue, &event, compareEvent);
		imprimeFila(queue, printEvent);
	}
	
	return 0;
}
