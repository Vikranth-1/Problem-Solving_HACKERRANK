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

int compare(const void* a, const void* b) {
    return *(int*)b - *(int*)a;
}

int luckBalance(int k, int contests_rows, int contests_columns, int** contests) {
    int total_luck = 0;
    int important_count = 0;
    int* important_luck = malloc(contests_rows * sizeof(int));
    
    for (int i = 0; i < contests_rows; i++) {
        if (contests[i][1] == 1) {
            important_luck[important_count++] = contests[i][0];
        } else {
            total_luck += contests[i][0];
        }
    }
    
    qsort(important_luck, important_count, sizeof(int), compare);
    
    for (int i = 0; i < important_count; i++) {
        if (i < k) {
            total_luck += important_luck[i];
        } else {
            total_luck -= important_luck[i];
        }
    }
    
    free(important_luck);
    return total_luck;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int k = parse_int(*(first_multiple_input + 1));

    int** contests = malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++) {
        *(contests + i) = malloc(2 * (sizeof(int)));

        char** contests_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < 2; j++) {
            int contests_item = parse_int(*(contests_item_temp + j));

            *(*(contests + i) + j) = contests_item;
        }
    }

    int result = luckBalance(k, n, 2, contests);

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
