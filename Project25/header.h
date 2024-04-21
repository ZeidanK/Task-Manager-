



typedef struct {
	int day;
	int month;
	int year;
} Date;

// Task struct
typedef struct {
	int taskId;
	char* description;
	Date CompletionDate;
	Date finishDate;
} Task;
