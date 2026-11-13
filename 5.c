
// Rectangle Management System

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>

#define MAX_CAPACITY 1000
#define TOLERANCE 0.00001f

typedef struct {
    int id;
    float width;
    float height;
    float area;
    float perimeter;
} Rectangle;



// calculating  area and perimeters
int calculate_geometry(Rectangle *r) {
    //struct variable for its field inside r->

    r->area = r->width * r->height;
    r->perimeter = 2.0f * (r->width + r->height);

    if (!isfinite(r->area) || !isfinite(r->perimeter)) {
        return 0;
    }

    return 1;
}



// make space for rectangles
Rectangle* create_rectangle_list(int n) {

    if (n <= 0 || n > MAX_CAPACITY) {
        return NULL;
    }

    Rectangle *rectangles = malloc((size_t)n * sizeof(Rectangle));

    return rectangles;
}

// read integer input properly
int read_int(const char *message, int *value) {

    char line[100];
    char *end;

    printf("%s", message);

    if (fgets(line, sizeof(line), stdin) == NULL) {
        return -1;
    }
    //file or input not yet reach end of strim

    if (strchr(line, '\n') == NULL && !feof(stdin)) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        printf("Input is too long!\n");
        return 0;
    }

    errno = 0;
    long number = strtol(line, &end, 10);

    while (isspace((unsigned char)*end)) {
        end++;
    }

    if (end == line || *end != '\0' ||
        errno == ERANGE || number < INT_MIN || number > INT_MAX) {
        printf("Invalid number!\n");
        return 0;
    }

    *value = (int)number;
    return 1;
}

// read decimal input
int read_float(const char *message, float *value) {

    char line[100];
    char *end;

    printf("%s", message);

    if (fgets(line, sizeof(line), stdin) == NULL) {
        return -1;
    }

    if (strchr(line, '\n') == NULL && !feof(stdin)) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        printf("Input is too long!\n");
        return 0;
    }

    errno = 0;
    float number = strtof(line, &end);

    while (isspace((unsigned char)*end)) {
        end++;
    }

    if (end == line || *end != '\0' ||
        errno == ERANGE || !isfinite(number)) {
        printf("Invalid dimension! Enter a finite number.\n");
        return 0;
    }

    *value = number;
    return 1;
}

// fill the rectangle details
int populate_rectangle(Rectangle *r, int id, float width, float height) {

    if (id <= 0 || !isfinite(width) || !isfinite(height) ||
        width <= 0.0f || height <= 0.0f) {
        return 0;
    }

    r->id = id;
    r->width = width;
    r->height = height;

    if (!calculate_geometry(r)) {
        return 0;
    }

    return 1;
}

// search rectangle using id
int search_rectangle(Rectangle *rectangles, int count, int id) {

    for (int i = 0; i < count; i++) {

        if (rectangles[i].id == id) {
            return i;
        }
    }

    return -1;
}

// take details of a new rectangle
int input_rectangle(Rectangle *r, Rectangle *rectangles, int count) {

    int id;
    float width, height;
    int status;

    while (1) {

        status = read_int("Enter rectangle ID: ", &id);

        if (status == -1) {
            return 0;
        }

        if (status == 0) {
            continue;
        }

        if (id <= 0) {
            printf("ID must be positive!\n");
            continue;
        }

        if (search_rectangle(rectangles, count, id) != -1) {
            printf("This ID already exists!\n");
            continue;
        }

        break;
    }

    while (1) {

        status = read_float("Enter width: ", &width);

        if (status == -1) {
            return 0;
        }

        if (status == 0) {
            continue;
        }

        if (width <= 0.0f) {
            printf("Width must be positive!\n");
            continue;
        }

        status = read_float("Enter height: ", &height);

        if (status == -1) {
            return 0;
        }

        if (status == 0) {
            continue;
        }

        if (height <= 0.0f) {
            printf("Height must be positive!\n");
            continue;
        }

        if (!populate_rectangle(r, id, width, height)) {
            printf("Area or perimeter is too large!\n");
            continue;
        }

        break;
    }

    return 1;
}

