
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Type of value stored in container
typedef enum {
    TYPE_EMPTY,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_STRING
} DataType;

// Stores one value at a time
typedef union {
    int int_value;
    float float_value;
    char char_value;
    char *string_value;
} Data;

// Container stores data and its type
typedef struct {
    Data data;
    DataType type;
} Container;

// Function declarations
void container_init(Container *c);
void container_clear(Container *c);

int container_set_int(Container *c, int value);
int container_set_float(Container *c, float value);
int container_set_char(Container *c, char value);
int container_set_string(Container *c, const char *value);

int container_get_int(const Container *c, int *out_value);
int container_get_float(const Container *c, float *out_value);
int container_get_char(const Container *c, char *out_value);
int container_get_string(const Container *c, const char **out_value);

void container_print(const Container *c);
const char *container_type_name(const Container *c);

int container_copy(Container *destination, const Container *source);


// Start with an empty container
void container_init(Container *c) {
    if (c == NULL) return;

    c->type = TYPE_EMPTY;
    c->data.int_value = 0;
}

// Free string memory and make container empty
void container_clear(Container *c) {
    if (c == NULL) return;

    if (c->type == TYPE_STRING) {
        free(c->data.string_value);
    }

    c->type = TYPE_EMPTY;
    c->data.int_value = 0;
}

// Store an integer
int container_set_int(Container *c, int value) {
    if (c == NULL) return 0;

    container_clear(c);

    c->data.int_value = value;
    c->type = TYPE_INT;

    return 1;
}

// Store a float
int container_set_float(Container *c, float value) {
    if (c == NULL) return 0;

    container_clear(c);

    c->data.float_value = value;
    c->type = TYPE_FLOAT;

    return 1;
}

// Store a character
int container_set_char(Container *c, char value) {
    if (c == NULL) return 0;

    container_clear(c);

    c->data.char_value = value;
    c->type = TYPE_CHAR;

    return 1;
}

// Store a separate copy of the string
int container_set_string(Container *c, const char *value) {
    if (c == NULL || value == NULL) return 0;

    size_t length = strlen(value);

    char *new_string = malloc(length + 1);

    if (new_string == NULL) {
        printf("Memory allocation failed.\n");
        return 0;
    }

    memcpy(new_string, value, length + 1);

    // Clear old value only after new memory is ready
    container_clear(c);

    c->data.string_value = new_string;
    c->type = TYPE_STRING;

    return 1;
}

// Get integer value
int container_get_int(const Container *c, int *out_value) {
    if (c == NULL || out_value == NULL || c->type != TYPE_INT) {
        return 0;
    }

    *out_value = c->data.int_value;
    return 1;
}

// Get float value
int container_get_float(const Container *c, float *out_value) {
    if (c == NULL || out_value == NULL || c->type != TYPE_FLOAT) {
        return 0;
    }

    *out_value = c->data.float_value;
    return 1;
}

// Get character value
int container_get_char(const Container *c, char *out_value) {
    if (c == NULL || out_value == NULL || c->type != TYPE_CHAR) {
        return 0;
    }

    *out_value = c->data.char_value;
    return 1;
}

// Get string value
int container_get_string(const Container *c, const char **out_value) {
    if (c == NULL || out_value == NULL || c->type != TYPE_STRING) {
        return 0;
    }

    *out_value = c->data.string_value;
    return 1;
}

// Return the type name
const char *container_type_name(const Container *c) {
    if (c == NULL) return "INVALID";

    switch (c->type) {
        case TYPE_EMPTY:  return "EMPTY";
        case TYPE_INT:    return "INT";
        case TYPE_FLOAT:  return "FLOAT";
        case TYPE_CHAR:   return "CHAR";
        case TYPE_STRING: return "STRING";
        default:          return "INVALID";
    }
}

// Print the current value
void container_print(const Container *c) {
    if (c == NULL) {
        printf("Invalid container.\n");
        return;
    }

    switch (c->type) {
        case TYPE_EMPTY:
            printf("Empty container");
            break;

        case TYPE_INT:
            printf("%d", c->data.int_value);
            break;

        case TYPE_FLOAT:
            printf("%.2f", c->data.float_value);
            break;

        case TYPE_CHAR:
            printf("%c", c->data.char_value);
            break;

        case TYPE_STRING:
            printf("%s", c->data.string_value);
            break;

        default:
            printf("Invalid type");
    }
}

