
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
/*
Author: Conner Wulf
*/

//Struct to hold all information required by process
typedef struct Process
{
	int process_id;
	int eventtype;
	int arrivalTime;
	int burstTime;
	int burstTimeCalc;
	int timeFinished;
} Process;

//Base node for custom Queue
typedef struct Node
{
	struct Node *ptr;
	Process *process;
} Node;


//Function definitions
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
	//Initialize variables and read in command line args
	int quantum = atoi(argv[1]);
	int contSwitch = atoi(argv[2]);
	char * fileName = argv[3];

	//Open file to read
	FILE* file = fopen(fileName, "r");
	char fileLine[16];
	int numProcesses, totalTime = 0;
	if(file == NULL)
	{
		printf("Error reading in file\n");
		exit(0);
	}

	//Traverse file to calculate the number of Processes from input file
	while(fgets(fileLine, sizeof(fileLine), file))
	{
		numProcesses++;
	}

	//Array of struct Process to hold all data from input file
	Process processes[numProcesses];

	//Rewind file to reread in order to extracted information
	rewind(file);

	//Read in processes from input file
	int line = 0;
	for(line = 0; line < numProcesses; line++)
	{
		fscanf(file, "%d %d %d %d",&processes[line].process_id
						   , &processes[line].eventtype
						   , &processes[line].arrivalTime
					   		, &processes[line].burstTime);
		processes[line].burstTimeCalc = processes[line].burstTime;


	}
	//Quick Sort algorithm sort processes by the time they arrive.
	sortByArrival(processes, 0, numProcesses - 1);


/*** ROUND ROBIN ALOGRITHM STARTS*****************************************/
	int time = 0;
	int processesFinished = 0;
	int process_index = 0;
	int runningQuantum = 0;
	int printNew = 0;
	Node *head = NULL;
	Process *temp = NULL;

	//Loop until all processes are finished
	while(processesFinished < numProcesses)
	{

		//If a process exists in the readyQueue or If one is already loaded
		if(head != NULL || temp != NULL)
		{
				//Keep track of time slice
				if(runningQuantum == 0 )
				{
					//ContextSwitch
					temp = dequeue(&head);
				}
				//printf("%d %d %d %d\n", temp->process_id, temp->burstTime, quantum, runningQuantum);
				//Checks if timeSlice expired before cpu burst
				if(runningQuantum < quantum)
				{
					//Conditional to check if new time Slice started
					if(runningQuantum == 0 || printNew == 1)
					{
						printf("P%d Runs at Time %d \n", temp->process_id, time);
						printNew = 0;
					}

					//Check for new process before running process --Context Switch
					if(time == processes[process_index].arrivalTime && process_index < numProcesses)
					{
						enqueue(&head, &processes[process_index]);
						printf("P%d arrives at Time %d \n", processes[process_index].process_id, time);
						time = time + contSwitch;
						printNew = 1;
						process_index++;
						printf("P%d Runs at Time %d \n", temp->process_id, time);
					}

					//One CPU_Burst cycle and increment time
					CPU_Burst(temp, &runningQuantum, quantum);
					time++;
			}


			//Checks if process finished after cpu burst
			if(temp->burstTime == 0)
			{
				processesFinished++;
				printf("P%d finished at Time %d \n", temp->process_id, time);
				temp->timeFinished = time;
				runningQuantum = 0;
				temp = NULL;

				//Context Switch If items in ReadyQueue
				if(head != NULL)
				{
					time = time + contSwitch;
				}
			}

			//Checks if time slice expired after cpu burst
			else if(runningQuantum == quantum)
			{
				//Context Switch
					enqueue(&head, temp);
					time = time + contSwitch;
					temp = NULL;
					runningQuantum = 0;
			}
		}
		//If nothing in temp or in queue then check for arrival and increment time
		else
		{
			//Context Switch if there is an arrival
			if(time == processes[process_index].arrivalTime && process_index < numProcesses)
			{

				enqueue(&head, &processes[process_index]);
				printf("P%d arrives at Time %d \n", processes[process_index].process_id, time);
				time = time + contSwitch;
				printNew = 1;
				process_index++;
			}
			else
			{
				time++;
			}
		}
	}

	//Calculations
	totalTime = time + contSwitch;
	int totalWait = 0; //Time finished - Time arrived - burstTime - contSwitch
	int totalBurst = 0;
	int turnaround = 0;
	for(int g = 0; g < numProcesses; g++)
	{
		totalWait = totalWait + (processes[g].timeFinished - processes[g].arrivalTime - processes[g].burstTimeCalc);
		turnaround = turnaround + (processes[g].timeFinished - processes[g].arrivalTime);
		totalBurst = totalBurst + processes[g].burstTimeCalc;

	}

	double averageWait = (double)totalWait / numProcesses;
	double avgTurnaround = (double)turnaround / numProcesses;
	double CPU_Utilization = (double)totalBurst / totalTime;

	printf("\n\nCPU Utilization: %f%\n", CPU_Utilization * 100);
	printf("Average Wait: %f\n", averageWait);
	printf("Average Turn Around Time: %f\n", avgTurnaround);
	exit(0);

}

/****************************************************************
*                  Supporting Functions definitions                                    *
****************************************************************/
//Subtracts one from process burst time
void CPU_Burst(Process *process, int* runningQuantum, int quantum)
{

	if(*runningQuantum < quantum)
	{
		*runningQuantum = *runningQuantum + 1;
		process->burstTime--;
	}
}

//Quicksort for Process array based on arrival time
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

//Debug print for custom queue
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
