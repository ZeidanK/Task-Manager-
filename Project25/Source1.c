#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// Date struct
typedef struct {
	int day;
	int month;
	int year;
} Date;

// Task struct
typedef struct {
	int taskId;
	char* description;
	Date endDate;// the date the task was completed
	Date finishDate;//the date for when the task should be completed
} Task;

// Structure to represent a node in the binary search tree
typedef struct TreeNode {
	Task task;
	struct TreeNode* left;
	struct TreeNode* right;
} TreeNode;

// Structure to represent a node in the stack
typedef struct StackNode {
	Task task;
	struct StackNode* next;
} StackNode;

// Structure to represent the stack
typedef struct {
	StackNode* top;
} Stack;

// Queue node structure
typedef struct QueueNode {
	Task task;
	struct QueueNode* next;
} QueueNode;

// Queue structure
typedef struct {
	QueueNode* front;
	QueueNode* rear;
} Queue;

// Function declarations
TreeNode* createNode(Task task);
TreeNode* insertByFinishDate(TreeNode* root, Task task);
void push(Stack* stack, Task task);
Task pop(Stack* stack);
void saveTasksToFile(TreeNode* root, const char* filename);
void inorderTraversalWriteToFile(TreeNode* root, FILE* file);
TreeNode* readTasksFromFile(const char* filename, TreeNode* root);
TreeNode* AddTask(TreeNode* root);
char** splitString(char* str, const char* delimiter, int* count);
Date stringToDate(char* str);
void inorderTraversal(TreeNode* root);
void ViewAllTasks(TreeNode* root);
void CompleteTask(TreeNode** root, Stack* stack, Date completionDate, int taskId);
void ViewCompletedTasks(Stack* stack);
TreeNode* searchTaskById(TreeNode* root, int taskId);
TreeNode* deleteNode(TreeNode* root, int taskId);
int isEmpty(Stack* stack);
void fillStackFromFile(Stack* stack);
void SearchTask(TreeNode* root, Date searchDate);
void inorderTraversalWithUserInput(TreeNode* root, Queue* queue);
void SavetoFile(TreeNode* root, Stack* stack);
void Menu(TreeNode** root, Stack* stack, Queue* queue);

// Function prototypes (Queue)
void initializeQueue(Queue* queue);
void enqueue(Queue* queue, Task task);
void enqueueAtPosition(Queue* queue, Task task, int position);
void ViewPrioritizedTasks(Queue* queue);
int isEmptyQ(Queue* queue);


void PrioritizeTask(TreeNode* root, Queue *queue);

void constructTask(Task* task, int id, char* str, Date end, Date finish);
void constructDate(Date* date, Date input);







// Function to initialize the queue
void initializeQueue(Queue* queue) {
	queue->front = NULL; 
	queue->rear = NULL;
}

// Function to enqueue a task into the queue
void enqueue(Queue* queue, Task task) {
	QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
	if (newNode == NULL) {
		printf("Memory allocation failed.\n");
		return;
	}
	
	constructTask(&newNode->task, task.taskId, task.description, task.endDate, task.finishDate);
	newNode->next = NULL;

	if (isEmptyQ(queue)) {
		queue->front = queue->rear = newNode;
	}
	else {
		queue->rear->next = newNode;
		queue->rear = newNode;
	}
}

// Function to enqueue a task into the queue at a specified position
void enqueueAtPosition(Queue* queue, Task task, int position) {
	if (position < 1) {
		printf("Invalid position.\n");
		return;
	}

	QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
	assert(newNode);
	//newNode->task = (Task*)malloc(sizeof(Task));
	//assert(newNode->task);
	if (newNode == NULL) {
		printf("Memory allocation failed.\n");
		return;
	}
	constructTask(&newNode->task, task.taskId, task.description, task.endDate, task.finishDate);

	if (isEmptyQ(queue) || position == 1) {
		newNode->next = queue->front;
		queue->front = newNode;
		if (queue->rear == NULL) {
			queue->rear = newNode;
		}
		return;
	}

	QueueNode* current = queue->front;
	int currentPosition = 1;
	while (currentPosition < position - 1 && current->next != NULL) {
		current = current->next;
		currentPosition++;
	}

	newNode->next = current->next;
	current->next = newNode;

	if (newNode->next == NULL) {
		queue->rear = newNode;
	}
}

