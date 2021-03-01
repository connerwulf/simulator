
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
/*
Author: Conner Wulf
References:
Basic Queue functionailty: https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/
*/

struct Process
{
	int process_id;
	int eventtype;
	int arrivalTime;
	int burstTime;
};

struct Queue
{
	int front;
	int rear;
	int size;
	unsigned maxSize;
	struct Process* array;
};

void sortByArrival(struct Process processes[], int first, int last);

struct Queue* createQueue(unsigned maxSize);

int isFull(struct Queue* queue);

int isEmpty(struct Queue* queue);

void push(struct Queue* queue, struct Process process);

struct Process pop(struct Queue* queue);

struct Process front(struct Queue* queue);

struct Process rear(struct Queue* queue);


/****************************************************************
*                  Main Body                                    *
****************************************************************/
int main(int argc, char *argv[])
{

	int quantum = atoi(argv[1]);
	int contSwitch = atoi(argv[2]);
	char * fileName = argv[3];
	FILE* file = fopen(fileName, "r");
	char fileLine[16];
	int numProcesses, totalTime = 0;

	if(file == NULL)
	{
		printf("Error reading in file\n");
		
	}
	while(fgets(fileLine, sizeof(fileLine), file))
	{
		numProcesses++;
	}

	struct Process processes[numProcesses];
	
	rewind(file);

	int line = 0;
	for(line = 0; line < numProcesses; line++)
	{
		fscanf(file, "%d %d %d %d",&processes[line].process_id
						   , &processes[line].eventtype
						   , &processes[line].arrivalTime
					   		, &processes[line].burstTime);
		totalTime += processes[line].burstTime;
		// printf("%d %d %d %d\n",processes[line].process_id
		// 				   , processes[line].eventtype
		// 				   , processes[line].arrivalTime
		// 			   		,processes[line].burstTime);
		
	}

	sortByArrival(processes, 0, numProcesses - 1);
	
	int time = 0;
	int processesFinished = 0;
	int process_index = 0;
	struct Queue* readyQueue = createQueue((unsigned)numProcesses);
	while(time < 100)
	{
		//check if we should add process to queue
		if(time == processes[process_index].arrivalTime)
		{
			push(readyQueue, processes[process_index]);
			printf("Time %d P%d arrives\n", time, processes[process_index].process_id);
			process_index++;
		}
		if(!isEmpty(readyQueue))
		{
			//run process for time quantum
		}
		
		
		//add process to ready queue if burst > 0
		time++;

	}
	
	exit(0);
	
}


void sortByArrival(struct Process processes[], int first, int last)
{
	int i, j, piv;
	struct Process temp;
	if(first < last)
	{
		piv = first;
		i = first;
		j = last;

		while(i<j)
		{
			while(processes[i].arrivalTime <= processes[piv].arrivalTime)
			{
				i++;
			}
			while(processes[j].arrivalTime > processes[piv].arrivalTime)
			{
				j--;
			}
			if(i<j)
			{
				temp = processes[i];
				processes[i] = processes[j];
				processes[j] = temp;
			}
		}
		temp = processes[piv];
		processes[piv] = processes[j];
		processes[j] = temp;

		sortByArrival(processes, first, j-1);
		sortByArrival(processes, j+1, last);
	}
}


struct Queue* createQueue(unsigned maxSize)
{
	struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
	queue->maxSize = maxSize;
	queue->front = 0;
	queue->size = 0;
	queue->rear = maxSize - 1;
	queue->array = (struct Process*)malloc(queue->maxSize * sizeof(struct Process));

	return queue;
}

int isFull(struct Queue* queue)
{
	return(queue->size == queue->maxSize);
}

int isEmpty(struct Queue* queue)
{
	return (queue->size == 0);
}

void push(struct Queue* queue, struct Process process)
{
	if(isFull(queue))
	{
		return;
	}
	queue->rear = (queue->rear + 1) & queue->maxSize;
	queue->array[queue->rear] = process;
	queue->size = queue->size + 1;
}

struct Process pop(struct Queue* queue)
{
	if(isEmpty(queue))
	{
		struct Process result;
		result.process_id = -1;
		return result;
	}

	struct Process item = queue->array[queue->front];
	queue->front = (queue->front + 1) % queue->maxSize;
	queue->size = queue->size - 1;
	return item;
}

struct Process front(struct Queue* queue)
{
	if(isEmpty(queue))
	{
		struct Process result;
		result.process_id = -1;
		return result;
	}
	return queue->array[queue->front];
}

struct Process rear(struct Queue* queue)
{
	if(isEmpty(queue))
	{
		struct Process result;
		result.process_id = -1;
		return result;
	}
	return queue->array[queue->rear];
}





