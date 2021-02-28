
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



/****************************************************************
*                  Main Body                                    *
****************************************************************/
int main(int argc, char *argv[])
{

	int quantum = atoi(argv[1]);
	int contSwitch = atoi(argv[2]);
	char * fileName = argv[3];
	FILE* file = fopen(fileName, "r");
	char line[16];
	int numProcesses = 0;

	if(file == NULL)
	{
		printf("Error reading in file\n");
		
	}
	while(fgets(line, sizeof(line), file))
	{
		numProcesses++;
	}
	
	struct Process processes[numProcesses];
	

	int lineIteration = 0;
		
}
