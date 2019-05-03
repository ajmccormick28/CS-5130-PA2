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

// Struct Node used to hold nodes of the tree 
typedef struct Node
{
	int nodeVal;
	int amountNode;
	
	struct Node *node[100];
	struct Node *head;
	
}Node;

int searchNode(Node *, int, int **, int [], int, int [], int [], int, int []);
int goDownPath(Node *, int, int [], int, int, int [], int);
int goUpPath(int [], int, Node *, int, int, int[], int);

void insertAfter(Node *, int, int);
void freeMem(Node *);

/*
  *********************************************************************
  *                                                                   *
  * 			Main Function                                 *
  * 			                                              *
  ********************************************************************* 			                                               
*/

int main(int argc, char * argv[])
{
	int horzNums = 0;
	int vertNums = 1;
	int i = 0;
	int j = 0;
        int level = -1;
        int path = -1; // Used to hold the shortest path 

	char lineNums[100]; 
	char *temp;

	FILE *readptr;

	// Checking to make sure there is an input file
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

	// Pulling the first line of the input file
	if(fgets(lineNums, 100, readptr) == NULL)
	{
		perror("main: Error: Line 1 in the input file is empty");
		return EXIT_FAILURE; 
	}

	temp = strtok(lineNums, " \n\t");

	// Getting the number of rows
	while(temp != NULL)
	{
		horzNums++;
		temp = strtok(NULL, " \n\t");
	}

	// Getting the number of columns
	while(fgets(lineNums, 100, readptr) != NULL)
	{
		vertNums++;
	}

	// Checking if rows are equal to columns
	if(vertNums != horzNums)
	{
		printf("The columns and rows of this matrix are not equal\n");
		return EXIT_FAILURE;
	}

	// Creating a 2D array to hold the matrix
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
	
	// Resetting the pointer to the beginning of the input file
	fseek(readptr, 0, SEEK_SET);

	// Populating the matrix from the input file
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

	printf("Matrix:\n");

	// Displaying the matrix
	for(i = 0; i < horzNums; i++)
	{
		for(j = 0; j < vertNums; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		
		printf("\n");
	}

	// Error checks for the matrix
	for(i = 0; i < horzNums; i++)
	{
		for(j = 0; j < vertNums; j++)
		{
			// If the column number is less than the row number
			if(j < i)
			{
				// Checking if value is 0 or 1
				if(matrix[i][j] != 0 && matrix[i][j] != 1)
				{
					printf("This matrix is invaild because it includes a non-bit value\n");
					return EXIT_FAILURE;
				}

				// Checking to make sure graph is symmetric
				else if(matrix[i][j] != matrix[j][i])
				{
					printf("This matrix is not symmetric\n");
					return EXIT_FAILURE;
				}
			}
			
			// If column is equal to row
			else if(j == i)
			{
				// Checking to make sure this value is zero
				if(matrix[i][j] != 0)
				{
					printf("Undirected graphs cannot have a self-loop\n");
					return EXIT_FAILURE;
				}
			}

			else
			{
				// Checking to make sure graph is symmetric
				if(matrix[i][j] != 0 && matrix[i][j] != 1)
				{
					printf("This matrix is invaild because it includes a non-bit value\n");
					return EXIT_FAILURE;
				}
			}
		}	
	}

	// Creating the root node for the tree
	Node *root = malloc(sizeof(Node));
	root->nodeVal = 0;
	root->head = NULL;
	root->amountNode = 0;

	// Creating int arrays 
	int tempShortPath[vertNums];
	int shortPath[vertNums];
	int parNodes[vertNums];
	int visable[vertNums];

	// Populating the int arrays with -1 in each indecie
	for(i = 0; i < vertNums; i++)
	{
		tempShortPath[i] = -1;
		parNodes[i] = -1;
		shortPath[i] = -1;
		visable[i] = -1;
	}

	// Adding the zero node to the visable nodes
	visable[0] = 0;

	// Call to function searchNode; Set path to an int value
	path = searchNode(root, horzNums, matrix, parNodes, level, shortPath, visable, path, tempShortPath);

	// Outputting shortest length value
	printf("Shortest Path Length: %d\n", path);
	
	// Outputting the nodes in the shortest path
	printf("Shortest Path Route:\n");

	for(i = 0; i < horzNums; i++)
	{
		if(shortPath[i] != -1)
		{
			printf("Node: %d", shortPath[i]);
		}

		if(i < horzNums - 1 && shortPath[i + 1] != -1)
		{
			printf(" -> ");
		}
	}

	printf("\n");
	
	// Freeing the nodes in the tree
	freeMem(root);

	fclose(readptr);
	free(matrix);

	return EXIT_SUCCESS;
}

/*
  *********************************************************************
  *                                                                   *
  *                   Function searchNode                             *
  *     This function takes a node and looks for the connections it   *
  *     has to other nodes and then creates those nodes.  It then     *
  *     recursivly calls those nodes and does the same thing with     *
  *     those nodes.  If it finds a node is created with on that      *
  *     already has that value, it calls goUpNode to search for the   *
  *     path length and nodes in that path. It returns an int value.  *
  *                                                                   *
  *********************************************************************
*/

int searchNode(Node* node, int horzNums, int **matrix, int parNodes[], int level, int shortPath[], int visable[], int path, int tempShortPath[])
{
	int i = 0;
	int j = 0;
	int k = 0;
	int nodeAmount = 0;
	int curPath = 0; // Holds the current path length when searching for a cycle

	char check = 'n';

	level++; // Adding one to the level

	// Adding this node to the parent node array
	parNodes[level] = node->nodeVal;

	// Creating child nodes and checking for cycles 
	for(i = 0; i < horzNums; i++)
	{
		check = 'n';
		curPath = 1;

		// Checking to see if the node trying to be created is already a parent node of the current node; will not create node if so
		for(j = 0; j < horzNums; j++)
		{
			if(i == parNodes[j])
			{
				check = 'y';
			}
		}
		
		// If not a parent node
		if(check != 'y')
		{
			// Checking the matrix for a '1' siginaling an edge
			if(matrix[node->nodeVal][i] == 1)
			{
				// Call to function insertAfter
				insertAfter(node, i, nodeAmount);

				// Incrementing the node Amount, childern the current node has
				nodeAmount++;
				
				// Checking to see if this node value has already been created
				if(i == visable[i])
				{
					// Setting the first two nodes of the current path 
					tempShortPath[0] = i;
					tempShortPath[1] = node->nodeVal;

					// Call to function goUpPath; Set path to an int value
					path = goUpPath(shortPath, path, node, i, curPath, tempShortPath, horzNums);
					
					// Resetting the first three nodes of the current path
					tempShortPath[0] = -1;
					tempShortPath[1] = -1;
					tempShortPath[2] = -1;
				}

				// Adding the current node value to the visable array
				visable[i] = i;
			}
		}
	}

	// Going to child nodes to create nodes attached to them
	for(i = 0; i < nodeAmount; i++)
	{
		// Recursive Call to searchNode; path is set to int value
		path = searchNode(node->node[i], horzNums, matrix, parNodes, level, shortPath, visable, path, tempShortPath);
		
		// Deletes previous node from the parent nodes array
		parNodes[level + 1] = -1;
	}

	return path; // Returning an int value
}

/*
  *********************************************************************
  *                                                                   *
  *                   Function goUpPath                               *
  *     This function takes a node and makes calls to the function    *
  *     goDownPath for each of it's child nodes.  Once it has called  *
  *     all it's childern, it then recursivly calls itself by going   *
  *     to the parent node of the current node and restarting the     *
  *     the process.  It returns an int value.                        *
  *                                                                   *
  *********************************************************************
*/


int goUpPath(int shortPath[], int path, Node* node, int numSearch, int curPath, int tempShortPath[], int horzNums)
{
	int i = 0;

	// Creating a parent node and setting it to the head of node
	Node *parent = node->head;

	// Adding one to the current path
	curPath++;

	// Adding the current node to the current path
	tempShortPath[curPath] = parent->nodeVal;
	
	// Checking the child nodes attached to the parent for the node value needed
	for(i = 0; i < parent->amountNode; i++)
	{
		// Making sure we don't go back down the node we orignally came from on this path
		if(parent->node[i]->nodeVal != node->nodeVal)
		{
			// Call to function goDownPath; path is set to the int value returned
			path = goDownPath(parent->node[i], path, shortPath, numSearch, curPath, tempShortPath, horzNums);

			// Deleting the previous child node from the current path 
			tempShortPath[curPath + 1] = -1;
		}
	}

	// Checking to make sure we are not at the root node
	if(parent->head != NULL)
	{
		// Recursive call to function goUpPath; path is set to the int value returned
		path = goUpPath(shortPath, path, parent, numSearch, curPath, tempShortPath, horzNums);

		// Deleting the previous parent node from the current path
		tempShortPath[curPath + 1] = -1;
	}

	return path; // Return an int value
}

/*
  *********************************************************************
  *                                                                   *
  *                   Function goDownPath                             *
  *     This function takes a node and checks to see if it is the     *
  *     node that is being looked for.  If it is, the function        *
  *     checks to see if the current path is the smallest one and     *
  *     updates the variables if needed.  It then makes recursive     *
  *     calls to the function goDownPath for each of it's child       *
  *     nodes.  It returns an int value.                              *
  *                                                                   *
  *********************************************************************
*/

int goDownPath(Node *node, int path, int shortPath[], int numSearch, int curPath, int tempShortPath[], int horzNums)
{
	int i = 0;

	// Adding to the length of the current path
	curPath ++;

	// Adding the current node to the current path
	tempShortPath[curPath] = node->nodeVal;

	// Checking if this is the node that is being searched for
	if(node->nodeVal == numSearch)
	{
		// If no path has been found yet
		if(path == -1)
		{
			// Updating the length of the shortest path
			path = curPath;
			
			// Updating the nodes of the shortest path
			for(i = 0; i < horzNums; i++)
			{
				shortPath[i] = tempShortPath[i];
			}
		}

		// Checking to see if it is shorter than the current shortest path
		else if(curPath < path)
		{
			// Updating the length of the shortest path
			path = curPath;

			// Updating the nodes of the shortest path
			for(i = 0; i < horzNums; i++)
			{
				shortPath[i] = tempShortPath[i];
			}
		}
	}

	// If this node is not the one looked for, check it's child nodes
	else
	{
		for(i = 0; i < node->amountNode; i++)
		{
			// Recursive Call to function goDownPath; path is set to return int value
			path = goDownPath(node->node[i], path, shortPath, numSearch, curPath, tempShortPath, horzNums);
	
			// Deleting child node just visited from temorpary shortest path holder
			tempShortPath[curPath + 1] = -1;
		}
	}

	return path; // Returning int value
}

/*
  *********************************************************************
  *                                                                   *
  *                     Function insertAfter                          *
  *     This function takes a node and information to make a child    *
  *     node of the orginal node.                                     *
  *                                                                   *
  *********************************************************************
*/

void insertAfter(Node* prev_node, int new_data, int nodeAmount) 
{ 
	// 1. check if the given prev_node is NULL 
	if (prev_node == NULL)  
	{  
		printf("the given previous node cannot be NULL");        
		return;   
	}   
	             
	// 2. allocate new node 
	Node *new_node = malloc(sizeof(Node));
									    
	// 3. put in the data  
	new_node->nodeVal  = new_data; 		       
 
	// 4. move the next of prev_node as new_node 
	prev_node->node[nodeAmount] = new_node; 
	prev_node->amountNode = nodeAmount + 1;
	new_node->head = prev_node;
	new_node->amountNode = 0;
}

/*
  *********************************************************************
  *                                                                   *
  *                       Function freeMem                            *
  *     This function frees all the nodes within the tree.            *
  *                                                                   *
  *********************************************************************
*/


void freeMem(Node *node)
{
	int i = 0;
	
	// Searching for childern nodes attached
	for(i = 0; i < node->amountNode; i++)
	{
		freeMem(node->node[i]);
	}
	
	// Freeing current memory allocation
	free(node);
}

