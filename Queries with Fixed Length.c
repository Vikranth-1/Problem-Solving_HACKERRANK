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

int* solve(int arr_count, int* arr, int queries_count, int* queries, int* result_count) {

    *result_count = queries_count;
    int* result = malloc(queries_count * sizeof(int));

    for(int qi = 0; qi < queries_count; qi++) {

        int d = queries[qi];
        int* deque = malloc(arr_count * sizeof(int));
        int front = 0, rear = -1;

        int min_of_max = INT_MAX;

        for(int i = 0; i < arr_count; i++) {

            while(front <= rear && deque[front] <= i - d)
                front++;

            while(front <= rear && arr[deque[rear]] <= arr[i])
                rear--;

            deque[++rear] = i;

            if(i >= d - 1) {
                int current_max = arr[deque[front]];
                if(current_max < min_of_max)
                    min_of_max = current_max;
            }
        }

        result[qi] = min_of_max;
        free(deque);
    }

    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));
    int q = parse_int(*(first_multiple_input + 1));

    char** arr_temp = split_string(rtrim(readline()));
    int* arr = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        arr[i] = parse_int(*(arr_temp + i));
    }

    int* queries = malloc(q * sizeof(int));

    for (int i = 0; i < q; i++) {
        queries[i] = parse_int(ltrim(rtrim(readline())));
    }

    int result_count;
    int* result = solve(n, arr, q, queries, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", result[i]);
        if (i != result_count - 1)
            fprintf(fptr, "\n");
    }

    fprintf(fptr, "\n");

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

        if (!line)
            break;

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n')
            break;

        alloc_length <<= 1;
        data = realloc(data, alloc_length);
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);
        data[data_length] = '\0';
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) return '\0';
    while (*str && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str) return '\0';

    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;

    *(end + 1) = '\0';
    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");
    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);
        splits[spaces - 1] = token;
        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0')
        exit(EXIT_FAILURE);

    return value;
}
