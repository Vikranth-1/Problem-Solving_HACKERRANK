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

int boardCutting(int cost_y_count, int* cost_y, int cost_x_count, int* cost_x) {
    qsort(cost_y, cost_y_count, sizeof(int), compare);
    qsort(cost_x, cost_x_count, sizeof(int), compare);
    
    long long total_cost = 0;
    int horizontal_segments = 1;
    int vertical_segments = 1;
    int i = 0, j = 0;
    
    while (i < cost_y_count && j < cost_x_count) {
        if (cost_y[i] >= cost_x[j]) {
            total_cost += (long long)cost_y[i] * vertical_segments;
            horizontal_segments++;
            i++;
        } else {
            total_cost += (long long)cost_x[j] * horizontal_segments;
            vertical_segments++;
            j++;
        }
    }
    
    while (i < cost_y_count) {
        total_cost += (long long)cost_y[i] * vertical_segments;
        i++;
    }
    
    while (j < cost_x_count) {
        total_cost += (long long)cost_x[j] * horizontal_segments;
        j++;
    }
    
    return total_cost % 1000000007;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int q = parse_int(ltrim(rtrim(readline())));

    for (int q_itr = 0; q_itr < q; q_itr++) {
        char** first_multiple_input = split_string(rtrim(readline()));

        int m = parse_int(*(first_multiple_input + 0));

        int n = parse_int(*(first_multiple_input + 1));

        char** cost_y_temp = split_string(rtrim(readline()));

        int* cost_y = malloc((m - 1) * sizeof(int));

        for (int i = 0; i < m - 1; i++) {
            int cost_y_item = parse_int(*(cost_y_temp + i));

            *(cost_y + i) = cost_y_item;
        }

        char** cost_x_temp = split_string(rtrim(readline()));

        int* cost_x = malloc((n - 1) * sizeof(int));

        for (int i = 0; i < n - 1; i++) {
            int cost_x_item = parse_int(*(cost_x_temp + i));

            *(cost_x + i) = cost_x_item;
        }

        int result = boardCutting(m - 1, cost_y, n - 1, cost_x);

        fprintf(fptr, "%d\n", result);
    }

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
