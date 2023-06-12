#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int day;
    int month;
    int year;
    char task[100];
    int completed;
} TodoItem;

typedef struct Node {
    TodoItem item;
    struct Node* next;
} Node;

Node* createNode(TodoItem item) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->item = item;
    newNode->next = NULL;
    return newNode;
}

void insertNode(Node** head, TodoItem item) {
    Node* newNode = createNode(item);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void markCompleted(Node* head, int index) {
    Node* temp = head;
    int i = 0;
    while (temp != NULL) {
        if (i == index) {
            temp->item.completed = 1;
            printf("Task marked as completed.\n");
            return;
        }
        temp = temp->next;
        i++;
    }
    printf("Invalid task index.\n");
}

void editTask(Node* head, int index) {
    Node* temp = head;
    int i = 0;
    while (temp != NULL) {
        if (i == index) {
            printf("Enter the new task: ");
            fgets(temp->item.task, sizeof(temp->item.task), stdin);
            temp->item.task[strcspn(temp->item.task, "\n")] = '\0';
            printf("Task updated.\n");
            return;
        }
        temp = temp->next;
        i++;
    }
    printf("Invalid task index.\n");
}

void displayTodoList(Node* head, int day, int month, int year) {
    Node* temp = head;
    int count = 0;
    while (temp != NULL) {
        if (temp->item.day == day && temp->item.month == month && temp->item.year == year) {
            printf("%d. [%s] %s\n", count, (temp->item.completed == 1 ? "X" : " "), temp->item.task);
            count++;
        }
        temp = temp->next;
    }
    if (count == 0) {
        printf("No tasks found for the given date.\n");
    }
}

float calculateCompletionPercentage(Node* head) {
    Node* temp = head;
    int totalTasks = 0;
    int completedTasks = 0;
    while (temp != NULL) {
        totalTasks++;
        if (temp->item.completed == 1) {
            completedTasks++;
        }
        temp = temp->next;
    }
    if (totalTasks == 0) {
        return 0.0;
    } else {
        return ((float)completedTasks / totalTasks) * 100;
    }
}

void freeList(Node** head) {
    Node* temp = *head;
    while (temp != NULL) {
        Node* nextNode = temp->next;
        free(temp);
        temp = nextNode;
    }
    *head = NULL;
}

void storeTodoListToFile(Node* head) {
    FILE* file = fopen("todo.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Node* temp = head;
    while (temp != NULL) {
        fprintf(file, "[%s] %d/%d/%d - %s\n", (temp->item.completed == 1 ? "X" : " "), temp->item.day, temp->item.month, temp->item.year, temp->item.task);
        temp = temp->next;
    }

    fclose(file);
    printf("Todo list stored in 'todo.txt' file.\n");
}

int main() {
    Node* todoList = NULL;
    int choice;
    int day, month, year;
    int index;

    while (1) {
        printf("\n");
        printf("========== Todo List App ==========\n");
        printf("1. Create Todo\n");
        printf("2. Mark Completed\n");
        printf("3. Edit Task\n");
        printf("4. Show Todo List\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear the newline character from the input buffer

        switch (choice) {
            case 1:
                printf("Enter the date (DDMMYYYY): ");
                scanf("%2d%2d%4d", &day, &month, &year);
                getchar();  // Clear the newline character from the input buffer

                TodoItem newItem;
                newItem.day = day;
                newItem.month = month;
                newItem.year = year;
                printf("Enter the task: ");
                fgets(newItem.task, sizeof(newItem.task), stdin);
                newItem.task[strcspn(newItem.task, "\n")] = '\0';
                newItem.completed = 0;

                insertNode(&todoList, newItem);
                printf("Todo created successfully.\n");
                break;
            case 2:
                printf("Enter the task index: ");
                scanf("%d", &index);
                getchar();  // Clear the newline character from the input buffer
                markCompleted(todoList, index);
                break;
            case 3:
                printf("Enter the task index: ");
                scanf("%d", &index);
                getchar();  // Clear the newline character from the input buffer
                editTask(todoList, index);
                break;
            case 4:
                printf("Enter the date (DDMMYYYY): ");
                scanf("%2d%2d%4d", &day, &month, &year);
                getchar();  // Clear the newline character from the input buffer
                displayTodoList(todoList, day, month, year);
                break;
            case 5:
                storeTodoListToFile(todoList);
                freeList(&todoList);
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
