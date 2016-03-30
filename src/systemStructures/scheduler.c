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
	
	//Prints the job and resolves de Event ID.
	switch(event->id){
		case 1:
			printf("%d\t\t\tJob Arrival\t\t\t%d\n", event->job->id, event->scheduledTime);
			break;
		case 2:
			printf("%d\t\t\tMemory Request\t\t%d\n", event->job->id, event->scheduledTime);
			break;
		case 3:
			printf("%d\t\t\tCPU Request\t\t\t%d\n", event->job->id, event->scheduledTime);
			break;
		case 4:
			printf("%d\t\t\tCPU Release\t\t\t%d\n", event->job->id, event->scheduledTime);
			break;
		case 5:
			printf("%d\t\t\tIO Request\t\t\t%d\n", event->job->id, event->scheduledTime);
			break;
		case 6:
			printf("%d\t\t\tIO Release\t\t\t%d\n", event->job->id, event->scheduledTime);
			break;
		case 7:
			printf("%d\t\t\tJob Completion\t\t%d\n", event->job->id, event->scheduledTime);
			break;
		case 8:
			printf("%d\t\t\tR-R Interruption\t%d\n", event->job->id, event->scheduledTime);
			break;
	}
}

//Function designed to print a job for the generic list.
void printJob(void *inf1){
	JOB *job = *((JOB**) inf1);
	printf("%d ", job->id);
}

//Starts the scheduler by allocating the queues and starting the system time.
int initializeScheduler(SCHEDULER *scheduler){
	if(!criaFila(&scheduler->cpuQueue, sizeof(JOB*))) return 0;
	if(!criaFila(&scheduler->cmQueue, sizeof(JOB*))) return 0;
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

//Returns a rounded up division of two integers.
int divideRoundUpper(int n, int d){
	return ((n < 0) ^ (d < 0)) ? ((n + d/2)/d) : ((n - d/2)/d);
}

void printEventQueue(SCHEDULER *scheduler){
	imprimeFila(scheduler->eventQueue, printEvent);
}

//Receives the job.
void receiveJob(SCHEDULER *scheduler, JOB *job){
	//Schedules the Event 1: job arrival.
	scheduleEvent(scheduler, job, 1, job->arrivalTime);
}

//Assigns the job.
void eventArrival(SCHEDULER *scheduler, JOB *job){
	//Schedules the Event 2: memory request.
	scheduleEvent(scheduler, job, 2, scheduler->totalDt);
}

//Requests memory needed for the job.
void eventMemoryRequest(SCHEDULER *scheduler, JOB *job){
	//Calculates the number of quads for the job (so it can be paged).
	int memoryQuads = divideRoundUpper(job->memorySize, scheduler->quadSize);
	
	//Checks if there is avaliable memory.
	if(memoryQuads <= scheduler->memorySize){ //Memory is avaliable.
		//Updates the Central Memory.
		scheduler->memorySize -= memoryQuads;
		
		//Schedules the Event 3: cpu request.
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
				
				//Schedules the Event 4: CPU freeing.
				scheduleEvent(scheduler, job, 4, scheduler->totalDt + job->remainingCpu); //Updates the event time to simulate process running.
			}else{ //There are no IOs remaining.
				//Schedules the Event 7: job completion.
				scheduleEvent(scheduler, job, 7, scheduler->totalDt + job->remainingCpu); //Updates the event time to simulate process running.
			}
			job->remainingCpu = job->cpuBurst;
		}else{ //Round-Robin.
			//Updates the job time to simulate process running.
			job->remainingCpu -= scheduler->rrTimeSlice;
			
			//Schedules the Event 8: round robin swap.
			scheduleEvent(scheduler, job, 8, scheduler->totalDt + scheduler->rrTimeSlice); //Updates the event time to simulate process running.
		}
	}else{ //CPU is 'busy'.
		job->nextEvent = NULL;
		//Job is inserted to the cpu queue.
		insereFila(scheduler->cpuQueue, &job);
	}
}

void eventCpuRelease(SCHEDULER *scheduler, JOB *job){
	//Updates the CPU to 'free'.
	scheduler->usingCpu = 0;
	
	JOB *test;
	//Checks if there are Jobs on the CPU queue.
	if(retiraFila(scheduler->cpuQueue, &test)){ //Queue was not empty.
		//Schedules the removed Job to Event 3: CPU request.
		scheduleEvent(scheduler, test, 3, scheduler->totalDt); //An overtime can be assigned to simulate the time that the opearation takes.
	}
	
	//Schedules the IO requester to Event 5: IO request.
	scheduleEvent(scheduler, job, 5, scheduler->totalDt);
}

