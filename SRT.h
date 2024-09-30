#ifndef SRT_H
#define SRT_H

#include "scheduler_common.h"

void SRT(ProcessData dataArray[], int numProcesses, double alpha);
int getShortestEstimatedRemainingTimeIndex(ReadyQueue *queue, double estimatedBurstTime[]);

#endif