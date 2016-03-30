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
	if(!criaFila(&scheduler->memorySize, sizeof(JOB*))) return 0;
	if(!criaFila(&scheduler->diskQueue, sizeof(JOB*))) return 0;
	if(!criaFila(&scheduler->eventQueue, sizeof(EVENT*))) return 0;
	scheduler->totalDt = 0;
	scheduler->usingCpu = 0;
	scheduler->usingDisk = 0;
	
	return 1;
}

//Schedules the event to happen next.
void scheduleEvent(SCHEDULER *scheduler, JOB *job, int id, int time){
	EVENT *event = (EVENT*) malloc(sizeof(EVENT));
	event->id = id;
	event->scheduledTime = time;
	event->job = job;
	
	//Updates the next event.
	job->nextEvent = event;
	
	//Inserts the new event on the Event Queue.
	inserePrioridadeFila(scheduler->eventQueue, &event, compareEvent);
}

//Assigns the job.
void receiveJob(SCHEDULER *scheduler, JOB *job){
	//Creates the Event 1: job arrival.
	scheduleEvent(scheduler, job, 1, job->arrivalTime);
}

void eventArrival(SCHEDULER *scheduler, JOB *job){
	//Creates the Event 2: memory request.
	scheduleEvent(scheduler, job, 2, scheduler->totalDt);
}

int divideRoundUpper(int n, int d){
	return ((n < 0) ^ (d < 0)) ? ((n + d/2)/d) : ((n - d/2)/d);
}

//Requests memory needed for the job.
void eventMemoryRequest(SCHEDULER *scheduler, JOB *job){
	//Calculates the number of quads for the job (so it can be paged).
	int memoryQuads = divideRoundUpper(job->memorySize, scheduler->quadSize);
	
	//Checks if there is avaliable memory.
	if(memoryQuads <= scheduler.memorySize){ //Memory is avaliable.
		//Updates the Central Memory.
		scheduler.memorySize -= memoryQuads;
		
		//Creates the Event 3: cpu request.
		scheduleEvent(scheduler, job, 3, scheduler->totalDt); //Here it would be realistic to add a delay to the memory allocation.
	}else{ //No memory is avaliable.
		job->nextEvent = NULL;
		//Job is inserted to the memory queue.
		insereFila(scheduler->cmQueue, &job);
	}
}

void eventCpuRequest(SCHEDULER *scheduler, JOB *job){
	//Checks if the CPU is avaliable.
	if(!scheduler->usingCpu){ //CPU is avaliable.
		//Updates the CPU to 'busy'.
		scheduler->usingCpu = 1;
		
		//Checks the processing method.
		if(!scheduler->rrTimeSlice || job->remainingCpu <= scheduler->rrTimeSlice){ //FCFS
			//Checks the remaining IOs.
			if(job->ioRequests>0){ //There are IOs remaining.
				//Decrements the IO requests.
				job->ioRequests--;
				
				//Creates the Event 4: CPU freeing.
				scheduleEvent(scheduler, job, 4, scheduler->totalDt + job->remainingCpu); //Updates the event time to simulate process running.
			}else{ //There are no IOs remaining.
				//Creates the Event 7: job completion.
				scheduleEvent(scheduler, job, 7, scheduler->totalDt + job->remainingCpu); //Updates the event time to simulate process running.
			}
		}else{ //Round-Robin.
			//Updates the job time to simulate process running.
			job->remainingCpu -= scheduler->rrTimeSlice;
			
			//Creates the Event 8: round robin swap.
			scheduleEvent(scheduler, job, 8, scheduler->totalDt + scheduler->rrTimeSlice); //Updates the event time to simulate process running.
		}
	}else{ //CPU is 'busy'.
		job->nextEvent = NULL;
		//Job is inserted to the cpu queue.
		insereFila(scheduler->cpuQueue, &job);
	}
}
