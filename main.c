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
	
	int amountNode;
	struct Node *node[100];
	struct Node *head;
	
}Node;
/*
typedef struct ArrHold
{
	int ** matrix;
	int * parNodes;

}ArrHold;
*/

int searchNode(Node *, int, int **, int [], int, int [], int [], int);
void insertAfter(Node *, int, int);
int goDownPath(Node *, int, int [], int, int);
int goUpPath(int [], int, Node *, int, int);





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
	root->head = NULL;

	int shortPath[vertNums];
	int parNodes[vertNums];
	int visable[vertNums];

	for(i = 0; i < vertNums; i++)
	{
		parNodes[i] = -1;
		shortPath[i] = -1;
		visable[i] = -1;
	}

	visable[0] = 0;
	int level = -1;
	int path = -1;

	path = searchNode(root, horzNums, matrix, parNodes, level, shortPath, visable, path);


	


	return EXIT_SUCCESS;
}

int searchNode(Node* node, int horzNums, int **matrix, int parNodes[], int level, int shortPath[], int visable[], int path)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int nodeAmount = 0;
	int curPath = 0;

	char check = 'n';
	level++;

	parNodes[level] = node->nodeVal;
	//visable[node->nodeVal] = node->nodeVal;

//	printf("nodeVal:%d Level:%d\n", node->nodeVal, level);
//	printf("Header Node\n");
/*
	for(i = 0; i < horzNums; i++)
	{
		printf("%d ", parNodes[i]);
	}
*/
//	printf("\n");
	//Creating Child Nodes
	for(i = 0; i < horzNums; i++)
	{
		check = 'n';


		for(j = 0; j < horzNums; j++)
		{
			if(i == parNodes[j])
			{
				check = 'y';
			}
		}
		
		if(check != 'y')
		{
			if(matrix[node->nodeVal][i] == 1)
			{
				insertAfter(node, i, nodeAmount);
				nodeAmount++;
			/*	
				for(k = 0; k < horzNums; k++)
				{
					printf("%d ", visable[k]);
				}
				printf("\n");
*/
				if(i == visable[i])
				{
					printf("Searching:\n");
					path = goUpPath(shortPath, path, node, i, curPath);
//					printf("I'm visable\n");
					printf("\n");
				}

				visable[i] = i;
			}
		}
	}

	for(i = 0; i < nodeAmount; i++)
	{
		path = searchNode(node->node[i], horzNums, matrix, parNodes, level, shortPath, visable, path);
		parNodes[level + 1] = -1;
	}
	
	return path;
}

int goUpPath(int shortPath[], int path, Node* node, int numSearch, int curPath)
{
	Node *parent = malloc(sizeof(Node));
//	Node *temp = malloc(sizeof(Node));

	int i = 0;

// Adding curPath to program

	parent = node->head;
	
	printf("Head: %d nodeVal: %d\n", parent->nodeVal, node->nodeVal);
		
	
	for(i = 0; i < parent->amountNode; i++)
	{
		if(parent->node[i]->nodeVal != node->nodeVal)
		{
			path = goDownPath(parent->node[i], path, shortPath, numSearch, curPath);
		}
	}

	if(parent->head != NULL)
	{
		path = goUpPath(shortPath, path, parent->node[i], numSearch, curPath);
	}

	return path;
}

int goDownPath(Node *node, int path, int shortPath[], int numSearch, int curPath)
{
	int i = 0;

	printf("child: %d \n", node->nodeVal);

	if(node->nodeVal == numSearch)
	{
		if(path == -1)
		{
			//path = curPath;
		}

		else if(curPath < path)
		{
			//path = curPath;

		}
	}

	else
	{
		for(i = 0; i < node->amountNode; i++)
		{
			path = goDownPath(node->node[i], path, shortPath, numSearch, curPath);
		}
	}

	return path;
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
	//struct Node* new_node =(struct Node*) malloc(sizeof(struct Node)); 
	Node *new_node = malloc(sizeof(Node));
									    
	/* 3. put in the data  */
	new_node->nodeVal  = new_data; 
						       
	/* 4. Make next of new node as next of prev_node */
	//new_node->next = prev_node->next;  
	    
	/* 5. move the next of prev_node as new_node */
	prev_node->node[nodeAmount] = new_node; 
	prev_node->amountNode = nodeAmount + 1;
	new_node->head = prev_node;

	printf("nodeVal:%d\n", new_node->nodeVal);
}
	
