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

    int processesCompleted = 0;
    int index = 0;
    CPUTIME = 0;

    // // Initialize timeRan for all processes    not needed cause of memset for dataArray in main
    // for (int i = 0; i < numProcesses; i++)
    // {
    //     dataArray[i].timeRan = 0;
    // }

    ProcessData *currentProcess = NULL;

    while (processesCompleted < numProcesses)
    {
        // Add processes that have arrived to the ready queue
        while (index < numProcesses && dataArray[index].arrivalTime <= CPUTIME)
        {
            addToReadyQueue(&readyQueue, &dataArray[index]);
            index++;
        }

        // If there is no current process, select one
        if (currentProcess == NULL)
        {
            if (readyQueue.size == 0)
            {
                // Advance time to next process arrival if no process is ready
                if (index < numProcesses)
                {
                    CPUTIME = dataArray[index].arrivalTime;
                    continue;
                }
            }
            else
            {
                // Select process with shortest estimated remaining time
                int srtIndex = getShortestEstimatedRemainingTimeIndex(&readyQueue, estimatedBurstTime);
                currentProcess = removeFromReadyQueue(&readyQueue, srtIndex);

                int pid = currentProcess->xPid;

                // Initialize tracker for the PID if not already done
                if (tracker[pid].processStart == -1)
                {
                    tracker[pid].processStart = CPUTIME;
                    tracker[pid].ArrivalTime = currentProcess->arrivalTime;
                    tracker[pid].burstLength = 0;
                    tracker[pid].waitTime = 0;
                }
            }
        }
        else
        {
            // Check for preemption
            int preempt = 0;

            // Add new arrivals to ready queue
            while (index < numProcesses && dataArray[index].arrivalTime <= CPUTIME)
            {
                addToReadyQueue(&readyQueue, &dataArray[index]);

                // Check if the new process should preempt the current process
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
                // Preempt current process
                addToReadyQueue(&readyQueue, currentProcess);
                // Select new process with shortest estimated remaining time
                int srtIndex = getShortestEstimatedRemainingTimeIndex(&readyQueue, estimatedBurstTime);
                currentProcess = removeFromReadyQueue(&readyQueue, srtIndex);

                int pid = currentProcess->xPid;

                // Initialize tracker for the PID if not already done
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
            // No process to run, advance time
            CPUTIME++;
            continue;
        }

        // Run current process for 1 unit of time
        CPUTIME++;
        currentProcess->timeRan++;

        int pid = currentProcess->xPid;

        if (tracker[pid].responseTime == -1 && currentProcess->timeRan == currentProcess->timeUntilFirstResponse)
        {
            tracker[pid].responseTime = CPUTIME - tracker[pid].ArrivalTime;
        }

        // Update tracker
        tracker[pid].burstLength++;
        tracker[pid].endTime = CPUTIME;

        // Check if process has finished
        if (currentProcess->timeRan == currentProcess->burstLength)
        {
            processesCompleted++;

            // Update estimated burst time for PID using exponential averaging
            double actualBurstTime = currentProcess->burstLength;
            double prevEstimatedTime = estimatedBurstTime[pid];
            estimatedBurstTime[pid] = alpha * actualBurstTime + (1 - alpha) * prevEstimatedTime;

            // Calculate wait time and turnaround time
            tracker[pid].waitTime += tracker[pid].endTime - currentProcess->arrivalTime - currentProcess->burstLength;
            tracker[pid].turnAroundTime = tracker[pid].endTime - tracker[pid].ArrivalTime;

            // No current process
            currentProcess = NULL;
        }
    }

    // Print results
    printf("\nPreemptive SRT Scheduling (Alpha = %.2f):\n", alpha);
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

// Helper function to get index of process with shortest estimated remaining time
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