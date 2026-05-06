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

int dfs(int matrix_rows, int matrix_columns, int** matrix, int i, int j) {
    if (i < 0 || i >= matrix_rows || j < 0 || j >= matrix_columns || matrix[i][j] != 1) {
        return 0;
    }
    
    matrix[i][j] = -1;
    int count = 1;
    
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue;
            count += dfs(matrix_rows, matrix_columns, matrix, i + di, j + dj);
        }
    }
    
    return count;
}

int connectedCell(int matrix_rows, int matrix_columns, int** matrix) {
    int max_region = 0;
    
    for (int i = 0; i < matrix_rows; i++) {
        for (int j = 0; j < matrix_columns; j++) {
            if (matrix[i][j] == 1) {
                int region_size = dfs(matrix_rows, matrix_columns, matrix, i, j);
                if (region_size > max_region) {
                    max_region = region_size;
                }
            }
        }
    }
    
    return max_region;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int n = parse_int(ltrim(rtrim(readline())));

    int m = parse_int(ltrim(rtrim(readline())));

    int** matrix = malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++) {
        *(matrix + i) = malloc(m * (sizeof(int)));

        char** matrix_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < m; j++) {
            int matrix_item = parse_int(*(matrix_item_temp + j));

            *(*(matrix + i) + j) = matrix_item;
        }
    }

    int result = connectedCell(n, m, matrix);

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
