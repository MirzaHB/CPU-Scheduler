#include "Priority.h"
#include <stdio.h>
#include <stdlib.h>

void priorityScheduling(ProcessData dataArray[], int numProcesses)
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

    ReadyQueue readyQueue;
    initReadyQueue(&readyQueue);

    int processesCompleted = 0;
    int index = 0;

    CPUTIME = 0;

    while (processesCompleted < numProcesses)
    {
        // Add all processes that have arrived to the ready queue
        while (index < numProcesses && dataArray[index].arrivalTime <= CPUTIME)
        {
            addToReadyQueue(&readyQueue, &dataArray[index]);
            index++;
        }

        if (readyQueue.size == 0)
        {
            // No processes are ready; advance CPUTIME to the arrival time of the next process
            if (index < numProcesses)
            {
                CPUTIME = dataArray[index].arrivalTime;
                continue;
            }
        }

        ProcessData *currentProcess = getHighestPriorityJob(&readyQueue);

        int ProcessEnd = CPUTIME + currentProcess->burstLength;
        if (Tracker[currentProcess->xPid].processStart == -1)
        {
            Tracker[currentProcess->xPid].processStart = CPUTIME;
            Tracker[currentProcess->xPid].timeTillFirstResponse = currentProcess->timeUntilFirstResponse;
            Tracker[currentProcess->xPid].burstLength = currentProcess->burstLength;
            Tracker[currentProcess->xPid].waitTime = CPUTIME - currentProcess->arrivalTime;
            Tracker[currentProcess->xPid].ArrivalTime = currentProcess->arrivalTime;
            Tracker[currentProcess->xPid].endTime = ProcessEnd;
        }
        else
        {
            Tracker[currentProcess->xPid].burstLength += currentProcess->burstLength;
            Tracker[currentProcess->xPid].waitTime += CPUTIME - currentProcess->arrivalTime;

            if (Tracker[currentProcess->xPid].endTime < ProcessEnd)
            {
                Tracker[currentProcess->xPid].endTime = ProcessEnd;
            }
            if (Tracker[currentProcess->xPid].ArrivalTime > currentProcess->arrivalTime)
            {
                Tracker[currentProcess->xPid].ArrivalTime = currentProcess->arrivalTime;
            }
        }

        processesCompleted++;
        CPUTIME += currentProcess->burstLength;
    }

    // Calculate and print metrics
    for (int i = 1; i < 51; i++)
    {
        if (Tracker[i].processStart != -1) // Process was executed
        {
            Tracker[i].responseTime = Tracker[i].processStart + Tracker[i].timeTillFirstResponse - Tracker[i].ArrivalTime;
            Tracker[i].turnAroundTime = Tracker[i].endTime - Tracker[i].ArrivalTime;
            printf("Id=%d, Arrival= %d, Burst= %d, Wait= %d, Turnaround= %d, Response Time= %d, StartTime= %d, endTime= %d\n",
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