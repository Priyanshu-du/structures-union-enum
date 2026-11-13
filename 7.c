
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define DESC_SIZE 100

// Commands
typedef enum {
    CMD_ADD,
    CMD_DELETE,
    CMD_VIEW,
    CMD_UPDATE,
    CMD_COMPLETE,
    CMD_FIND,
    CMD_HELP,
    CMD_EXIT,
    CMD_UNKNOWN
} CommandType;

// Task status
typedef enum {
    PENDING,
    DONE
} TaskStatus;

// Task details
typedef struct {
    int id;
    char description[DESC_SIZE];
    TaskStatus status;
} Task;

// Input result
typedef enum {
    INPUT_OK,
    INPUT_EOF,
    INPUT_TOO_LONG,
    INPUT_INVALID
} InputResult;

// Read a line safely
InputResult read_line(const char *message, char *input, size_t size) {
    printf("%s", message);

    if (fgets(input, size, stdin) == NULL) {
        return INPUT_EOF;
    }

    size_t len = strlen(input);

    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (!feof(stdin)) {
        int ch = getchar();

        if (ch != '\n' && ch != EOF) {
            while (ch != '\n' && ch != EOF) {
                ch = getchar();
            }
            return INPUT_TOO_LONG;
        }
    }

    return INPUT_OK;
}

// Remove extra spaces from the beginning and end
void trim(char *str) {
    size_t start = 0;
    size_t len = strlen(str);

    while (isspace((unsigned char)str[start])) {
        start++;
    }

    while (len > start && isspace((unsigned char)str[len - 1])) {
        len--;
    }

    memmove(str, str + start, len - start);
    str[len - start] = '\0';
}

// Convert command text into an enum value
CommandType get_command_type(char *command) {
    for (size_t i = 0; command[i] != '\0'; i++) {
        command[i] = (char)toupper((unsigned char)command[i]);
    }

    if (strcmp(command, "ADD") == 0) return CMD_ADD;
    if (strcmp(command, "DELETE") == 0) return CMD_DELETE;
    if (strcmp(command, "VIEW") == 0) return CMD_VIEW;
    if (strcmp(command, "UPDATE") == 0) return CMD_UPDATE;
    if (strcmp(command, "COMPLETE") == 0) return CMD_COMPLETE;
    if (strcmp(command, "FIND") == 0) return CMD_FIND;
    if (strcmp(command, "HELP") == 0) return CMD_HELP;
    if (strcmp(command, "EXIT") == 0) return CMD_EXIT;

    return CMD_UNKNOWN;
}

// Read and validate a task ID
InputResult read_id(int *id) {
    char input[50];
    char *end;

    InputResult result = read_line("Enter task ID: ", input, sizeof(input));

    if (result != INPUT_OK) {
        if (result == INPUT_TOO_LONG) {
            printf("ID input is too long.\n");
        }
        return result;
    }

    errno = 0;
    long number = strtol(input, &end, 10);

    while (isspace((unsigned char)*end)) {
        end++;
    }

    if (input == end || *end != '\0' ||
        errno == ERANGE || number < 1 || number > INT_MAX) {
        printf("Invalid ID. Enter a positive whole number.\n");
        return INPUT_INVALID;
    }

    *id = (int)number;
    return INPUT_OK;
}

// Find a task using its ID
size_t find_task(Task tasks[], size_t count, int id) {
    for (size_t i = 0; i < count; i++) {
        if (tasks[i].id == id) {
            return i;
        }
    }

    return count;
}

// Display one task
void display_task(Task task) {
    printf("\nTask ID: %d\n", task.id);
    printf("Description: %s\n", task.description);
    printf("Status: %s\n", task.status == DONE ? "DONE" : "PENDING");
}

// Add a new task
int add_task(Task **tasks, size_t *count, size_t *capacity) {
    int id;
    char description[DESC_SIZE];

    InputResult result = read_id(&id);

    if (result == INPUT_EOF) return 0;
    if (result != INPUT_OK) return 1;

    if (find_task(*tasks, *count, id) != *count) {
        printf("Task ID already exists.\n");
        return 1;
    }

    result = read_line("Enter task description: ", description, sizeof(description));

    if (result == INPUT_EOF) return 0;

    if (result == INPUT_TOO_LONG) {
        printf("Description is too long. Maximum length is %d characters.\n",
               DESC_SIZE - 1);
        return 1;
    }

    if (result != INPUT_OK) return 1;

    trim(description);

    if (strlen(description) == 0) {
        printf("Description cannot be empty.\n");
        return 1;
    }

    // Increase array size when it becomes full
    if (*count == *capacity) {
        size_t new_capacity = (*capacity == 0) ? 4 : *capacity * 2;

        Task *temp = realloc(*tasks, new_capacity * sizeof(Task));

        if (temp == NULL) {
            printf("Memory allocation failed.\n");
            return 1;
        }

        *tasks = temp;
        *capacity = new_capacity;
    }

    (*tasks)[*count].id = id;
    strcpy((*tasks)[*count].description, description);
    (*tasks)[*count].status = PENDING;

    (*count)++;

    printf("Task %d added successfully.\n", id);
    return 1;
}

