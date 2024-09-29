#include "scheduler_common.h"
#include <stdio.h>
#include <stdlib.h>

void RoundRobin(ProcessData dataArray[], int numProcesses, int quantum)
{
    ProcessTrackerRR tracker[MAX_UNIQUE_PIDS + 1];
    for (int i = 0; i <= MAX_UNIQUE_PIDS; i++)
    {
        tracker[i].processStart = -1;
        tracker[i].ArrivalTime = 0;
        tracker[i].timeTillFirstResponse = 0;
        tracker[i].endTime = 0;
        tracker[i].waitTime = 0;
        tracker[i].responseTime = -1;
        tracker[i].burstLength = 0;
        tracker[i].turnAroundTime = 0;
    }

    ReadyQueue readyQueue;
    initReadyQueue(&readyQueue);

    int processesCompleted = 0;
    int index = 0;

    CPUTIME = 0;

    for (int i = 0; i < numProcesses; i++)
    {
        dataArray[i].timeRan = 0;
    }

    // qsort(dataArray, numProcesses, sizeof(ProcessData), compareByArrivalTime); Not sure if this is needed

    while (processesCompleted < numProcesses)
    {
        while (index < numProcesses && dataArray[index].arrivalTime <= CPUTIME)
        {
            addToReadyQueue(&readyQueue, &dataArray[index]);
            index++;
        }

        if (readyQueue.size == 0)
        {
            if (index < numProcesses)
            {
                CPUTIME = dataArray[index].arrivalTime;
                continue;
            }
        }

        ProcessData *currentProcess = removeFromReadyQueue(&readyQueue, 0);
        int pid = currentProcess->xPid;

        if (tracker[pid].processStart == -1)
        {
            tracker[pid].processStart = CPUTIME;
            tracker[pid].ArrivalTime = currentProcess->arrivalTime;
        }

        int timeRemaining = currentProcess->burstLength - currentProcess->timeRan;
        int timeToRun = timeRemaining > quantum ? quantum : timeRemaining;

        CPUTIME += timeToRun;
        currentProcess->timeRan += timeToRun;
        tracker[pid].burstLength += timeToRun;
        tracker[pid].endTime = CPUTIME;

        while (index < numProcesses && dataArray[index].arrivalTime <= CPUTIME)
        {
            addToReadyQueue(&readyQueue, &dataArray[index]);
            index++;
        }

        if (tracker[pid].responseTime == -1 && currentProcess->timeUntilFirstResponse - currentProcess->timeRan <= quantum)
        {
            tracker[pid].responseTime = CPUTIME + currentProcess->timeUntilFirstResponse;
        }

        if (currentProcess->timeRan == currentProcess->burstLength)
        {
            processesCompleted++;
            tracker[pid].waitTime += CPUTIME - currentProcess->arrivalTime - currentProcess->burstLength;
            tracker[pid].turnAroundTime = tracker[pid].endTime - tracker[pid].ArrivalTime;
            if (tracker[pid].responseTime == -1)
            {
                tracker[pid].responseTime = tracker[pid].processStart + currentProcess->timeUntilFirstResponse - tracker[pid].ArrivalTime;
            }
        }
        else
        {
            addToReadyQueue(&readyQueue, currentProcess);
        }
    }

    printf("\nRound Robin Scheduling (Quantum = %d):\n", quantum);
    for (int i = 1; i <= MAX_UNIQUE_PIDS; i++)
    {
        if (tracker[i].processStart != -1)
        {
            printf("Id=%d, Arrival=%d, Start=%d, End=%d, Burst=%d, Wait=%d, Turnaround=%d, Response=%d\n",
                   i,
                   tracker[i].ArrivalTime,
                   tracker[i].processStart,
                   tracker[i].endTime,
                   tracker[i].burstLength,
                   tracker[i].waitTime,
                   tracker[i].turnAroundTime,
                   tracker[i].responseTime);
        }
    }
}