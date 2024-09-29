

#include <stdlib.h>
#include <string.h>

#include "fileReader.h"

int csv_reader(const char *filename, ProcessData dataArray[], int maxRows)
{

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    char line[256];
    int rowCount = 0;

    // read header
    if (fgets(line, sizeof(line), file) == NULL)
    {
        fprintf(stderr, "Error reading header line\n");
        fclose(file);
        return -1;
    }

    // read each line 1 by 1 and assign the correct values to the struct fields for each process
    while (fgets(line, sizeof(line), file))
    {
        if (rowCount >= maxRows)
        {
            fprintf(stderr, "Exceeded maximum number of rows (%d)\n", maxRows);
            break;
        }

        // remve the new lines
        line[strcspn(line, "\n")] = '\0';

        // assigning correct values to each struct field
        char *token;
        int fieldCount = 0;

        token = strtok(line, ",");
        while (token != NULL)
        {
            switch (fieldCount)
            {
            case 0:
                dataArray[rowCount].xPid = atoi(token);
                break;
            case 1:
                dataArray[rowCount].arrivalTime = atoi(token);
                break;
            case 2:
                dataArray[rowCount].timeUntilFirstResponse = atoi(token);
                break;
            case 3:
                dataArray[rowCount].burstLength = atoi(token);
                break;
            default:
                fprintf(stderr, "Too many fields in line: %s\n", line);
                break;
            }
            fieldCount++;
            token = strtok(NULL, ",");
        }

        if (fieldCount != 4)
        {
            fprintf(stderr, "Invalid number of fields (%d) in line: %s\n", fieldCount, line);
        }

        rowCount++;
    }

    fclose(file);
    return rowCount; // return the number of rows read
}