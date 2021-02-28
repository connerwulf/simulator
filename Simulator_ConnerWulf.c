
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/****************************************************************
*                  Main Body                                    *
****************************************************************/
int main(int argc, char *argv[])
{

	int quantum = atoi(argv[1]);
	int contSwitch = atoi(argv[2]);
	char * fileName = argv[3];
	
	printf("%d\n", quantum);
	printf("%d\n", contSwitch);
	printf("%s\n", fileName);
	
}
