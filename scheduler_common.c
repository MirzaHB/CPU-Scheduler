// scheduler_common.c
#include "scheduler_common.h"
#include <stdio.h>

int CPUTIME = 0;

void initReadyQueue(ReadyQueue *queue)
{
    queue->size = 0;
}

void addToReadyQueue(ReadyQueue *queue, ProcessData *process)
{
    if (queue->size < MAX_READY_QUEUE_SIZE)
    {
        queue->processes[queue->size++] = process;
    }
    else
    {
        fprintf(stderr, "Ready Queue Overflow\n");
    }
}

ProcessData *removeFromReadyQueue(ReadyQueue *queue, int index)
{
    if (queue->size == 0 || index >= queue->size)
    {
        return NULL;
    }
    ProcessData *process = queue->processes[index];
    for (int i = index; i < queue->size - 1; i++)
    {
        queue->processes[i] = queue->processes[i + 1];
    }
    queue->size--;
    return process;
}

int compareByArrivalTime(const void *a, const void *b)
{
    const ProcessData *p1 = (const ProcessData *)a;
    const ProcessData *p2 = (const ProcessData *)b;
    return p1->arrivalTime - p2->arrivalTime;
}

int compareProcesses(const void *a, const void *b)
{
    const ProcessData *p1 = (const ProcessData *)a;
    const ProcessData *p2 = (const ProcessData *)b;

    if (p1->arrivalTime != p2->arrivalTime)
        return p1->arrivalTime - p2->arrivalTime;
    else
        return p2->xPid - p1->xPid;
}

ProcessData *getShortestJob(ReadyQueue *queue)
{
    if (queue->size == 0)
    {
        return NULL;
    }

    int shortestIndex = 0;
    int shortestBurst = queue->processes[0]->burstLength;

    for (int i = 1; i < queue->size; i++)
    {
        if (queue->processes[i]->burstLength < shortestBurst)
        {
            shortestBurst = queue->processes[i]->burstLength;
            shortestIndex = i;
        }
        else if (queue->processes[i]->burstLength == shortestBurst)
        {
            if (queue->processes[i]->xPid < queue->processes[shortestIndex]->xPid)
            {
                shortestIndex = i;
            }
        }
    }

    ProcessData *shortestProcess = queue->processes[shortestIndex];

    for (int i = shortestIndex; i < queue->size - 1; i++)
    {
        queue->processes[i] = queue->processes[i + 1];
    }
    queue->size--;

    return shortestProcess;
}

ProcessData *getHighestPriorityJob(ReadyQueue *queue)
{
    if (queue->size == 0)
    {
        return NULL;
    }

    int highestIndex = 0;
    int highestPriority = queue->processes[0]->xPid;
    int earliestArrivalTime = queue->processes[0]->arrivalTime;

    for (int i = 1; i < queue->size; i++)
    {
        if (queue->processes[i]->xPid > highestPriority)
        {
            highestPriority = queue->processes[i]->xPid;
            highestIndex = i;
            earliestArrivalTime = queue->processes[i]->arrivalTime;
        }
        else if (queue->processes[i]->xPid == highestPriority)
        {
            if (queue->processes[i]->arrivalTime < earliestArrivalTime)
            {
                highestIndex = i;
                earliestArrivalTime = queue->processes[i]->arrivalTime;
            }
        }
    }

    ProcessData *highestPriorityProcess = queue->processes[highestIndex];

    // Remove the process from the ready queue
    for (int i = highestIndex; i < queue->size - 1; i++)
    {
        queue->processes[i] = queue->processes[i + 1];
    }
    queue->size--;

    return highestPriorityProcess;
}

int getShortestRemainingTimeIndex(ReadyQueue *queue)
{
    if (queue->size == 0)
    {
        return -1;
    }

    int shortestIndex = 0;
    int shortestTime = queue->processes[0]->remainingTime;

    for (int i = 1; i < queue->size; i++)
    {
        if (queue->processes[i]->remainingTime < shortestTime)
        {
            shortestTime = queue->processes[i]->remainingTime;
            shortestIndex = i;
        }
        else if (queue->processes[i]->remainingTime == shortestTime)
        {
            if (queue->processes[i]->xPid < queue->processes[shortestIndex]->xPid)
            {
                shortestIndex = i;
            }
        }
    }

    return shortestIndex;
}
void sortReadyQueueByRemainingTime(ReadyQueue *queue)
{
    // Simple selection sort for illustration; replace with qsort if preferred
    for (int i = 0; i < queue->size - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < queue->size; j++)
        {
            if (queue->processes[j]->remainingTime < queue->processes[minIndex]->remainingTime)
            {
                minIndex = j;
            }
        }
        // Swap
        if (minIndex != i)
        {
            ProcessData *temp = queue->processes[i];
            queue->processes[i] = queue->processes[minIndex];
            queue->processes[minIndex] = temp;
        }
    }
}