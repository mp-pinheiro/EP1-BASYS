#include <stdio.h>
#include "systemStructures/structs.h"

void readInput(SCHEDULER *scheduler){
	int numberOfJobs;
	int i;
	JOB *job = (JOB*) malloc(sizeof(JOB));
	
	//Assigns the general system variables.
	scanf("%d %d %d %d %d", &scheduler->memorySize, &scheduler->quadSize, &scheduler->rrTimeSlice, &scheduler->ioBurst, &numberOfJobs);
	
	//Reads the jobs inside the input file.
	for(i=0; i<numberOfJobs; i++){
		scanf("%d %d %d %d %d ", &job->id, &job->arrivalTime, &job->cpuBurst, &job->memorySize, &job->ioRequests);
		job->remainingCpu = job->cpuBurst;
		receiveJob(scheduler, job);
	}
}

int main(void){
	//Initializes the scheduler and reads the input.
	SCHEDULER scheduler;
	initializeScheduler(&scheduler);
	readInput(&scheduler);
	EVENT *event;
	
	//System LOOP
	while(!testaVaziaFila(scheduler.eventQueue){
		//Checks the first event.
		retiraFila(scheduler.eventQueue, &event);
		
		//Updates system time.
		scheduler.totalDt = event->scheduledTime;
		
		//Event handler.
		switch(event->id){
			//Arrival
			case 1:
				eventArrival(&scheduler, event->job);
				break;
			//Memory Request
			case 2:
				eventMemoryRequest(&scheduler, event->job);
				break;
			case 3:
				
				break;
		}
		free(event);
	}
	
	return 0;
}
