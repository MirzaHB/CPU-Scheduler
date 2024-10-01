// Author: Mirza Hassan Baig
#include <stdio.h>
#include <string.h>
#include "fileReader.h"
#include "Fcfs.h"
#include "Priority.h"
#include "RoundRobin.h"
#include "SJF.h"
#include "SRT.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s [FCFS|SJF|Priority|RR] [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }

    ProcessData dataArray[MAX_ROWS];
    memset(dataArray, 0, sizeof(dataArray));

    // read process data from input file
    int numRows = csv_reader(stdin, dataArray, MAX_ROWS);
    if (numRows < 0)
    {
        fprintf(stderr, "Failed to read process data\n");
        return EXIT_FAILURE;
    }

    char *algorithm = argv[1];

    if (strcmp(algorithm, "FCFS") == 0)
    {
        qsort(dataArray, numRows, sizeof(ProcessData), compareProcesses);
        fcfs(dataArray, numRows);
    }
    else if (strcmp(algorithm, "SJF") == 0)
    {
        qsort(dataArray, numRows, sizeof(ProcessData), compareProcesses);
        sjf(dataArray, numRows);
    }
    else if (strcmp(algorithm, "Priority") == 0)
    {
        priorityScheduling(dataArray, numRows);
    }
    else if (strcmp(algorithm, "RR") == 0)
    {
        if (argc < 3)
        {
            fprintf(stderr, "Usage: %s RR [quantum]\n", argv[0]);
            return EXIT_FAILURE;
        }
        int quantum = atoi(argv[2]);
        RoundRobin(dataArray, numRows, quantum);
    }
    else if (strcmp(algorithm, "SRT") == 0)
    {
        if (argc < 3)
        {
            fprintf(stderr, "Usage: %s SRT [alpha]\n", argv[0]);
            return EXIT_FAILURE;
        }
        float alpha = atof(argv[2]);
        SRT(dataArray, numRows, alpha);
    }
    else
    {
        fprintf(stderr, "Unknown scheduling algorithm: %s\n", algorithm);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}