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

char* superReducedString(char* s) {
    int len = strlen(s);
    char* stack = malloc((len + 1) * sizeof(char));
    int top = -1;
    
    for (int i = 0; i < len; i++) {
        if (top >= 0 && stack[top] == s[i]) {
            top--;
        } else {
            top++;
            stack[top] = s[i];
        }
    }
    
    if (top == -1) {
        free(stack);
        char* result = malloc(13 * sizeof(char));
        strcpy(result, "Empty String");
        return result;
    }
    
    char* result = malloc((top + 2) * sizeof(char));
    for (int i = 0; i <= top; i++) {
        result[i] = stack[i];
    }
    result[top + 1] = '\0';
    free(stack);
    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char* s = readline();

    char* result = superReducedString(s);

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
