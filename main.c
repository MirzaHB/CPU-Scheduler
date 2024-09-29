
#include <stdio.h>
#include <string.h>
#include "fileReader.h"
#include "scheduler.h"
#include "Priority.h"
#include "RoundRobin.h"
#include <stdlib.h>

int main()
{
    ProcessData dataArray[MAX_ROWS];
    memset(dataArray, 0, sizeof(dataArray));
    ProcessData result[MAX_ROWS];
    int numRows = csv_reader("Assignment 1 input file.csv", dataArray, MAX_ROWS);

    if (numRows < 0)
    {
        fprintf(stderr, "Failed to read CSV data\n");
        return EXIT_FAILURE;
    }

    int count = 0;

    // --------FOR FCFS--------
    // qsort(dataArray, numRows, sizeof(ProcessData), compareProcesses);
    // fcfs(dataArray, numRows);
    // qsort(dataArray, numRows, sizeof(ProcessData), compareByxPid);
    // processCombiner(dataArray, numRows, result, &count);

    // ---------FOR SJF-------
    // gcc -o scheduler main.c fileReader.c scheduler_common.c SJF.c -I.
    // qsort(dataArray, numRows, sizeof(ProcessData), compareProcesses);
    // sjf(dataArray, numRows);

    // Bottom two not needed for SJF
    //  qsort(dataArray, numRows, sizeof(ProcessData), compareByxPid);

    // ---------For Priority------
    // gcc -o scheduler main.c fileReader.c scheduler_common.c Priority.c -I.
    // qsort(dataArray, numRows, sizeof(ProcessData), compareProcesses); DONT THINK THIS IS NEEDED
    // priorityScheduling(dataArray, numRows);

    // ----------ROUND ROBIN--------
    RoundRobin(dataArray, numRows, 100);

    // processCombiner(dataArray, numRows, result, &count);

    // for (int i = 0; i < count; i++)
    // {
    //     printf("Id=%d, Arrival= %d, Burst= %d, Wait= %d, Turnaround= %d, Response Time= %d, StartTime= %d, endTime= %d\n",
    //            result[i].xPid,
    //            result[i].arrivalTime,
    //            result[i].burstLength,
    //            result[i].waitTime,
    //            result[i].turnAroundTime,
    //            result[i].responseTime,
    //            result[i].processStartTime,
    //            result[i].completionTime);
    // }

    return EXIT_SUCCESS;
}