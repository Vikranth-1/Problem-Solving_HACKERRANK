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

char** bomberMan(int n, int grid_count, char** grid, int* result_count) {
    int r = grid_count;
    int c = strlen(grid[0]);
    
    *result_count = r;
    char** result = malloc(r * sizeof(char*));
    
    if (n == 1) {
        for (int i = 0; i < r; i++) {
            result[i] = malloc((c + 1) * sizeof(char));
            strcpy(result[i], grid[i]);
        }
        return result;
    }
    
    if (n % 2 == 0) {
        for (int i = 0; i < r; i++) {
            result[i] = malloc((c + 1) * sizeof(char));
            for (int j = 0; j < c; j++) {
                result[i][j] = 'O';
            }
            result[i][c] = '\0';
        }
        return result;
    }
    
    int iterations = (n % 4 == 3) ? 1 : 2;
    
    for (int iter = 0; iter < iterations; iter++) {
        char** temp = malloc(r * sizeof(char*));
        for (int i = 0; i < r; i++) {
            temp[i] = malloc((c + 1) * sizeof(char));
            for (int j = 0; j < c; j++) {
                temp[i][j] = 'O';
            }
            temp[i][c] = '\0';
        }
        
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if (grid[i][j] == 'O') {
                    temp[i][j] = '.';
                    if (i > 0) temp[i-1][j] = '.';
                    if (i < r-1) temp[i+1][j] = '.';
                    if (j > 0) temp[i][j-1] = '.';
                    if (j < c-1) temp[i][j+1] = '.';
                }
            }
        }
        
        for (int i = 0; i < r; i++) {
            grid[i] = temp[i];
        }
    }
    
    for (int i = 0; i < r; i++) {
        result[i] = malloc((c + 1) * sizeof(char));
        strcpy(result[i], grid[i]);
    }
    
    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int r = parse_int(*(first_multiple_input + 0));

    int c = parse_int(*(first_multiple_input + 1));

    int n = parse_int(*(first_multiple_input + 2));

    char** grid = malloc(r * sizeof(char*));

    for (int i = 0; i < r; i++) {
        char* grid_item = readline();

        *(grid + i) = grid_item;
    }

    int result_count;
    char** result = bomberMan(n, r, grid, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%s", *(result + i));

        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
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
