CC = gcc
CFLAGS = -I. -Wall -g

DEPS = fileReader.h Fcfs.h SJF.h Priority.h RoundRobin.h SRT.h scheduler_common.h

OBJ = main.o fileReader.o FCFS.o scheduler_common.o SJF.o Priority.o RoundRobin.o SRT.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

scheduler: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o scheduler

.PHONY: clean