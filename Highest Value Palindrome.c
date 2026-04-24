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

char* highestValuePalindrome(char* s, int n, int k) {
    char* result = malloc((n + 1) * sizeof(char));
    strcpy(result, s);
    
    int* diff = calloc(n, sizeof(int));
    int changes_needed = 0;
    
    for (int i = 0; i < n / 2; i++) {
        if (result[i] != result[n - 1 - i]) {
            diff[i] = 1;
            changes_needed++;
        }
    }
    
    if (changes_needed > k) {
        free(diff);
        free(result);
        char* no_answer = malloc(3 * sizeof(char));
        strcpy(no_answer, "-1");
        return no_answer;
    }
    
    int remaining = k - changes_needed;
    
    for (int i = 0; i < n / 2; i++) {
        if (result[i] != result[n - 1 - i]) {
            char max_char = result[i] > result[n - 1 - i] ? result[i] : result[n - 1 - i];
            if (max_char != '9' && remaining > 0) {
                result[i] = '9';
                result[n - 1 - i] = '9';
                remaining--;
            } else {
                result[i] = max_char;
                result[n - 1 - i] = max_char;
            }
        } else if (result[i] != '9' && remaining >= 2) {
            result[i] = '9';
            result[n - 1 - i] = '9';
            remaining -= 2;
        }
    }
    
    if (remaining > 0 && n % 2 == 1) {
        result[n / 2] = '9';
    }
    
    free(diff);
    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int k = parse_int(*(first_multiple_input + 1));

    char* s = readline();

    char* result = highestValuePalindrome(s, n, k);

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
