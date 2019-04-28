// Programmer: Andrew McCormick
// Function:   main.c
// Class:      CS-5130 PA 2
// Date:       4/28/2019

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct Node
{
	int nodeVal;

	struct Node *node[100];
	
}Node;


int main(int argc, char * argv[])
{
	int horzNums = 0;
	int vertNums = 1;
	int i = 0;
	int j = 0;

	char lineNums[100];
	char *temp;

	FILE *readptr;

	Node *nodes = malloc(sizeof(Node));

	nodes->nodeVal = 8;

	Node *newNod = malloc(sizeof(Node));
	Node *secNode = malloc(sizeof(Node));

	secNode->nodeVal = 22;

	nodes->node[0] = newNod;
	nodes->node[1] = secNode;

	newNod->nodeVal = 9;

	printf("%d %d\n", nodes->node[0]->nodeVal, nodes->node[1]->nodeVal);

	if(argc < 2)
	{
		printf("You didn't put any input in\n");
		return EXIT_FAILURE;
	}

	// Opening input file and error checking
	if((readptr = fopen(argv[1], "r")) == NULL)
	{
		perror("main: Error");
		return EXIT_FAILURE;
	}

	if(fgets(lineNums, 100, readptr) == NULL)
	{
		perror("main: Error: Line 1 in the input file is empty");
		return EXIT_FAILURE; 
	}

	temp = strtok(lineNums, " \n\t");

	while(temp != NULL)
	{
		horzNums++;
		temp = strtok(NULL, " \n\t");
	}

	while(fgets(lineNums, 100, readptr) != NULL)
	{
		vertNums++;
	}

	printf("\nVert:%d Horz:%d", vertNums, horzNums);
	if(vertNums != horzNums)
	{
		printf("The columns and rows of this matrix are not equal\n");
		return EXIT_FAILURE;
	}

	int matrix[horzNums][vertNums];



	fseek(readptr, 0, SEEK_SET);

	for(i = 0; i < horzNums; i++)
	{
		fgets(lineNums, 100, readptr);
		temp = strtok(lineNums, " \n\t");
		matrix[i][0] = (int) strtol(temp, NULL, 10);

		for(j = 1; j < vertNums; j++)
		{
			temp = strtok(NULL, " \n\t");
			matrix[i][j] = (int) strtol(temp, NULL, 10);
		}
	}

	printf("\n");

	for(i = 0; i < horzNums; i++)
	{
		for(j = 0; j < vertNums; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		
		printf("\n");
	}

	printf("%d\n", (matrix[1][1]+matrix[0][1]));


	for(i = 0; i < horzNums; i++)
	{
		for(j = 0; j < vertNums; j++)
		{
			if(j < i)
			{
				if(matrix[i][j] != 0 && matrix[i][j] != 1)
				{
					printf("This matrix is invaild because it includes a non-bit value\n");
					return EXIT_FAILURE;
				}

				else if(matrix[i][j] != matrix[j][i])
				{
					printf("This matrix is not symmetric\n");
					return EXIT_FAILURE;
				}


			}

			else if(j == i)
			{
				if(matrix[i][j] != 0)
				{
					printf("Undirected graphs cannot have a self-loop\n");
					return EXIT_FAILURE;
				}
			}

			else
			{
				if(matrix[i][j] != 0 && matrix[i][j] != 1)
				{
					printf("This matrix is invaild because it includes a non-bit value\n");
					return EXIT_FAILURE;
				}
			}
		}
	
	}
	


	return EXIT_SUCCESS;
}
