#include "Fcfs.h"
#include <stdio.h>
#include <stdlib.h>

void fcfs(ProcessData dataArray[], int numProcesses)
{
    ProcessTracker Tracker[51];
    for (int i = 0; i < 51; i++)
    {
        Tracker[i].processStart = -1;
        Tracker[i].endTime = 0;
        Tracker[i].responseTime = 0;
        Tracker[i].ArrivalTime = 0;
        Tracker[i].waitTime = 0;
        Tracker[i].burstLength = 0;
    }

    qsort(dataArray, numProcesses, sizeof(ProcessData), compareProcesses);
    ReadyQueue readyQueue;
    initReadyQueue(&readyQueue);

    int processesCompleted = 0;
    int index = 0;

    CPUTIME = 0;

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
                // if no process is in the ready queue move to time of next process arrival
                CPUTIME = dataArray[index].arrivalTime;
                continue;
            }
        }

        ProcessData *currentProcess = removeFromReadyQueue(&readyQueue, 0);

        if (Tracker[currentProcess->xPid].processStart == -1)
        {
            Tracker[currentProcess->xPid].processStart = CPUTIME;
            Tracker[currentProcess->xPid].ArrivalTime = currentProcess->arrivalTime;
            Tracker[currentProcess->xPid].timeTillFirstResponse = currentProcess->timeUntilFirstResponse;
        }

        Tracker[currentProcess->xPid].waitTime += CPUTIME - currentProcess->arrivalTime;
        Tracker[currentProcess->xPid].burstLength += currentProcess->burstLength;
        CPUTIME += currentProcess->burstLength;
        Tracker[currentProcess->xPid].endTime = CPUTIME;
        processesCompleted++;
    }

    for (int i = 1; i < 51; i++)
    {
        if (Tracker[i].processStart != -1)
        {
            Tracker[i].responseTime = Tracker[i].processStart + Tracker[i].timeTillFirstResponse - Tracker[i].ArrivalTime;
            Tracker[i].turnAroundTime = Tracker[i].endTime - Tracker[i].ArrivalTime;
            printf("Id=%d, Arrival=%d, Burst=%d, Wait=%d, Turnaround=%d, Response Time=%d, StartTime=%d, EndTime=%d\n",
                   i,
                   Tracker[i].ArrivalTime,
                   Tracker[i].burstLength,
                   Tracker[i].waitTime,
                   Tracker[i].turnAroundTime,
                   Tracker[i].responseTime,
                   Tracker[i].processStart,
                   Tracker[i].endTime);
        }
    }
}