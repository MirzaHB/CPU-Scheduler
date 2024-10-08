// Author: Mirza Hassan Baig
#include "SRT.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void SRT(ProcessData dataArray[], int numProcesses, double alpha)
{
    // Initialize estimated burst times for each PID
    double estimatedBurstTime[MAX_UNIQUE_PIDS + 1];
    for (int i = 0; i <= MAX_UNIQUE_PIDS; i++)
    {
        estimatedBurstTime[i] = 10.0; // Initial guess is 10
    }

    ProcessTracker tracker[MAX_UNIQUE_PIDS + 1];
    memset(tracker, -1, sizeof(tracker));
    ReadyQueue readyQueue;
    initReadyQueue(&readyQueue);

    int Sequence[MAX_UNIQUE_PIDS + 1];
    int SequenceIndex = 0;

    int processesCompleted = 0;
    int index = 0;
    CPUTIME = 0;

    ProcessData *currentProcess = NULL;

    while (processesCompleted < numProcesses)
    {
        // add processes that have arrived to the ready queue
        while (index < numProcesses && dataArray[index].arrivalTime <= CPUTIME)
        {
            addToReadyQueue(&readyQueue, &dataArray[index]);
            index++;
        }

        // if there is no current process, select one
        if (currentProcess == NULL)
        {
            if (readyQueue.size == 0)
            {
                // move to next process arrival if no process is ready
                if (index < numProcesses)
                {
                    CPUTIME = dataArray[index].arrivalTime;
                    continue;
                }
            }
            else
            {
                // select the process with shortest estimated remaining time
                int srtIndex = getShortestEstimatedRemainingTimeIndex(&readyQueue, estimatedBurstTime);
                currentProcess = removeFromReadyQueue(&readyQueue, srtIndex);

                int pid = currentProcess->xPid;

                // initialize tracker for the PID if not already done
                if (tracker[pid].processStart == -1)
                {
                    tracker[pid].processStart = CPUTIME;
                    tracker[pid].ArrivalTime = currentProcess->arrivalTime;
                    tracker[pid].burstLength = 0;
                    tracker[pid].waitTime = 0;
                    Sequence[SequenceIndex++] = pid;
                }
            }
        }
        else
        {
            // preemption flag
            int preempt = 0;

            // add newly arrived processes to ready queue
            while (index < numProcesses && dataArray[index].arrivalTime <= CPUTIME)
            {
                addToReadyQueue(&readyQueue, &dataArray[index]);

                // checking if a new process should preempt the current process
                int newPid = dataArray[index].xPid;
                double newProcessRemainingTime = estimatedBurstTime[newPid];
                double currentProcessRemainingTime = estimatedBurstTime[currentProcess->xPid] - currentProcess->timeRan;

                if (newProcessRemainingTime < currentProcessRemainingTime)
                {
                    preempt = 1;
                }
                index++;
            }

            if (preempt)
            {
                // preempt current process
                addToReadyQueue(&readyQueue, currentProcess);
                // select new process with shortest estimated remaining time
                int srtIndex = getShortestEstimatedRemainingTimeIndex(&readyQueue, estimatedBurstTime);
                currentProcess = removeFromReadyQueue(&readyQueue, srtIndex);

                int pid = currentProcess->xPid;

                // initialize tracker for the PID if not already done
                if (tracker[pid].processStart == -1)
                {
                    tracker[pid].processStart = CPUTIME;
                    tracker[pid].ArrivalTime = currentProcess->arrivalTime;
                    tracker[pid].burstLength = 0;
                }
            }
        }

        if (currentProcess == NULL)
        {
            // No process to run, so we increment time
            CPUTIME++;
            continue;
        }

        // run current process for 1 unit of time
        CPUTIME++;
        currentProcess->timeRan++;

        int pid = currentProcess->xPid;

        if (tracker[pid].responseTime == -1 && currentProcess->timeRan == currentProcess->timeUntilFirstResponse)
        {
            tracker[pid].responseTime = CPUTIME - tracker[pid].ArrivalTime;
        }

        tracker[pid].burstLength++;
        tracker[pid].endTime = CPUTIME;

        // check for if process has finished
        if (currentProcess->timeRan == currentProcess->burstLength)
        {
            processesCompleted++;

            // update estimated burst time for PID using exponential averaging
            double actualBurstTime = currentProcess->burstLength;
            double prevEstimatedTime = estimatedBurstTime[pid];
            estimatedBurstTime[pid] = alpha * actualBurstTime + (1 - alpha) * prevEstimatedTime;

            // calculate wait time and turnaround time
            tracker[pid].waitTime += tracker[pid].endTime - currentProcess->arrivalTime - currentProcess->burstLength;
            tracker[pid].turnAroundTime = tracker[pid].endTime - tracker[pid].ArrivalTime;

            // set pointer to null again
            currentProcess = NULL;
        }
    }

    printf("\n");
    printf("\nPreemptive SRT Scheduling (Alpha = %.2f):\n", alpha);
    printf("Sequence = [");
    for (int i = 0; i < SequenceIndex; i++)
    {
        printf("%d ", Sequence[i]);
    }
    printf("]\n");
    printf("\n");
    printf("+------+-------------+------------+------------+-------------+--------------+-------------------+---------------+\n");
    printf("|  Id  |   Arrival   |    Start   |     End    |    Burst    |     Wait     |     TurnAround    |    Response   |  \n");
    printf("+------+-------------+------------+------------+-------------+--------------+-------------------+---------------+\n");
    int totalWaitTime = 0;
    int totalTurnaroundTime = 0;
    int totalResponseTime = 0;
    for (int i = 1; i <= MAX_UNIQUE_PIDS; i++)
    {
        if (tracker[i].processStart != -1)
        {
            printf("|Id=%-2d | Arrival=%-3d | Start=%-4d | End=%-6d | Burst=%-5d | Wait=%-7d | Turnaround=%-6d | Response=%-4d |\n",
                   i,
                   tracker[i].ArrivalTime,
                   tracker[i].processStart,
                   tracker[i].endTime,
                   tracker[i].burstLength,
                   tracker[i].waitTime,
                   tracker[i].turnAroundTime,
                   tracker[i].responseTime);

            totalWaitTime += tracker[i].waitTime;
            totalTurnaroundTime += tracker[i].turnAroundTime;
            totalResponseTime += tracker[i].responseTime;
        }
    }
    printf("+------+-------------+------------+------------+-------------+--------------+-------------------+---------------+\n");
    printf("\nAverage waiting time: %.2f ms\n", (float)totalWaitTime / 50);
    printf("Average turnaround time: %.2f ms\n", (float)totalTurnaroundTime / 50);
    printf("Average response time: %.2f ms\n", (float)totalResponseTime / 50);
}

// helper function to get index of process with shortest estimated remaining time; should probably be in sheduler_common !!!
int getShortestEstimatedRemainingTimeIndex(ReadyQueue *queue, double estimatedBurstTime[])
{
    int minIndex = -1;
    double minRemainingTime = 1e9; // Initialize with a large number

    for (int i = 0; i < queue->size; i++)
    {
        ProcessData *process = queue->processes[i];
        int pid = process->xPid;
        double remainingTime = estimatedBurstTime[pid] - process->timeRan;

        if (remainingTime < minRemainingTime)
        {
            minRemainingTime = remainingTime;
            minIndex = i;
        }
    }
    return minIndex;
}