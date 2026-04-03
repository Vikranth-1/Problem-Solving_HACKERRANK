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
 * Complete the 'organizingContainers' function below.
 *
 * The function is expected to return a STRING.
 * The function accepts 2D_INTEGER_ARRAY container as parameter.
 */

int compare_long_long(const void* a, const void* b) {
    long long la = *(const long long*)a;
    long long lb = *(const long long*)b;
    return (la > lb) - (la < lb);
}

char* organizingContainers(int container_rows, int container_columns, int** container) {
    // Allocate arrays for row sums and column sums
    long long* row_sums = (long long*)malloc(container_rows * sizeof(long long));
    long long* col_sums = (long long*)malloc(container_columns * sizeof(long long));
    
    // Initialize sums to 0
    for (int i = 0; i < container_rows; i++) {
        row_sums[i] = 0;
    }
    for (int j = 0; j < container_columns; j++) {
        col_sums[j] = 0;
    }
    
    // Calculate row sums and column sums
    for (int i = 0; i < container_rows; i++) {
        for (int j = 0; j < container_columns; j++) {
            row_sums[i] += container[i][j];
            col_sums[j] += container[i][j];
        }
    }
    
    // Sort the sums using qsort
    qsort(row_sums, container_rows, sizeof(long long), compare_long_long);
    qsort(col_sums, container_columns, sizeof(long long), compare_long_long);
    
    // Compare sorted sums
    char* result = (char*)malloc(20 * sizeof(char)); // Allocate enough space
    
    for (int i = 0; i < container_rows; i++) {
        if (row_sums[i] != col_sums[i]) {
            strcpy(result, "Impossible");
            free(row_sums);
            free(col_sums);
            return result;
        }
    }
    
    strcpy(result, "Possible");
    free(row_sums);
    free(col_sums);
    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int q = parse_int(ltrim(rtrim(readline())));

    for (int q_itr = 0; q_itr < q; q_itr++) {
        int n = parse_int(ltrim(rtrim(readline())));

        int** container = malloc(n * sizeof(int*));

        for (int i = 0; i < n; i++) {
            *(container + i) = malloc(n * (sizeof(int)));

            char** container_item_temp = split_string(rtrim(readline()));

            for (int j = 0; j < n; j++) {
                int container_item = parse_int(*(container_item_temp + j));

                *(*(container + i) + j) = container_item;
            }
        }

        char* result = organizingContainers(n, n, container);

        fprintf(fptr, "%s\n", result);
        
        // Free allocated memory
        for (int i = 0; i < n; i++) {
            free(container[i]);
        }
        free(container);
        free(result);
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
