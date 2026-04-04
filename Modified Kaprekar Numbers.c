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
void kaprekarNumbers(int p, int q) {
    int found = 0;
    
    for (long long i = p; i <= q; i++) {
        long long square = i * i;
        
        char str[20];
        sprintf(str, "%lld", square);
        
        int len = strlen(str);
        int d = 0;
        long long temp = i;
        while (temp > 0) {
            d++;
            temp /= 10;
        }
        if (i == 0) d = 1;
        
        int r_len = d;
        int l_len = len - r_len;
        
        long long r = 0;
        long long l = 0;
        
        if (r_len > 0) {
            char r_str[20];
            strncpy(r_str, str + l_len, r_len);
            r_str[r_len] = '\0';
            r = atoll(r_str);
        }
        
        if (l_len > 0) {
            char l_str[20];
            strncpy(l_str, str, l_len);
            l_str[l_len] = '\0';
            l = atoll(l_str);
        }
        
        if (l + r == i) {
            if (found) printf(" ");
            printf("%lld", i);
            found = 1;
        }
    }
    
    if (!found) {
        printf("INVALID RANGE");
    }
    printf("\n");
}

int main()
{
    int p = parse_int(ltrim(rtrim(readline())));

    int q = parse_int(ltrim(rtrim(readline())));

    kaprekarNumbers(p, q);

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
