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

char* isValid(char* s) {
    int freq[26] = {0};
    int len = strlen(s);
    
    for (int i = 0; i < len; i++) {
        freq[s[i] - 'a']++;
    }
    
    int freq_count[100001] = {0};
    for (int i = 0; i < 26; i++) {
        if (freq[i] > 0) {
            freq_count[freq[i]]++;
        }
    }
    
    int distinct_freq = 0;
    int freq1 = 0, count1 = 0;
    int freq2 = 0, count2 = 0;
    
    for (int i = 1; i <= 100000; i++) {
        if (freq_count[i] > 0) {
            distinct_freq++;
            if (distinct_freq == 1) {
                freq1 = i;
                count1 = freq_count[i];
            } else if (distinct_freq == 2) {
                freq2 = i;
                count2 = freq_count[i];
            } else {
                char* result = malloc(3 * sizeof(char));
                strcpy(result, "NO");
                return result;
            }
        }
    }
    
    if (distinct_freq == 1) {
        char* result = malloc(4 * sizeof(char));
        strcpy(result, "YES");
        return result;
    }
    
    if ((count1 == 1 && (freq1 == 1 || freq1 - freq2 == 1)) ||
        (count2 == 1 && (freq2 == 1 || freq2 - freq1 == 1))) {
        char* result = malloc(4 * sizeof(char));
        strcpy(result, "YES");
        return result;
    }
    
    char* result = malloc(3 * sizeof(char));
    strcpy(result, "NO");
    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char* s = readline();

    char* result = isValid(s);

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