// Function to display tasks in the queue
void ViewPrioritizedTasks(Queue* queue) {
	if (isEmptyQ(queue)) {
		printf("Queue is empty.\n");
		return;
	}

	QueueNode* current = queue->front;
	while (current != NULL) {
		printf("Task ID: %d, Description: %s, End Date: %02d/%02d/%04d, Finish Date: %02d/%02d/%04d\n",
			current->task.taskId, current->task.description,
			current->task.endDate.day, current->task.endDate.month, current->task.endDate.year,
			current->task.finishDate.day, current->task.finishDate.month, current->task.finishDate.year);
		current = current->next;
	}
}

// Function to check if the queue is empty
int isEmptyQ(Queue* queue) {
	return queue->front == NULL;
}
void constructDate(Date* date, Date input) {
	date->day = input.day;
	date->month = input.month;
	date->year = input.year;

}
void constructTask(Task* task,int id, char* str, Date end, Date finish) {
	task->taskId = id;
	constructDate(&task->endDate, end);
	constructDate(&task->finishDate, finish);
	task->description = (char*)malloc(strlen(str) + 1);
	assert(task->description);
	strcpy(task->description, str);
}



// Function to create a new node
TreeNode* createNode(Task task) {
	TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
	if (newNode == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
	constructTask(&newNode->task, task.taskId, task.description, task.endDate, task.finishDate);
	newNode->task.endDate.month = 0;
	newNode->task.endDate.day = 0;
	newNode->task.endDate.year = 0;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

// Function to insert a task into the binary search tree ordered by finish date
TreeNode* insertByFinishDate(TreeNode* root, Task task) {
	if (root == NULL) {
		return createNode(task);
	}

	// Compare finish dates for ordering
	if (task.finishDate.year < root->task.finishDate.year ||
		(task.finishDate.year == root->task.finishDate.year && task.finishDate.month < root->task.finishDate.month) ||
		(task.finishDate.year == root->task.finishDate.year && task.finishDate.month == root->task.finishDate.month && task.finishDate.day < root->task.finishDate.day)) {
		root->left = insertByFinishDate(root->left, task);
	}
	else {
		root->right = insertByFinishDate(root->right, task);
	}
	return root;
}

// Function to delete a node with a given task ID from the binary search tree --------------------------------------------------------------
TreeNode* deleteNode(TreeNode* root, int taskId) {
	// Base case: If the tree is empty, return NULL
	if (root == NULL)
		return NULL;

	// If the task ID to be deleted is less than the current node's task ID,
	// recursively call deleteNode on the left subtree
	if (taskId < root->task.taskId)
		root->left = deleteNode(root->left, taskId);
	// If the task ID to be deleted is greater than the current node's task ID,
	// recursively call deleteNode on the right subtree
	else if (taskId > root->task.taskId)
		root->right = deleteNode(root->right, taskId);
	// If the current node is the node to be deleted
	else {
		// Node with only one child or no child
		if (root->left == NULL) {
			TreeNode* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL) {
			TreeNode* temp = root->left;
			free(root);
			return temp;
		}

		// Node with two children: Get the inorder successor (smallest in the right subtree)
		TreeNode* temp = root->right;
		while (temp && temp->left != NULL)
			temp = temp->left;

		// Copy the inorder successor's content to this node
		root->task = temp->task;

		// Delete the inorder successor
		root->right = deleteNode(root->right, temp->task.taskId);
	}
	return root;
}

// Function to push a task onto the stack
void push(Stack* stack, Task task) {
	StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
	if (newNode == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
	constructTask(&newNode->task, task.taskId, task.description, task.endDate, task.finishDate);
	
	newNode->next = stack->top;
	stack->top = newNode;
}

// Function to pop a task from the stack
Task pop(Stack* stack) {
	if (stack->top == NULL) {
		printf("Stack underflow!\n");
		exit(1);
	}
	StackNode* temp = stack->top;
	Task task = temp->task;
	stack->top = stack->top->next;
	free(temp);
	return task;
}
int isEmpty(Stack* stack) {
	return stack->top == NULL;
}
// Function to traverse the binary search tree in inorder and write tasks to file
void inorderTraversalWriteToFile(TreeNode* root, FILE* file) {
	if (root != NULL) {
		inorderTraversalWriteToFile(root->left, file);
		fprintf(file, "%d,%s,%02d/%02d/%04d,%02d/%02d/%04d\n", root->task.taskId, root->task.description, root->task.endDate.day, root->task.endDate.month, root->task.endDate.year, root->task.finishDate.day, root->task.finishDate.month, root->task.finishDate.year);
		inorderTraversalWriteToFile(root->right, file);
	}
}


// Function to save tasks to a file
void saveTasksToFile(TreeNode* root, const char* filename) {
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("Error opening file %s\n", filename);
		exit(1);
	}

	inorderTraversalWriteToFile(root, file);

	fclose(file);
}

void fillStackFromFile(Stack* stack) {
	FILE* file = fopen("CompletedTasks.txt", "a+");
	if (file == NULL) {
		printf("Error opening file.\n");
		return;
	}

	char line[100];
	char description[100];
	while (fgets(line, sizeof(line), file) != NULL) {
		Task task;
		// Assuming the format of each line in CompletedTasks.txt is: TaskID,Description,EndDate,FinishDate
		sscanf(line, "%d,%99[^,],%d/%d/%d,%d/%d/%d", &task.taskId, description,
			&task.endDate.day, &task.endDate.month, &task.endDate.year,
			&task.finishDate.day, &task.finishDate.month, &task.finishDate.year);
		task.description = description;
		push(stack, task);
	}

	fclose(file);
}
// Function to read tasks from file and add them to the binary search tree
TreeNode* readTasksFromFile(const char* filename, TreeNode* root) { //Load Tasks
	FILE* file = fopen(filename, "a+");
	if (file == NULL) {
		printf("Error opening file %s\n", filename);
		exit(1);
	}

	char line[100];
	while (fgets(line, sizeof(line), file)) {
		Task task;
		int count;
		char** tokens = splitString(line, ",", &count);

		if (count != 4) {
			printf("Invalid data format in file!\n");
			exit(1);
		}

		task.taskId = atoi(tokens[0]);   //change functions or add explination 
		task.description = _strdup(tokens[1]);
		constructDate(&task.endDate, stringToDate(tokens[2]));
		constructDate(&task.finishDate, stringToDate(tokens[3]));
		

		root = insertByFinishDate(root, task);

		
		free(tokens);    // Freeing memory allocated for tokens
	}

	fclose(file);
	return root;
}

// Function to prompt user for task details and add the task to the binary search tree
TreeNode* AddTask(TreeNode* root) {
	Task task;
	printf("Enter Task ID: ");
	scanf("%d", &task.taskId);

	printf("Enter Description: ");
	char description[100];
	scanf(" %[^\n]s", description);  // Read description with spaces
	task.description = _strdup(description);

	//printf("Enter End Date (dd mm yyyy): ");
	//scanf("%d %d %d", &task.endDate.day, &task.endDate.month, &task.endDate.year);

	printf("Enter Finish Date (dd mm yyyy): ");
	scanf("%d %d %d", &task.finishDate.day, &task.finishDate.month, &task.finishDate.year);

	// Insert the task into the binary search tree ordered by finish date
	root = insertByFinishDate(root, task);
	printf("Task added successfully!\n");
	return root;
}

// Function to split a string by a delimiter and return the tokens
char** splitString(char* str, const char* delimiter, int* count) {
	char** tokens = NULL;
	*count = 0;

	char* token = strtok(str, delimiter);//strtok() used to split the string when it finds the delimiter 
	while (token != NULL) {
		tokens = realloc(tokens, (*count + 1) * sizeof(char*));
		if (tokens == NULL) {
			printf("Memory allocation failed!\n");
			exit(1);
		}
		tokens[*count] = token;
		(*count)++;
		token = strtok(NULL, delimiter);// strtok() to take the second string 
	}

	return tokens;
}

// Function to convert a string in the format "dd/mm/yyyy" to a Date struct
Date stringToDate(char* str) {
	Date date;
	int count;
	char** tokens = splitString(str, "/", &count);

	if (count != 3) {
		printf("Invalid date format!\n");
		exit(1);
	}

	date.day = atoi(tokens[0]);// atoi function to turn strings into int using the strings we split with strtok()
	date.month = atoi(tokens[1]);
	date.year = atoi(tokens[2]);

	free(tokens);
	return date;
}

// Function to traverse the binary search tree in inorder
void inorderTraversal(TreeNode* root) {
	if (root != NULL) {
		inorderTraversal(root->left);
		printf("Task ID: %d, Description: %s, Finish Date: %02d/%02d/%04d\n", root->task.taskId, root->task.description, root->task.finishDate.day, root->task.finishDate.month, root->task.finishDate.year);
		inorderTraversal(root->right);
	}
}

void SearchTask(TreeNode* root, Date searchDate) {
	if (root == NULL) {
		return;
	}

	// Traverse the BST in inorder manner
	SearchTask(root->left, searchDate);

	// Check if the finish date of the current task matches the search date
	if (root->task.finishDate.year == searchDate.year &&
		root->task.finishDate.month == searchDate.month &&
		root->task.finishDate.day == searchDate.day) {
		printf("Task ID: %d, Description: %s, Finish Date: %02d/%02d/%04d\n", root->task.taskId, root->task.description, root->task.finishDate.day, root->task.finishDate.month, root->task.finishDate.year);
	}

	// Traverse the right subtree
	SearchTask(root->right, searchDate);
}

TreeNode* searchTaskById(TreeNode* root, int taskId) {
	// Traverse the tree until the task is found or until the leaf nodes are reached
	while (root != NULL) {
		// If the current node's task ID matches the search task ID, return the current node
		if (root->task.taskId == taskId)
			return root;

		// If the search task ID is less than the current node's task ID, move to the left subtree
		if (taskId < root->task.taskId)
			root = root->left;
		// Otherwise, move to the right subtree
		else
			root = root->right;
	}

	// If the task with the given task ID is not found, return NULL
	return NULL;
}

void CompleteTask(TreeNode** root, Stack* stack, Date completionDate, int taskId) {
	// Search for the task with the given taskId
	TreeNode* node = searchTaskById(*root, taskId);
	if (node == NULL) {
		printf("Task with ID %d not found!\n", taskId);
		return;
	}

	// Update the finish date of the task
	constructDate(&node->task.endDate ,completionDate);
	

	// Move the task to the stack
	push(stack, node->task);

	// Remove the task from the BST and update the root
	*root = deleteNode(*root, taskId);

	printf("Task with ID %d completed and moved to the stack.\n", taskId);
}
void ViewAllTasks(TreeNode* root) {
	printf("All Tasks:\n");
	inorderTraversal(root);
}
void ViewCompletedTasks(Stack* stack) {
	printf("Completed Tasks:\n");
	StackNode* current = stack->top;
	while (current != NULL) {
		Task task;
		constructTask(&task, current->task.taskId, current->task.description, current->task.endDate, current->task.finishDate);
		printf("Task ID: %d, Description: %s, Finish Date: %02d/%02d/%04d\n", task.taskId, task.description, task.finishDate.day, task.finishDate.month, task.finishDate.year);
		current = current->next;
	}
}

void inorderTraversalWithUserInput(TreeNode* root, Queue* queue) {
	if (root == NULL)
		return;
	inorderTraversalWithUserInput(root->left, queue);

	Task task;
	constructTask(&task, root->task.taskId, root->task.description, root->task.endDate, root->task.finishDate); // Assuming each TreeNode contains a Task
	int position;
	printf("Enter priority position for task with ID %d: ", task.taskId);
	scanf("%d", &position);
	enqueueAtPosition(queue, task, position);

	inorderTraversalWithUserInput(root->right, queue);
}

void PrioritizeTask(TreeNode* root,Queue *queue) {
	
	
	inorderTraversalWithUserInput(root, queue);

	printf("Prioritized Tasks:\n");
	//displayTasks(&queue); // Assuming you have a function to display tasks in the queue
}



void SavetoFile(TreeNode* root, Stack* stack) {
	
	saveTasksToFile(root, "Tasks.txt");
	

	// Save completed tasks from stack to CompletedTasks.txt
	FILE* completedTasksFile = fopen("CompletedTasks.txt", "w");
	if (completedTasksFile == NULL) {
		printf("Error opening CompletedTasks.txt for writing.\n");
		return;
	}
	while (!isEmpty(stack)) {
		Task task = pop(stack);
		fprintf(completedTasksFile, "%d,%s,%02d/%02d/%04d,%02d/%02d/%04d\n", task.taskId, task.description, task.endDate.day, task.endDate.month, task.endDate.year, task.finishDate.day, task.finishDate.month, task.finishDate.year);
	}
	fclose(completedTasksFile);

	printf("Data saved to Tasks.txt and CompletedTasks.txt successfully.\n");
	
}
void Menu(TreeNode** root, Stack* stack, Queue* queue) {
	int choice;
	Date completionDate;
	int taskId;
	char description[100];
	do {
		
		printf("Welcome to the Task Management System\n");

		printf("\n1. Add Task\n");
		printf("2. Search Task\n");
		printf("3. Complete Task\n");
		printf("4. View All Tasks\n");
		printf("5. View Completed Tasks\n");
		printf("6. Prioritize tasks\n");
		printf("7. Exit and save to file\n");
		printf("8. view prioritized tasks\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			system("cls");
			*root = AddTask(*root);
			break;
		case 2:
			system("cls");
			printf("Enter Task ID to search: ");
			scanf("%d", &taskId);
			// Call search task function
			
			break;
		case 3:
			system("cls");
			printf("Enter Task ID to complete: ");
			scanf("%d", &taskId);
			printf("Enter completion date (DD MM YYYY): ");
			scanf("%d %d %d", &completionDate.day, &completionDate.month, &completionDate.year);
			// Call complete task function
			CompleteTask(root, stack, completionDate, taskId);
			break;
		case 4:
			system("cls");
			
			// Call view all tasks function
			ViewAllTasks(*root);
			break;
		case 5:
			system("cls");
			
			// Call view completed tasks function
			ViewCompletedTasks(stack);
			break;
		case 6:
			system("cls");
			// Call save to file function
			PrioritizeTask(*root,queue);

			break;
		case 7:
			SavetoFile(*root, stack);
			printf("Exiting...\n");
			
			exit(1);
		case 8:
			system("cls");
			// Call save to file function
			ViewPrioritizedTasks(queue);
			break;
	
		default:
			printf("Invalid choice. Please enter a number between 1 and 8.\n");
		}
	} while (choice);
}
int main() {
	TreeNode* root = NULL;
	Stack stack = { NULL };
	Queue queue = { NULL };
	initializeQueue(&queue);
	fillStackFromFile(&stack);
	// Reading tasks from file and adding them to the binary search tree
	root = readTasksFromFile("Tasks.txt", root);//load tasks
	Menu(&root, &stack,&queue);
	
	
	saveTasksToFile(root, "Tasks.txt");

	return 0;
}
