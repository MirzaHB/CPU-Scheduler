CC = gcc
CFLAGS = -I. -Wall -g

DEPS = fileReader.h Fcfs.h

OBJ = main.o fileReader.o FCFS.o scheduler_common.o SJF.o Priority.o RoundRobin.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

scheduler: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o scheduler

.PHONY: clean