// Copy one container into another
int container_copy(Container *destination, const Container *source) {
    if (destination == NULL || source == NULL) return 0;

    // Copying a container onto itself changes nothing
    if (destination == source) return 1;

    Container temp;
    container_init(&temp);

    switch (source->type) {
        case TYPE_EMPTY:
            break;

        case TYPE_INT:
            temp.data.int_value = source->data.int_value;
            temp.type = TYPE_INT;
            break;

        case TYPE_FLOAT:
            temp.data.float_value = source->data.float_value;
            temp.type = TYPE_FLOAT;
            break;

        case TYPE_CHAR:
            temp.data.char_value = source->data.char_value;
            temp.type = TYPE_CHAR;
            break;

        case TYPE_STRING:
            if (!container_set_string(&temp, source->data.string_value)) {
                return 0;
            }
            break;

        default:
            return 0;
    }

    // Replace destination only after the copy succeeds
    container_clear(destination);
    *destination = temp;

    return 1;
}


int main(void) {
    Container records[6];
    Container copied;

    int int_value;
    float float_value;
    char char_value;
    const char *string_value;

    // Initialize all containers
    for (int i = 0; i < 6; i++) {
        container_init(&records[i]);
    }

    container_init(&copied);

    // Store different types of values
    container_set_int(&records[0], 10);
    container_set_float(&records[1], 3.14f);
    container_set_char(&records[2], 'A');
    container_set_string(&records[3], "Hello");
    container_set_int(&records[4], 0);
    container_set_string(&records[5], "Data");

    printf("----- Container Records -----\n");

    for (int i = 0; i < 6; i++) {
        printf("Index %d | Type: %s | Value: ",
               i, container_type_name(&records[i]));

        container_print(&records[i]);
        printf("\n");
    }

    // Copy the last string container
    printf("\n----- Deep Copy Test -----\n");

    if (container_copy(&copied, &records[5])) {
        printf("Copied value: ");
        container_print(&copied);
        printf("\n");
    }

    // Change original string
    container_set_string(&records[5], "Changed");

    printf("Original after change: ");
    container_print(&records[5]);

    printf("\nCopied value after change: ");
    container_print(&copied);
    printf("\n");

    // Change the same container between different types
    printf("\n----- Type Transition Test -----\n");

    container_set_string(&records[0], "First String");

    printf("STRING: ");
    container_print(&records[0]);

    container_set_int(&records[0], 25);

    printf("\nINT: ");
    container_print(&records[0]);

    container_set_string(&records[0], "Second String");

    printf("\nSTRING: ");
    container_print(&records[0]);

    container_set_float(&records[0], 9.5f);

    printf("\nFLOAT: ");
    container_print(&records[0]);
    printf("\n");

    // Test getters
    printf("\n----- Getter Tests -----\n");

    if (container_get_int(&records[4], &int_value)) {
        printf("Integer value: %d\n", int_value);
    }

    if (!container_get_float(&records[4], &float_value)) {
        printf("Wrong type: records[4] does not contain a float.\n");
    }

    if (container_get_float(&records[1], &float_value)) {
        printf("Float value: %.2f\n", float_value);
    }

    if (container_get_char(&records[2], &char_value)) {
        printf("Character value: %c\n", char_value);
    }

    if (container_get_string(&records[3], &string_value)) {
        printf("String value: %s\n", string_value);
    }

    // Test empty string
    printf("\n----- Empty String Test -----\n");

    if (container_set_string(&records[3], "")) {
        printf("Empty string stored successfully.\n");
        container_print(&records[3]);
        printf("\n");
    }

    // Test copying a container onto itself
    if (container_copy(&records[1], &records[1])) {
        printf("Self-copy test successful.\n");
    }

    // Test clearing the same container twice
    printf("\n----- Clear Test -----\n");

    container_clear(&records[3]);
    container_clear(&records[3]);

    printf("After clearing: %s\n",
           container_type_name(&records[3]));

    // Release all allocated memory
    for (int i = 0; i < 6; i++) {
        container_clear(&records[i]);
    }

    container_clear(&copied);

    printf("\nAll containers cleared successfully.\n");

    return 0;
}
