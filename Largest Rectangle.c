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

/*
 * Complete the 'largestRectangle' function below.
 */

long largestRectangle(int h_count, int* h) {

    int *stack = malloc(sizeof(int) * (h_count + 1));
    int top = -1;

    long maxArea = 0;
    int i = 0;

    while (i < h_count) {

        if (top == -1 || h[stack[top]] <= h[i]) {
            stack[++top] = i++;
        } else {
            int height = h[stack[top--]];
            int width;

            if (top == -1)
                width = i;
            else
                width = i - stack[top] - 1;

            long area = (long)height * width;

            if (area > maxArea)
                maxArea = area;
        }
    }

    while (top != -1) {
        int height = h[stack[top--]];
        int width;

        if (top == -1)
            width = i;
        else
            width = i - stack[top] - 1;

        long area = (long)height * width;

        if (area > maxArea)
            maxArea = area;
    }

    free(stack);
    return maxArea;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int n = parse_int(ltrim(rtrim(readline())));

    char** h_temp = split_string(rtrim(readline()));

    int* h = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int h_item = parse_int(*(h_temp + i));
        *(h + i) = h_item;
    }

    long result = largestRectangle(n, h);

    fprintf(fptr, "%ld\n", result);

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

        if (!data)
            break;
    }

    if (data[data_length - 1] == '\n')
        data[data_length - 1] = '\0';

    return data;
}

char* ltrim(char* str) {
    if (!str) return NULL;

    while (*str != '\0' && isspace(*str))
        str++;

    return str;
}

char* rtrim(char* str) {
    if (!str) return NULL;

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end))
        end--;

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
