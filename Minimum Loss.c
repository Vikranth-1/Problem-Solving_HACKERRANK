#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);
long parse_long(char*);
typedef struct {
    long price;
    int index;
} House;

int compare(const void* a, const void* b) {
    House* h1 = (House*)a;
    House* h2 = (House*)b;
    return (h1->price > h2->price) - (h1->price < h2->price);
}

int minimumLoss(int price_count, long* price) {
    House* arr = malloc(price_count * sizeof(House));

    // Store price with original index
    for (int i = 0; i < price_count; i++) {
        arr[i].price = price[i];
        arr[i].index = i;
    }

    // Sort by price
    qsort(arr, price_count, sizeof(House), compare);

    long minLoss = LONG_MAX;

    // Check adjacent pairs
    for (int i = 1; i < price_count; i++) {
        long higher = arr[i].price;
        long lower = arr[i - 1].price;

        // Ensure buying happens before selling
        if (arr[i].index < arr[i - 1].index) {
            long loss = higher - lower;
            if (loss < minLoss) {
                minLoss = loss;
            }
        }
    }

    free(arr);
    return (int)minLoss;
}
int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int n = parse_int(ltrim(rtrim(readline())));

    char** price_temp = split_string(rtrim(readline()));

    long* price = malloc(n * sizeof(long));

    for (int i = 0; i < n; i++) {
        long price_item = parse_long(*(price_temp + i));

        *(price + i) = price_item;
    }

    int result = minimumLoss(n, price);

    fprintf(fptr, "%d\n", result);

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}

long parse_long(char* str) {
    char* endptr;
    long value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
