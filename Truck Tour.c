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
 * Complete the 'truckTour' function below.
 */

int truckTour(int petrolpumps_rows, int petrolpumps_columns, int** petrolpumps) {

    int start = 0;
    int balance = 0;
    int deficit = 0;

    for(int i = 0; i < petrolpumps_rows; i++) {

        int petrol = petrolpumps[i][0];
        int distance = petrolpumps[i][1];

        balance += petrol - distance;

        if(balance < 0) {
            start = i + 1;
            deficit += balance;
            balance = 0;
        }
    }

    if(balance + deficit >= 0)
        return start;

    return -1;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int n = parse_int(ltrim(rtrim(readline())));

    int** petrolpumps = malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++) {
        *(petrolpumps + i) = malloc(2 * sizeof(int));

        char** petrolpumps_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < 2; j++) {
            int petrolpumps_item = parse_int(*(petrolpumps_item_temp + j));
            *(*(petrolpumps + i) + j) = petrolpumps_item;
        }
    }

    int result = truckTour(n, 2, petrolpumps);

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

        if (!line) break;

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
