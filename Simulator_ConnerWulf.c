
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct Process
{
	int process_id;
	int eventtype;
	int arrivalTime;
	int burstTime;
};

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
		// printf("%d %d %d %d\n",processes[line].process_id
		// 				   , processes[line].eventtype
		// 				   , processes[line].arrivalTime
		// 			   		,processes[line].burstTime);
		
	}

	sortByArrival(processes, 0, numProcesses - 1);
	
	int time = 0;

	while(true)
	{
		//check if we should add process to queue
		//check queue for a process to run
		//run process for time quantum
		//add process to ready queue if burst > 0

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
