#ifndef SCHEDULER_COMMON_H
#define SCHEDULER_COMMON_H

#include "fileReader.h"

#define MAX_READY_QUEUE_SIZE MAX_ROWS
#define MAX_UNIQUE_PIDS 50

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

typedef struct
{
    int processStart;
    int ArrivalTime;
    int endTime;
    int waitTime;
    int responseTime;
    int burstLength;
    int turnAroundTime;
    int timeTillFirstResponse;
    int totalTimeRan;
} ProcessTrackerRR;

// Queue operations
void initReadyQueue(ReadyQueue *queue);
void addToReadyQueue(ReadyQueue *queue, ProcessData *process);
ProcessData *removeFromReadyQueue(ReadyQueue *queue, int index);

// Utility functions
int compareByArrivalTime(const void *a, const void *b);
int compareProcesses(const void *a, const void *b);
ProcessData *getHighestPriorityJob(ReadyQueue *queue);
ProcessData *getShortestJob(ReadyQueue *queue);

extern int CPUTIME;

#endif