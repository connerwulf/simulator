
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
/*
Author: Conner Wulf
References: https://codereview.stackexchange.com/questions/141238/implementing-a-generic-queue-in-c
*/

struct Process
{
	int process_id;
	int eventtype;
	int arrivalTime;
	int burstTime;
	int timeFinished;
} ;

typedef struct Node 
{
	struct Node *ptr;
	struct Process *process;
} Node;



void enqueue(Node **head, struct Process *process)
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

struct Process* dequeue(Node **head)
{
	Node *cur  = NULL;
	Node *prev = NULL;
	struct Process *temp = malloc(sizeof(struct Process));
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

void print_queue(Node *head) 
{
	Node *temp = head;
	while (temp != NULL) 
	{
		struct Process *processTemp = malloc(sizeof(struct Process));
		processTemp = temp->process; 
		printf("%d\n", processTemp->process_id);
		temp = temp->ptr;
	}
}



void sortByArrival(struct Process processes[], int first, int last);



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
		
	}

	sortByArrival(processes, 0, numProcesses - 1);
	
	int time = 0;
	int processesFinished = 0;
	int process_index = 0;
	Node *head = NULL;

	while(time < 100)
	{
		//printf("%d\n", processesFinished);
		//check if we should add process to queue
		if(time == processes[process_index].arrivalTime)
		{
			enqueue(&head, &processes[process_index]);
			printf("Time %d P%d arrives\n", time, processes[process_index].process_id);
			process_index++;
		}
		
		// if(!isEmpty(readyQueue))
		// {

		// 	//struct Process temp = pop(readyQueue);
		// 	printf("%d\n", temp.process_id);
		// 	if(temp.burstTime > quantum)
		// 	{
		// 		temp.burstTime = temp.burstTime - quantum;
		// 		time = time + quantum;
		// 		//push(readyQueue, &temp);
		// 	}
		// 	else if(temp.burstTime <= quantum)
		// 	{
		// 		temp.burstTime = 0;
		// 		time = time + temp.burstTime;
		// 		//printf("Time %d P%d finished\n", time, temp.process_id);
		// 		processesFinished++;
		// 	}


		//} 
		time++;
		
	}
	print_queue(head);
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






