

#ifndef FILEREADER_H
#define FILEREADER_H

#include <stdio.h>

#define MAX_ROWS 1000

typedef struct
{
    int xPid;
    int arrivalTime;
    int timeUntilFirstResponse;
    int burstLength;
    int waitTime;
    int turnAroundTime;
    int responseTime;
    int processStartTime;
    int completionTime;
    int count;
    int timeRan;
} ProcessData;

int csv_reader(FILE *input, ProcessData dataArray[], int maxRows);

#endif