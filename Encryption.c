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

char* encryption(char* s) {
    int len = strlen(s);
    
    char* clean = malloc(len + 1);
    int k = 0;
    
    for (int i = 0; i < len; i++) {
        if (s[i] != ' ') {
            clean[k++] = s[i];
        }
    }
    clean[k] = '\0';
    
    int L = k;
    
    int rows = floor(sqrt(L));
    int cols = ceil(sqrt(L));
    
    if (rows * cols < L) rows++;
    
    char* result = malloc((L + cols + 1) * sizeof(char));
    int idx = 0;
    
    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            int pos = r * cols + c;
            if (pos < L) {
                result[idx++] = clean[pos];
            }
        }
        if (c != cols - 1) {
            result[idx++] = ' ';
        }
    }
    
    result[idx] = '\0';
    
    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char* s = readline();

    char* result = encryption(s);

    fprintf(fptr, "%s\n", result);

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
