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
	int head;
	int amountNode;
	struct Node *node[100];
	
}Node;
/*
typedef struct ArrHold
{
	int ** matrix;
	int * parNodes;

}ArrHold;
*/

void searchNode(Node *, int, int **, int [], int);
void insertAfter(Node *, int, int);



int main(int argc, char * argv[])
{
	int horzNums = 0;
	int vertNums = 1;
	int i = 0;
	int j = 0;

	char lineNums[100];
	char *temp;

	FILE *readptr;
/*
	Node *nodes = malloc(sizeof(Node));

	nodes->nodeVal = 8;

	Node *newNod = malloc(sizeof(Node));
	Node *secNode = malloc(sizeof(Node));

	secNode->nodeVal = 22;

	nodes->node[0] = newNod;
	newNod->node[0] = secNode;

	newNod->nodeVal = 9;
*/
	//printf("%d %d\n", nodes->node[0]->nodeVal, nodes->node[0]->node[0]->nodeVal);

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

	int ** matrix = (int**) malloc(sizeof(int*)*horzNums);

	for(i = 0; i < vertNums; i++)
	{
		*(matrix + i) = (int*)malloc(sizeof(int)*horzNums);
	}

	for(i = 0; i < vertNums; i++)
	{
		for(j = 0; j < vertNums; j++)
		{
			matrix[i][j]= i * j;
		}
	}

	//int * parNodes = (int*) malloc(sizeof(int) * horzNums);

	//ArrHold *arrays = malloc(sizeof(struct ArrHold));
	
	//int tempMatrix[horzNums][vertNums];
	//int tempParNodes[horzNums];


	//arrays->matrix = tempMatrix;
	//arrays->parNodes = tempParNodes;

	//arrays->matrix = malloc(horzNums * sizeof(int) + vertNums * sizeof(int));
	//arrays->parNodes = malloc(horzNums * sizeof(int));


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

	printf("%d\n", (matrix[1][1] + matrix[0][1]));


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

	Node *root = malloc(sizeof(Node));
	root->nodeVal = 0;
	root->head = -1;

	int parNodes[vertNums];

	for(i = 0; i < vertNums; i++)
	{
		parNodes[i] = -1;
	}

	int level = -1;

	searchNode(root, horzNums, matrix, parNodes, level);

	


	return EXIT_SUCCESS;
}

void searchNode(Node* node, int horzNums, int **matrix, int parNodes[], int level)
{
	int i = 0;
	int j = 0;
	int nodeAmount = 0;

	level++;

	parNodes[level] = node->nodeVal;

	printf("%d\n", node->nodeVal);

	//Creating Child Nodes
	for(i = 0; i < horzNums; i++)
	{
		for(j = 0; j < horzNums; j++)
		{
			if(i == parNodes[j])
			{
				break;
			}

			else if(matrix[node->nodeVal][i] == 1)
			{
				insertAfter(node, i, nodeAmount);
				nodeAmount++;
			}
		}
	}

	for(i = 0; i < nodeAmount; i++)
	{
		searchNode(node->node[i], horzNums, matrix, parNodes, level);
	}
}

void insertAfter(Node* prev_node, int new_data, int nodeAmount) 
{ 
	/*1. check if the given prev_node is NULL */ 
	if (prev_node == NULL)  
	{  
		printf("the given previous node cannot be NULL");        
		return;   
	}   
	             
	/* 2. allocate new node */
	struct Node* new_node =(struct Node*) malloc(sizeof(struct Node)); 
									    
	/* 3. put in the data  */
	new_node->nodeVal  = new_data; 
						       
	/* 4. Make next of new node as next of prev_node */
	//new_node->next = prev_node->next;  
	    
	/* 5. move the next of prev_node as new_node */
	prev_node->node[nodeAmount] = new_node; 
	prev_node->amountNode = nodeAmount + 1;
}
	
