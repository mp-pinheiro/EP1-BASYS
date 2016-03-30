#include "../dataStructures/filaPub.h"

typedef struct Job JOB;
typedef struct Event EVENT;

struct Job{
	int id;
	int arrivalTime;
	int remainingCpu;
	int cpuBurst;
	int memorySize;
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
	int memorySize;
	int quadSize;
	int rrTimeSlice;
	int ioBurst;
	int totalDt;
	int usingCpu;
	int usingDisk;
}SCHEDULER;

int initializeScheduler(SCHEDULER *scheduler);
void receiveJob(SCHEDULER *scheduler, JOB *job);
void eventArrival(SCHEDULER *scheduler, EVENT *event);
void eventMemoryRequest(SCHEDULER *scheduler, EVENT *event);
