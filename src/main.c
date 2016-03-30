#include <stdio.h>
#include "systemStructures/structs.h"

void readInput(SCHEDULER *scheduler){
	int numberOfJobs;
	int i;
	JOB *job;
	
	//Assigns the general system variables.
	scanf("%d %d %d %d %d", &scheduler->memorySize, &scheduler->quadSize, &scheduler->rrTimeSlice, &scheduler->ioBurst, &numberOfJobs);
	
	//Prints the system informations.
	printf("Memory Size\t\tQuad Size\t\tRound-Robin Time Slice\t\tDisk Write Rate\t\tNumber of Jobs\n");
	printf("%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t\t\t\t%d\t\t\t\t\t%d\n", scheduler->memorySize, scheduler->quadSize, scheduler->rrTimeSlice, scheduler->ioBurst, numberOfJobs);
	printf("\n");
	
	//Reads the jobs inside the input file.
	for(i=0; i<numberOfJobs; i++){
		job = (JOB*) malloc(sizeof(JOB));
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
	
	//System LOOP.
	while(!testaVaziaFila(scheduler.eventQueue)){
		printf("Job ID\t\tNext Event\t\t\tScheduled Time\n");
		
		//Prints the current event queue.
		printEventQueue(&scheduler);
		
		//Checks the first event.
		retiraFila(scheduler.eventQueue, &event);
		
		//Updates system time.
		scheduler.totalDt = event->scheduledTime;
		
		//Event handler.
		switch(event->id){
			//Arrival.
			case 1:
				eventArrival(&scheduler, event->job);
				break;
			//Memory Request.
			case 2:
				eventMemoryRequest(&scheduler, event->job);
				break;
			//CPU Request.
			case 3:
				eventCpuRequest(&scheduler, event->job);
				break;
			//CPU Release.
			case 4:
				eventCpuRelease(&scheduler, event->job);
				break;
			//IO Request.
			case 5:
				eventIoRequest(&scheduler, event->job);
				break;
			//IO Release.
			case 6:
				eventIoRelease(&scheduler, event->job);
				break;
			//Job Completion.
			case 7:
				eventJobCompletion(&scheduler, event->job);
				break;
			//Round-Robin Interruption.
			case 8:
				eventRrInterruption(&scheduler, event->job);
				break;
		}
		free(event);
	}
	
	return 0;
}
