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

int parse_int(char*);
int alternate(char* s) {
    int len = strlen(s);
    int max_length = 0;
    
    for (char c1 = 'a'; c1 <= 'z'; c1++) {
        for (char c2 = c1 + 1; c2 <= 'z'; c2++) {
            char last = '\0';
            int count = 0;
            int valid = 1;
            int has_c1 = 0, has_c2 = 0;
            
            for (int i = 0; i < len; i++) {
                if (s[i] == c1 || s[i] == c2) {
                    if (s[i] == c1) has_c1 = 1;
                    if (s[i] == c2) has_c2 = 1;
                    
                    if (s[i] == last) {
                        valid = 0;
                        break;
                    }
                    last = s[i];
                    count++;
                }
            }
            
            if (valid && has_c1 && has_c2 && count > max_length) {
                max_length = count;
            }
        }
    }
    
    return max_length;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int l = parse_int(ltrim(rtrim(readline())));

    char* s = readline();

    int result = alternate(s);

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

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