// Delete a task
int delete_task(Task tasks[], size_t *count) {
    int id;

    InputResult result = read_id(&id);

    if (result == INPUT_EOF) return 0;
    if (result != INPUT_OK) return 1;

    size_t index = find_task(tasks, *count, id);

    if (index == *count) {
        printf("Task not found.\n");
        return 1;
    }

    // Move the remaining tasks one position back
    for (size_t i = index; i < *count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    (*count)--;

    printf("Task %d deleted successfully.\n", id);
    return 1;
}

// Display all tasks
void view_tasks(Task tasks[], size_t count) {
    if (count == 0) {
        printf("Task list is empty.\n");
        return;
    }

    printf("\n--- Task List ---\n");

    for (size_t i = 0; i < count; i++) {
        display_task(tasks[i]);
    }
}

// Update a task description
int update_task(Task tasks[], size_t count) {
    int id;
    char description[DESC_SIZE];

    InputResult result = read_id(&id);

    if (result == INPUT_EOF) return 0;
    if (result != INPUT_OK) return 1;

    size_t index = find_task(tasks, count, id);

    if (index == count) {
        printf("Task not found.\n");
        return 1;
    }

    result = read_line("Enter new description: ", description, sizeof(description));

    if (result == INPUT_EOF) return 0;

    if (result == INPUT_TOO_LONG) {
        printf("Description is too long.\n");
        return 1;
    }

    if (result != INPUT_OK) return 1;

    trim(description);

    if (strlen(description) == 0) {
        printf("Description cannot be empty.\n");
        return 1;
    }

    strcpy(tasks[index].description, description);

    printf("Task %d updated successfully.\n", id);
    return 1;
}

// Mark a task as completed
int complete_task(Task tasks[], size_t count) {
    int id;

    InputResult result = read_id(&id);

    if (result == INPUT_EOF) return 0;
    if (result != INPUT_OK) return 1;

    size_t index = find_task(tasks, count, id);

    if (index == count) {
        printf("Task not found.\n");
        return 1;
    }

    tasks[index].status = DONE;

    printf("Task %d marked as DONE.\n", id);
    return 1;
}

// Find and display one task
int find_and_display(Task tasks[], size_t count) {
    int id;

    InputResult result = read_id(&id);

    if (result == INPUT_EOF) return 0;
    if (result != INPUT_OK) return 1;

    size_t index = find_task(tasks, count, id);

    if (index == count) {
        printf("Task not found.\n");
    } else {
        display_task(tasks[index]);
    }

    return 1;
}

// Show available commands
void show_help(void) {
    printf("\nAvailable commands:\n");
    printf("ADD      - Add a new task\n");
    printf("DELETE   - Delete a task using its ID\n");
    printf("VIEW     - Display all tasks\n");
    printf("UPDATE   - Change a task description\n");
    printf("COMPLETE - Mark a task as done\n");
    printf("FIND     - Find a task using its ID\n");
    printf("HELP     - Show available commands\n");
    printf("EXIT     - Close the program\n");
}

int main(void) {
    Task *tasks = NULL;
    size_t count = 0;
    size_t capacity = 0;

    char input[100];
    CommandType command;

    printf("===== TASK MANAGER =====\n");
    printf("Type HELP to see available commands.\n");

    while (1) {
        InputResult result = read_line("\nEnter command: ", input, sizeof(input));

        if (result == INPUT_EOF) {
            printf("\nInput ended. Closing program.\n");
            break;
        }

        if (result == INPUT_TOO_LONG) {
            printf("Command is too long. Try again.\n");
            continue;
        }

        trim(input);
        command = get_command_type(input);

        switch (command) {
            case CMD_ADD:
                if (!add_task(&tasks, &count, &capacity)) goto end;
                break;

            case CMD_DELETE:
                if (!delete_task(tasks, &count)) goto end;
                break;

            case CMD_VIEW:
                view_tasks(tasks, count);
                break;

            case CMD_UPDATE:
                if (!update_task(tasks, count)) goto end;
                break;

            case CMD_COMPLETE:
                if (!complete_task(tasks, count)) goto end;
                break;

            case CMD_FIND:
                if (!find_and_display(tasks, count)) goto end;
                break;

            case CMD_HELP:
                show_help();
                break;

            case CMD_EXIT:
                printf("Exiting program.\n");
                goto end;

            default:
                printf("Unknown command. Type HELP to see available commands.\n");
        }
    }

end:
    free(tasks);
    printf("Memory released.\n");

    return 0;
}
