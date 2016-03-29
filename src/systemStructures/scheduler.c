#include <stdio.h>
#include "../dataStructures/filaPub.h"
#include "structs.h"

//Function designed to compare two events for the generic list.
int compareEvent(void *inf1, void *inf2){
	//Resolves the casting for pointers to pointers of events.
	EVENT *event1 = *((EVENT**) inf1);
	EVENT *event2 = *((EVENT**) inf2);
	return event1->scheduledTime > event2->scheduledTime;
}

//Function designed to print an event for the generic list.
void printEvent(void *inf1){
	//Resolves the casting for pointers to pointers of jobs.
	EVENT *event = *((EVENT**) inf1);
	printf("%d \t %d \t\n", event->id, event->scheduledTime);
}

//Function designed to print a job for the generic list.
void printJob(void *inf1){
	JOB *job = *((JOB**) inf1);
	printf("%d \t", job->id);
}

//Starts the scheduler by allocating the queues and starting the system time.
int initializeScheduler(SCHEDULER *scheduler){
	if(!criaFila(&scheduler->cpuQueue, sizeof(JOB*))) return 0;
	if(!criaFila(&scheduler->cmQueue, sizeof(JOB*))) return 0;
	if(!criaFila(&scheduler->diskQueue, sizeof(JOB*))) return 0;
	if(!criaFila(&scheduler->eventQueue, sizeof(EVENT*))) return 0;
	scheduler->totalDt = 0;
	
	return 1;
}

//Assigns the job to the Event 1
void receiveJob(SCHEDULER *scheduler, JOB *job){
	//Creates the Event
	EVENT *event = (EVENT*) malloc(sizeof(EVENT));
	event->id = 1;
	event->scheduledTime = job->arrivalTime;
	event->job = job;
	
	//Updates the next event
	job->nextEvent = event;
	
	//Inserts the new event on the Event Queue
	inserePrioridadeFila(scheduler->eventQueue, &event, compareEvent);
}
