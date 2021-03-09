
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
/*
Author: Conner Wulf
References: https://codereview.stackexchange.com/questions/141238/implementing-a-generic-queue-in-c
*/

typedef struct Process
{
	int process_id;
	int eventtype;
	int arrivalTime;
	int burstTime;
	int timeFinished;
} Process;

typedef struct Node
{
	struct Node *ptr;
	Process *process;
} Node;

void sortByArrival(Process processes[], int first, int last);
void print_queue(Node *head);
Process* dequeue(Node **head);
void enqueue(Node **head, Process *process);
void CPU_Burst(Process *process, int* runningQuantum, int quantum);


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

	Process processes[numProcesses];

	rewind(file);

	int line = 0;
	for(line = 0; line < numProcesses; line++)
	{
		fscanf(file, "%d %d %d %d",&processes[line].process_id
						   , &processes[line].eventtype
						   , &processes[line].arrivalTime
					   		, &processes[line].burstTime);
		totalTime += processes[line].burstTime;

	}

	sortByArrival(processes, 0, numProcesses - 1);

	int time = 0;
	int processesFinished = 0;
	int process_index = 0;
	int runningQuantum = 0;
	Node *head = NULL;
	Process *temp = NULL;
	while(processesFinished < numProcesses)
	{
		//check if we should add process to queue
		if(time == processes[process_index].arrivalTime && process_index < numProcesses)
		{
			enqueue(&head, &processes[process_index]);
			printf("Time %d P%d arrives\n", time, processes[process_index].process_id);
			process_index++;
		}

		if(head != NULL || temp != NULL)
		{
			if(runningQuantum == 0)
			{
				temp = dequeue(&head);
			}
			//printf("%d %d %d %d\n", temp->process_id, temp->burstTime, quantum, runningQuantum);

			if(runningQuantum < quantum)
			{
			printf("Time %d P%d Runs\n", time, temp->process_id);
			CPU_Burst(temp, &runningQuantum, quantum);
			time++;
			//printf("%d %d %d %d\n", temp->process_id, temp->burstTime, quantum, runningQuantum);
			}
			if(temp->burstTime == 0)
			{
				processesFinished++;

				printf("Time %d P%d finished\n", time, temp->process_id);
				runningQuantum = 0;
				temp = NULL;
			}
			else if(runningQuantum == quantum)
			{
					enqueue(&head, temp);

					temp = NULL;
					runningQuantum = 0;
			}

		}
		else
		{
			time++;
		}
	}
	//print_queue(head);
	exit(0);

}

/****************************************************************
*                  Supporting Functions                                    *
****************************************************************/
void CPU_Burst(Process *process, int* runningQuantum, int quantum)
{

	if(*runningQuantum < quantum)
	{
		*runningQuantum = *runningQuantum + 1;
		process->burstTime--;
	}
}


void sortByArrival(Process processes[], int first, int last)
{
	int i, j, piv;
	Process temp;
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
void print_queue(Node *head)
{
	Node *temp = head;
	while (temp != NULL)
	{
		Process *processTemp = malloc(sizeof(Process));
		processTemp = temp->process;
		printf("%d\n", processTemp->process_id);
		temp = temp->ptr;
	}
}


Process* dequeue(Node **head)
{
	Node *cur  = NULL;
	Node *prev = NULL;
	Process *temp = malloc(sizeof(Process));
	if(*head == NULL)
	{
		temp->process_id = -1;
		return temp;
	}

	cur = *head;
	while(cur->ptr != NULL)
	{
		prev = cur;
		cur = cur->ptr;
	}
	temp = cur->process;
	free(cur);

	if(prev)
	{
		prev->ptr = NULL;
	}
	else
	{
		*head = NULL;
	}
	return temp;
}

void enqueue(Node **head, Process *process)
{
	Node *new = malloc(sizeof(Node));

	if(!new)
	{
		//create empty process withj id = -1
		return;
	}
	new->process = process;
	new->ptr = *head;
	*head = new;
}
