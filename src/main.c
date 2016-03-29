#include <stdio.h>
#include "systemStructures/structs.h"

void readInput(SCHEDULER *scheduler){
	int numberOfJobs;
	int i;
	JOB *job = (JOB*) malloc(sizeof(JOB));
	
	//Assigns the general system variables.
	scanf("%d %d %d %d %d", &scheduler->cmSize, &scheduler->quadSize, &scheduler->rrTimeSlice, &scheduler->ioBurst, &numberOfJobs);
	
	//Reads the jobs inside the input file.
	for(i=0; i<numberOfJobs; i++){
		scanf("%d %d %d %d %d ", &job->id, &job->arrivalTime, &job->cpuBurst, &job->cmSize, &job->ioRequests);
		receiveJob(scheduler, job);
	}
}

int main(void){
	SCHEDULER scheduler;
	initializeScheduler(&scheduler);
	readInput(&scheduler);
	return 0;
}
