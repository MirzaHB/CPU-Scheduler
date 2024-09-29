#ifndef SJF_H
#define SJF_H

#include "fileReader.h"

#define MAX_READY_QUEUE_SIZE MAX_ROWS

typedef struct
{
    ProcessData *processes[MAX_READY_QUEUE_SIZE];
    int size;
} ReadyQueue;

typedef struct
{
    int processStart;
    int ArrivalTime;
    int timeTillFirstResponse;
    int endTime;
    int waitTime;
    int responseTime;
    int burstLength;
    int turnAroundTime;
} ProcessTracker;

// Function declarations
void initReadyQueue(ReadyQueue *queue);
void addToReadyQueue(ReadyQueue *queue, ProcessData *process);
ProcessData *getShortestJob(ReadyQueue *queue);
void sjf(ProcessData dataArray[], int numProcesses);

#endif