// add a new rectangle
void add_rectangle(Rectangle **rectangles, int *count, int *capacity) {

    Rectangle new_rectangle;

    if (!input_rectangle(&new_rectangle, *rectangles, *count)) {
        printf("Input ended. Rectangle not added.\n");
        return;
    }

    if (*count == *capacity) {

        if (*capacity >= MAX_CAPACITY) {
            printf("Maximum capacity reached!\n");
            return;
        }

        int new_capacity;

        if (*capacity > MAX_CAPACITY / 2) {
            new_capacity = MAX_CAPACITY;
        } else {
            new_capacity = *capacity * 2;
        }

        Rectangle *temp = realloc(
            *rectangles,
            (size_t)new_capacity * sizeof(Rectangle)
        );

        if (temp == NULL) {
            printf("Memory expansion failed!\n");
            return;
        }

        *rectangles = temp;
        *capacity = new_capacity;

        printf("Memory expanded successfully!\n");
    }

    (*rectangles)[*count] = new_rectangle;
    (*count)++;

    printf("Rectangle added successfully!\n");
}

// check whether it is square or not
int is_square(Rectangle r) {

    float difference = fabsf(r.width - r.height);
    float largest_dimension = fmaxf(r.width, r.height);

    // small difference is allowed because of float values
    return difference <= TOLERANCE * largest_dimension;
}

// display all rectangles
void display_rectangles(Rectangle *rectangles, int count) {

    if (count == 0) {
        printf("No rectangles available!\n");
        return;
    }

    printf("\nID\tWidth\tHeight\tArea\tPerimeter\tType\n");

    for (int i = 0; i < count; i++) {

        printf("%d\t%.2f\t%.2f\t%.2f\t%.2f\t\t%s\n",
               rectangles[i].id,
               rectangles[i].width,
               rectangles[i].height,
               rectangles[i].area,
               rectangles[i].perimeter,
               is_square(rectangles[i]) ? "Square" : "Non-square");
    }
}

// search and show one rectangle
void find_rectangle(Rectangle *rectangles, int count) {

    int id;
    int status;

    status = read_int("Enter rectangle ID to search: ", &id);

    if (status != 1) {
        printf("Invalid input!\n");
        return;
    }

    int index = search_rectangle(rectangles, count, id);

    if (index == -1) {
        printf("Rectangle not found!\n");
        return;
    }

    printf("ID: %d\n", rectangles[index].id);
    printf("Width: %.2f\n", rectangles[index].width);
    printf("Height: %.2f\n", rectangles[index].height);
    printf("Area: %.2f\n", rectangles[index].area);
    printf("Perimeter: %.2f\n", rectangles[index].perimeter);
    printf("Type: %s\n",
           is_square(rectangles[index]) ? "Square" : "Non-square");
}

// update dimensions of a rectangle
void update_rectangle(Rectangle *rectangles, int count) {

    int id;
    int status;
    float width, height;

    status = read_int("Enter rectangle ID to update: ", &id);

    if (status != 1) {
        printf("Invalid input!\n");
        return;
    }

    int index = search_rectangle(rectangles, count, id);

    if (index == -1) {
        printf("Rectangle not found!\n");
        return;
    }

    while (1) {

        status = read_float("Enter new width: ", &width);

        if (status == -1) {
            return;
        }

        if (status == 0) {
            continue;
        }

        if (width <= 0.0f) {
            printf("Width must be positive!\n");
            continue;
        }

        status = read_float("Enter new height: ", &height);

        if (status == -1) {
            return;
        }

        if (status == 0) {
            continue;
        }

        if (height <= 0.0f) {
            printf("Height must be positive!\n");
            continue;
        }

        Rectangle temp;

        if (!populate_rectangle(&temp, id, width, height)) {
            printf("Area or perimeter is too large!\n");
            continue;
        }

        rectangles[index] = temp;
        break;
    }

    printf("Rectangle updated successfully!\n");
}

// delete a rectangle
void delete_rectangle(Rectangle *rectangles, int *count) {

    int id;
    int status;

    status = read_int("Enter rectangle ID to delete: ", &id);

    if (status != 1) {
        printf("Invalid input!\n");
        return;
    }

    int index = search_rectangle(rectangles, *count, id);

    if (index == -1) {
        printf("Rectangle not found!\n");
        return;
    }

    for (int i = index; i < *count - 1; i++) {
        rectangles[i] = rectangles[i + 1];
    }

    (*count)--;

    printf("Rectangle deleted successfully!\n");
}

