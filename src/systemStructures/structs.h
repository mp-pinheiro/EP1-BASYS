typedef struct{
	int id;
	int recordCount;
}JOB;

typedef struct{
	int id;
	int scheduledTime;
	JOB *job;
}EVENT;
