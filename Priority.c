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

    int Sequence[MAX_UNIQUE_PIDS + 1];
    int SequenceIndex = 0;

    int processesCompleted = 0;
    int index = 0;

    CPUTIME = 0;

    while (processesCompleted < numProcesses)
    {
        // add all processes that have arrived to the ready queue
        while (index < numProcesses && dataArray[index].arrivalTime <= CPUTIME)
        {
            addToReadyQueue(&readyQueue, &dataArray[index]);
            index++;
        }

        if (readyQueue.size == 0)
        {
            // no processes are ready? advance CPUTIME to arrival time of the next process
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
            Sequence[SequenceIndex++] = currentProcess->xPid;
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

    printf("\n");
    printf("Sequence = [");
    for (int i = 0; i < SequenceIndex; i++)
    {
        printf("%d ", Sequence[i]);
    }
    printf("]\n");
    printf("\n");
    printf("+------+-------------+--------------+------------+-------------+--------------+-------------------+-----------------+\n");
    printf("|  Id  |   Arrival   |     Start    |     End    |    Burst    |     Wait     |     TurnAround    |     Response    |  \n");
    printf("+------+-------------+--------------+------------+-------------+--------------+-------------------+-----------------+\n");
    int totalWaitTime = 0;
    int totalTurnaroundTime = 0;
    int totalResponseTime = 0;
    for (int i = 1; i <= MAX_UNIQUE_PIDS; i++)
    {
        Tracker[i].responseTime = Tracker[i].processStart + Tracker[i].timeTillFirstResponse - Tracker[i].ArrivalTime;
        Tracker[i].turnAroundTime = Tracker[i].endTime - Tracker[i].ArrivalTime;
        if (Tracker[i].processStart != -1)
        {
            printf("|Id=%-2d | Arrival=%-3d | Start=%-6d | End=%-6d | Burst=%-5d | Wait=%-7d | Turnaround=%-6d | Response=%-6d |\n",
                   i,
                   Tracker[i].ArrivalTime,
                   Tracker[i].processStart,
                   Tracker[i].endTime,
                   Tracker[i].burstLength,
                   Tracker[i].waitTime,
                   Tracker[i].turnAroundTime,
                   Tracker[i].responseTime);

            totalWaitTime += Tracker[i].waitTime;
            totalTurnaroundTime += Tracker[i].turnAroundTime;
            totalResponseTime += Tracker[i].responseTime;
        }
    }
    printf("+------+-------------+--------------+------------+-------------+--------------+-------------------+-----------------+\n");
    printf("\nAverage waiting time: %.2f ms\n", (float)totalWaitTime / 50);
    printf("Average turnaround time: %.2f ms\n", (float)totalTurnaroundTime / 50);
    printf("Average response time: %.2f ms\n", (float)totalResponseTime / 50);
}