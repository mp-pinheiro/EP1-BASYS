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
	char eventName[9][16];
}SCHEDULER;

int initializeScheduler(SCHEDULER *scheduler);
void printEventQueue(SCHEDULER *scheduler);
void receiveJob(SCHEDULER *scheduler, JOB *job);
void eventArrival(SCHEDULER *scheduler, JOB *job);
void eventMemoryRequest(SCHEDULER *scheduler, JOB *job);
void eventCpuRequest(SCHEDULER *scheduler, JOB *job);
void eventCpuRelease(SCHEDULER *scheduler, JOB *job);
void eventIoRequest(SCHEDULER *scheduler, JOB *job);
void eventIoRelease(SCHEDULER *scheduler, JOB *job);
void eventJobCompletion(SCHEDULER *scheduler, JOB *job);
void eventRrInterruption(SCHEDULER *scheduler, JOB *job);