void eventIoRequest(SCHEDULER *scheduler, JOB *job){
	//Checks if the DISK is avaliable.
	if(!scheduler->usingDisk){ //DISK is avaliable.
		//Updates the DISK to 'busy'.
		scheduler->usingDisk = 1;
		
		//Schedules the Event 6: IO release.
		scheduleEvent(scheduler, job, 6, scheduler->totalDt + scheduler->ioBurst); //Updates the event time to simulate DISK writing.
	}else{ //DISK is 'busy'.
		job->nextEvent = NULL;
		//Job is inserted to the cpu queue.
		insereFila(scheduler->diskQueue, &job);
	}
}

void eventIoRelease(SCHEDULER *scheduler, JOB *job){
	//Updates the DISK to 'free'.
	scheduler->usingDisk = 0;
	
	JOB *test;
	//Checks if there are Jobs on the DISK queue.
	if(retiraFila(scheduler->diskQueue, &test)){ //Queue was not empty.
		//Schedules the removed Job to Event 5: IO request.
		scheduleEvent(scheduler, test, 5, scheduler->totalDt); //An overtime can be assigned to simulate the time that the opearation takes.
	}
	
	//Schedules the releasing job to Event 3: CPU request.
	scheduleEvent(scheduler, job, 3, scheduler->totalDt);
}

void eventJobCompletion(SCHEDULER *scheduler, JOB *job){
	//Updates the CPU to 'free'.
	scheduler->usingCpu = 0;
	
	JOB *test;
	//Checks if there are Jobs on the CPU queue.
	if(retiraFila(scheduler->cpuQueue, &test)){ //Queue was not empty.
		//Schedules the removed Job to Event 3: CPU request.
		scheduleEvent(scheduler, test, 3, scheduler->totalDt); //An overtime can be assigned to simulate the time that the opearation takes.
	}
	
	//Calculates the number of quads taken from the job.
	int memoryQuads = divideRoundUpper(job->memorySize, scheduler->quadSize);
	
	//Frees the memory.
	scheduler->memorySize += memoryQuads;
	
	//Creates a virtual memory so it can be controled without messing with the physical memory.
	//int virtualMemory = scheduler->memorySize;
	
	int length = tamanhoDaFila(scheduler->cmQueue);
	int i;
	for(i=0; i<length; i++){ //Jobs are searched for so the memory can be filled.
		//Checks if there are jobs that can fit inside the memory.
		retiraFila(scheduler->cmQueue, &test);
		
		/*
		memoryQuads = divideRoundUpper(test->memorySize, scheduler->quadSize);
		if(memoryQuads <= virtualMemory){ //This job that can be allocated (there is enough memory space).
			//Schedules the removed Job to Event 2: CPU memory request.
			scheduleEvent(scheduler, test, 2, scheduler->totalDt); //An overtime can be assigned to simulate memory operations.
			virtualMemory -= memoryQuads;
		}else{ //This job can not be allocated (there is not enough memory space).
			//The job is returned to the queue.
			insereFila(scheduler->cmQueue, &test);
		}
		*/
		
		//Schedules the removed Job to Event 2: CPU memory request.
		scheduleEvent(scheduler, test, 2, scheduler->totalDt); //An overtime can be assigned to simulate memory operations.
	}
	
	/*
	printf("Job ID: %d\n", job->id);
	imprimeFila(scheduler->cpuQueue, printJob);
	imprimeFila(scheduler->diskQueue, printJob);
	imprimeFila(scheduler->cmQueue, printJob);
	*/
	
	//Releases the job from memory.
	free(job);
}

void eventRrInterruption(SCHEDULER *scheduler, JOB *job){
	//Updates the CPU to 'free'.
	scheduler->usingCpu = 0;
	
	JOB *test;
	//Checks if there are Jobs on the CPU queue.
	if(retiraFila(scheduler->cpuQueue, &test)){ //Queue was not empty.
		//Schedules the removed Job to Event 3: CPU request.
		scheduleEvent(scheduler, test, 3, scheduler->totalDt); //An overtime can be assigned to simulate the time that the opearation takes.
	}
	
	//Schedules the interrupted Job to Event 3: CPU request.
	scheduleEvent(scheduler, job, 3, scheduler->totalDt);
}
