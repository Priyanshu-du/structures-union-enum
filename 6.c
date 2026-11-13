
// Unions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>

typedef union {
    int i;
    float f;
    char str[20];
} UnionData;

typedef struct {
    int i;
    float f;
    char str[20];
} StructData;

// this tells us which value is stored
enum ValueType {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_TEXT
};

// union with a tag
typedef struct {
    UnionData data;
    enum ValueType type;
} TaggedValue;

// store integer value
int set_int(TaggedValue *value, int number) {

    if (value == NULL) {
        return 0;
    }

    value->data.i = number;
    value->type = VALUE_INT;

    return 1;
}

// store float value
int set_float(TaggedValue *value, float number) {

    if (value == NULL || !isfinite(number)) {
        return 0;
    }

    value->data.f = number;
    value->type = VALUE_FLOAT;

    return 1;
}

// store string value
int set_text(TaggedValue *value, const char *text) {

    if (value == NULL || text == NULL) {
        return 0;
    }

    if (strlen(text) >= sizeof(value->data.str)) {
        printf("Text is too long! Maximum 19 characters allowed.\n");
        return 0;
    }

    strcpy(value->data.str, text);
    value->type = VALUE_TEXT;

    return 1;
}

// print the value according to its type
void display_value(TaggedValue value) {

    switch (value.type) {

        case VALUE_INT:
            printf("Integer: %d\n", value.data.i);
            break;

        case VALUE_FLOAT:
            printf("Float: %.2f\n", value.data.f);
            break;

        case VALUE_TEXT:
            printf("Text: %s\n", value.data.str);
            break;

        default:
            printf("Invalid value type!\n");
    }
}

// show addresses of union and structure
void show_addresses(UnionData *udata, StructData *sdata) {

    printf("\n--- Union addresses ---\n");

    printf("Address of i:   %p\n", (void *)&udata->i);
    printf("Address of f:   %p\n", (void *)&udata->f);
    printf("Address of str: %p\n", (void *)&udata->str);

    printf("\n--- Structure addresses ---\n");

    printf("Address of i:   %p\n", (void *)&sdata->i);
    printf("Address of f:   %p\n", (void *)&sdata->f);
    printf("Address of str: %p\n", (void *)&sdata->str);
}

// check the memory layout
void memory_details(void) {

    printf("\n--- Size and Alignment ---\n");

    printf("Size of UnionData: %zu bytes\n", sizeof(UnionData));
    printf("Size of StructData: %zu bytes\n", sizeof(StructData));

    printf("Alignment of UnionData: %zu bytes\n", _Alignof(UnionData));
    printf("Alignment of StructData: %zu bytes\n", _Alignof(StructData));

    printf("\n--- Structure field offsets ---\n");

    printf("Offset of i: %zu bytes\n", offsetof(StructData, i));
    printf("Offset of f: %zu bytes\n", offsetof(StructData, f));
    printf("Offset of str: %zu bytes\n", offsetof(StructData, str));

    printf("\n--- Memory for 100 records ---\n");

    printf("100 structures: %zu bytes\n",
           100 * sizeof(StructData));

    printf("100 unions: %zu bytes\n",
           100 * sizeof(UnionData));

    printf("100 tagged records: %zu bytes\n",
           100 * sizeof(TaggedValue));

    printf("\n--- Tagged record size ---\n");

    printf("TaggedValue: %zu bytes\n", sizeof(TaggedValue));
}

// demonstrate changing the value in one record
void change_value_demo(void) {

    TaggedValue temp;

    printf("\n--- Changing values in one record ---\n");

    set_int(&temp, 10);
    display_value(temp);

    set_text(&temp, "Hello Union");
    display_value(temp);

    set_float(&temp, 220.5f);
    display_value(temp);
}

int main() {

    UnionData udata;
    StructData sdata;

    // putting all values in structure
    sdata.i = 10;
    sdata.f = 220.5f;
    strcpy(sdata.str, "Struct Programming");

    printf("--- Structure values ---\n");

    printf("Integer: %d\n", sdata.i);
    printf("Float: %.2f\n", sdata.f);
    printf("String: %s\n", sdata.str);

    // union can store one value at a time
    printf("\n--- Union values ---\n");

    udata.i = 10;
    printf("Integer: %d\n", udata.i);

    strcpy(udata.str, "Union Programming");
    printf("String: %s\n", udata.str);

    udata.f = 220.5f;
    printf("Float: %.2f\n", udata.f);

    // show addresses
    show_addresses(&udata, &sdata);

    // show size and offsets
    memory_details();

    // make five records with different values
    TaggedValue records[5];

    set_int(&records[0], 10);
    set_float(&records[1], 220.5f);
    set_text(&records[2], "Union Programming");
    set_int(&records[3], 50);
    set_text(&records[4], "C Programming");

    printf("\n--- Tagged records ---\n");

    for (int i = 0; i < 5; i++) {

        printf("Record %d: ", i + 1);
        display_value(records[i]);
    }

    // check maximum string length
    printf("\n--- String length test ---\n");

    TaggedValue test;

    if (set_text(&test, "1234567890123456789")) {
        printf("19 character string accepted: ");
        display_value(test);
    }

    set_text(&test, "12345678901234567890");

    // change the type of one record
    change_value_demo();

    // check invalid tag
    printf("\n--- Invalid tag test ---\n");

    TaggedValue invalid;
    invalid.type = (enum ValueType)99;

    display_value(invalid);

    return 0;
}