// find largest, smallest and total area
void area_statistics(Rectangle *rectangles, int count) {

    if (count == 0) {
        printf("No rectangles available!\n");
        return;
    }

    float largest = rectangles[0].area;
    float smallest = rectangles[0].area;
    double total = 0.0;

    int largest_index = 0;
    int smallest_index = 0;

    for (int i = 0; i < count; i++) {

        if (rectangles[i].area > largest) {
            largest = rectangles[i].area;
            largest_index = i;
        }

        if (rectangles[i].area < smallest) {
            smallest = rectangles[i].area;
            smallest_index = i;
        }

        total += rectangles[i].area;
    }

    printf("Largest area: %.2f (ID: %d)\n",
           largest, rectangles[largest_index].id);

    printf("Smallest area: %.2f (ID: %d)\n",
           smallest, rectangles[smallest_index].id);

    printf("Total area: %.2f\n", total);
    printf("Average area: %.2f\n", total / count);
}

// arrange rectangles according to area
void sort_rectangles(Rectangle *rectangles, int count) {

    for (int i = 0; i < count - 1; i++) {

        for (int j = 0; j < count - i - 1; j++) {

            if (rectangles[j].area > rectangles[j + 1].area ||
                (rectangles[j].area == rectangles[j + 1].area &&
                 rectangles[j].id > rectangles[j + 1].id)) {

                Rectangle temp = rectangles[j];
                rectangles[j] = rectangles[j + 1];
                rectangles[j + 1] = temp;
            }
        }
    }

    printf("Rectangles sorted successfully!\n");
}

// show rectangles above a particular area
void filter_rectangles(Rectangle *rectangles, int count) {

    float threshold;
    int status;
    int found = 0;

    status = read_float("Enter area threshold: ", &threshold);

    if (status != 1) {
        printf("Invalid threshold!\n");
        return;
    }

    printf("\nRectangles with area greater than %.2f:\n", threshold);

    for (int i = 0; i < count; i++) {

        if (rectangles[i].area > threshold) {

            printf("ID: %d | Width: %.2f | Height: %.2f | Area: %.2f\n",
                   rectangles[i].id,
                   rectangles[i].width,
                   rectangles[i].height,
                   rectangles[i].area);

            found = 1;
        }
    }

    if (!found) {
        printf("No matching rectangles found!\n");
    }
}

// free the memory
void free_rectangle_list(Rectangle *rectangles) {
    free(rectangles);
}

int main() {

    int count;
    int capacity;
    int choice;

    while (1) {

        int status = read_int("Enter number of rectangles (1-1000): ", &count);

        if (status == -1) {
            return 1;
        }

        if (status == 1 && count >= 1 && count <= MAX_CAPACITY) {
            break;
        }

        printf("Enter a number between 1 and %d!\n", MAX_CAPACITY);
    }

    capacity = count;

    Rectangle *rectangles = create_rectangle_list(capacity);

    if (rectangles == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // enter initial rectangle details
    for (int i = 0; i < count; i++) {

        printf("\nRectangle %d details:\n", i + 1);

        if (!input_rectangle(&rectangles[i], rectangles, i)) {
            free_rectangle_list(rectangles);
            return 1;
        }
    }

    while (1) {

        printf("\n===== RECTANGLE MANAGEMENT SYSTEM =====\n");
        printf("1. Add Rectangle\n");
        printf("2. Display Rectangles\n");
        printf("3. Search Rectangle\n");
        printf("4. Update Rectangle\n");
        printf("5. Delete Rectangle\n");
        printf("6. Area Statistics\n");
        printf("7. Sort Rectangles\n");
        printf("8. Filter by Area\n");
        printf("9. Exit\n");

        status:
        {
            int result = read_int("Enter your choice: ", &choice);

            if (result == -1) {
                break;
            }

            if (result == 0) {
                goto status;
            }
        }

        switch (choice) {

            case 1:
                add_rectangle(&rectangles, &count, &capacity);
                break;

            case 2:
                display_rectangles(rectangles, count);
                break;

            case 3:
                find_rectangle(rectangles, count);
                break;

            case 4:
                update_rectangle(rectangles, count);
                break;

            case 5:
                delete_rectangle(rectangles, &count);
                break;

            case 6:
                area_statistics(rectangles, count);
                break;

            case 7:
                sort_rectangles(rectangles, count);
                break;

            case 8:
                filter_rectangles(rectangles, count);
                break;

            case 9:
                free_rectangle_list(rectangles);
                printf("Memory released successfully!\n");
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }

    free_rectangle_list(rectangles);
    return 0;
}
