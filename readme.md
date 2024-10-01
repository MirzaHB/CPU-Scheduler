# CPU Scheduling Algorithms

This repository contains implementations of several CPU scheduling algorithms, including First-Come-First-Served (FCFS), Shortest Job First (SJF), Priority Scheduling, Round Robin (RR), and Shortest Remaining Time with Exponential Averaging (SRT).

## How to Run

### Build Instructions

To compile the program and create the necessary object files, simply run the following command:

```bash
make
```

To clean up all the object files, use:

```bash
make clean
```

## Running different algorithms

Once compiled, you can run any of the following CPU scheduling algorithms by using the corresponding commands below.

1.  FCFS:

    ```bash
    ./scheduler FCFS < "Assignment 1 input file.csv"
    ```

2.  Shortest Job First (SJF):

    ```bash
    ./scheduler SJF < "Assignment 1 input file.csv"
    ```

3.  Priority Scheduling

    ```bash
    ./scheduler Priority < "Assignment 1 input file.csv"
    ```

4.  Round Robin (RR) with Time Quantum:
    ```bash
    ./scheduler RR 3 < "Assignment 1 input file.csv"
    ```
5.  Shortest Remaining Time with Exponential Averaging (SRT):
    ```bash
    ./scheduler SRT 0.1 < "Assignment 1 input file.csv"
    ```
        •	The second argument (e.g., 0.1) represents the alpha value for exponential averaging. This value should range from 0 to 1 in increments of 0.1.

### Input file format

The input file is named Assignment 1 input file.csv and should follow a specific format for process data. Please ensure that this file is correctly structured before running the algorithms.

Enjoy Scheduling!!!
