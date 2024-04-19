//#include <stdio.h>
//#include <time.h>
//#include <assert.h>
//#define _CRT_SECURE_NO_WARNINGS
//typedef struct {
//	int day;
//	int month;
//	int year;
//}Date;
//typedef struct {
//	int taskId;
//	char* description;
//	Date endDate;
//	Date finishDate;
//
//}Task;
//
//// Structure to represent a stack node
//typedef struct StackNode {
//	Task data;
//	struct StackNode* next;
//} StackNode;
//
//// Structure to represent the stack
//typedef struct {
//	StackNode* top;
//} Stack;
//
//// Function to create a new stack node
//StackNode* createNode(Task task) {
//	StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
//	if (newNode == NULL) {
//		printf("Memory allocation failed!\n");
//		exit(1);
//	}
//	newNode->data = task;
//	newNode->next = NULL;
//	return newNode;
//}
//
//// Function to initialize the stack
//void initialize(Stack* stack) {
//	stack->top = NULL;
//}
//
//// Function to check if the stack is empty
//int isEmpty(Stack* stack) {
//	return (stack->top == NULL);
//}
//
//// Function to push an element onto the stack
//void push(Stack* stack, Task task) {
//	StackNode* newNode = createNode(task);
//	newNode->next = stack->top;
//	stack->top = newNode;
//}
//
//// Function to pop an element from the stack
//Task pop(Stack* stack) {
//	if (isEmpty(stack)) {
//		printf("Stack underflow!\n");
//		exit(1);
//	}
//	StackNode* temp = stack->top;
//	Task data = temp->data;
//	stack->top = stack->top->next;
//	free(temp);
//	return data;
//}
//
//// Function to return the top element of the stack without removing it
//Task peek(Stack* stack) {
//	if (isEmpty(stack)) {
//		printf("Stack is empty!\n");
//		exit(1);
//	}
//	return stack->top->data;
//}
//
//
//// Structure to represent a node in the binary search tree
//typedef struct TreeNode {
//	Task task;
//	struct TreeNode* left;
//	struct TreeNode* right;
//} TreeNode;
//
//// Function to create a new node
//TreeNode* createNode(Task task) {
//	TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
//	if (newNode == NULL) {
//		printf("Memory allocation failed!\n");
//		exit(1);
//	}
//	newNode->task = task;
//	newNode->left = NULL;
//	newNode->right = NULL;
//	return newNode;
//}
//
//// Function to insert a task into the binary search tree
//TreeNode* insert(TreeNode* root, Task task) {
//	if (root == NULL) {
//		return createNode(task);
//	}
//	if (task.taskId < root->task.taskId) {
//		root->left = insert(root->left, task);
//	}
//	else if (task.taskId > root->task.taskId) {
//		root->right = insert(root->right, task);
//	}
//	return root;
//}
//
//// Function to search for a task in the binary search tree
//TreeNode* search(TreeNode* root, int taskId) {
//	if (root == NULL || root->task.taskId == taskId) {
//		return root;
//	}
//	if (taskId < root->task.taskId) {
//		return search(root->left, taskId);
//	}
//	else {
//		return search(root->right, taskId);
//	}
//}
//
//// Function to traverse the binary search tree in inorder
//void inorderTraversal(TreeNode* root) {
//	if (root != NULL) {
//		inorderTraversal(root->left);
//		printf("Task ID: %d, Description: %s\n", root->task.taskId, root->task.description);
//		inorderTraversal(root->right);
//	}
//}
//
//
//// Function to split a string by a delimiter and return the tokens
//char** splitString(char* str, const char* delimiter, int* count) {
//	char** tokens = NULL;
//	*count = 0;
//
//	char* token = strtok(str, delimiter);
//	while (token != NULL) {
//		tokens = realloc(tokens, (*count + 1) * sizeof(char*));
//		if (tokens == NULL) {
//			printf("Memory allocation failed!\n");
//			exit(1);
//		}
//		tokens[*count] = token;
//		(*count)++;
//		token = strtok(NULL, delimiter);
//	}
//
//	return tokens;
//}
//
//// Function to convert a string in the format "dd/mm/yyyy" to a Date struct
//Date stringToDate(char* str) {
//	Date date;
//	int count;
//	char** tokens = splitString(str, "/", &count);
//
//	if (count != 3) {
//		printf("Invalid date format!\n");
//		exit(1);
//	}
//
//	date.day = atoi(tokens[0]);
//	date.month = atoi(tokens[1]);
//	date.year = atoi(tokens[2]);
//
//	free(tokens);
//	return date;
//}
//
//// Function to read tasks from file and add them to the binary search tree
//TreeNode* readTasksFromFile(const char* filename, TreeNode* root) {
//	FILE* file = fopen(filename, "r");
//	if (file == NULL) {
//		printf("Error opening file %s\n", filename);
//		exit(1);
//	}
//
//	char line[100];
//	while (fgets(line, sizeof(line), file)) {
//		Task task;
//		int count;
//		char** tokens = splitString(line, ",", &count);
//
//		if (count != 4) {
//			printf("Invalid data format in file!\n");
//			exit(1);
//		}
//
//		task.taskId = atoi(tokens[0]);
//		task.description = strdup(tokens[1]);
//		task.endDate = stringToDate(tokens[2]);
//		task.finishDate = stringToDate(tokens[3]);
//
//		root = insert(root, task);
//
//		free(tokens[1]); // Freeing memory allocated for description
//		free(tokens);    // Freeing memory allocated for tokens
//	}
//
//	fclose(file);
//	return root;
//}
//
//// Function to traverse the binary search tree in inorder
//void inorderTraversal(TreeNode* root) {
//	if (root != NULL) {
//		inorderTraversal(root->left);
//		printf("Task ID: %d, Description: %s\n", root->task.taskId, root->task.description);
//		inorderTraversal(root->right);
//	}
//}
//
//// Function to traverse the binary search tree in inorder and write tasks to file
//void inorderTraversalWriteToFile(TreeNode* root, FILE* file) {
//	if (root != NULL) {
//		inorderTraversalWriteToFile(root->left, file);
//		fprintf(file, "Task ID: %d, Description: %s\n", root->task.taskId, root->task.description);
//		inorderTraversalWriteToFile(root->right, file);
//	}
//}
//
//// Function to save tasks from binary search tree to a file
//void saveTasksToFile(TreeNode* root, const char* filename) {
//	FILE* file = fopen(filename, "w");
//	if (file == NULL) {
//		printf("Error opening file %s\n", filename);
//		exit(1);
//	}
//
//	inorderTraversalWriteToFile(root, file);
//
//	fclose(file);
//}
//
//
//
//
//
//
//// Function to insert a task into the binary search tree ordered by finish date
//TreeNode* insertByFinishDate(TreeNode* root, Task task) {
//	if (root == NULL) {
//		return createNode(task);
//	}
//
//	// Compare finish dates for ordering
//	if (task.finishDate.year < root->task.finishDate.year ||
//		(task.finishDate.year == root->task.finishDate.year && task.finishDate.month < root->task.finishDate.month) ||
//		(task.finishDate.year == root->task.finishDate.year && task.finishDate.month == root->task.finishDate.month && task.finishDate.day < root->task.finishDate.day)) {
//		root->left = insertByFinishDate(root->left, task);
//	}
//	else {
//		root->right = insertByFinishDate(root->right, task);
//	}
//	return root;
//}
//
//// Function to prompt user for task details and add the task to the binary search tree
//TreeNode* AddTask(TreeNode* root) {
//	Task task;
//	printf("Enter Task ID: ");
//	scanf("%d", &task.taskId);
//
//	printf("Enter Description: ");
//	char description[100];
//	scanf(" %[^\n]s", description);  // Read description with spaces
//	task.description = strdup(description);
//
//	printf("Enter End Date (dd mm yyyy): ");
//	scanf("%d %d %d", &task.endDate.day, &task.endDate.month, &task.endDate.year);
//
//	printf("Enter Finish Date (dd mm yyyy): ");
//	scanf("%d %d %d", &task.finishDate.day, &task.finishDate.month, &task.finishDate.year);
//
//	// Insert the task into the binary search tree ordered by finish date
//	root = insertByFinishDate(root, task);
//	printf("Task added successfully!\n");
//	return root;
//}
//
//
//
//void constructDate(Date* date, Date input) {
//	date->day = input.day;
//	date->month = input.month;
//	date->year = input.year;
//
//}
//void constructTask(Task* task,int id, char* str, Date end, Date finish) {
//	task->taskId = id;
//	constructDate(&task->endDate, end);
//	constructDate(&task->finishDate, finish);
//	task->description = (char*)malloc(strlen(str) + 1);
//	assert(task->description);
//	strcpy(task->description, str);
//}