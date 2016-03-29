#include "../dataStructures/filaPub.h"

typedef struct Job JOB;
typedef struct Event EVENT;

struct Job{
	int id;
	int arrivalTime;
	int cpuBurst;
	int cmSize;
	int ioRequests;
	EVENT *nextEvent;
};

struct Event{
	int id;
	int scheduledTime;
	JOB *job;
};

typedef struct{
	pF cpuQueue;
	pF cmQueue;
	pF diskQueue;
	pF eventQueue;
	int cmSize;
	int quadSize;
	int rrTimeSlice;
	int ioBurst;
	int totalDt;
}SCHEDULER;

int initializeScheduler(SCHEDULER *scheduler);
void receiveJob(SCHEDULER *scheduler, JOB *job);
