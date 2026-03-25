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
int getTotalX(int a_count, int* a, int b_count, int* b) {
    int count = 0;
    
    // Find the LCM of all elements in array a
    int lcm_a = a[0];
    for (int i = 1; i < a_count; i++) {
        // Calculate LCM using formula: LCM(a,b) = (a*b) / GCD(a,b)
        int gcd_val = 1;
        int temp_lcm = lcm_a;
        int temp_a = a[i];
        
        // Calculate GCD
        while (temp_a != 0) {
            int temp = temp_a;
            temp_a = temp_lcm % temp_a;
            temp_lcm = temp;
        }
        gcd_val = temp_lcm;
        
        lcm_a = (lcm_a * a[i]) / gcd_val;
        
        // Early break if LCM exceeds the smallest element in b
        if (lcm_a > b[0]) {
            return 0;
        }
    }
    
    // Find the GCD of all elements in array b
    int gcd_b = b[0];
    for (int i = 1; i < b_count; i++) {
        int temp_gcd = gcd_b;
        int temp_b = b[i];
        
        // Calculate GCD
        while (temp_b != 0) {
            int temp = temp_b;
            temp_b = temp_gcd % temp_b;
            temp_gcd = temp;
        }
        gcd_b = temp_gcd;
    }
    
    // Check all multiples of lcm_a that divide gcd_b
    for (int x = lcm_a; x <= gcd_b; x += lcm_a) {
        if (gcd_b % x == 0) {
            count++;
        }
    }
    
    return count;
}
int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int m = parse_int(*(first_multiple_input + 1));

    char** arr_temp = split_string(rtrim(readline()));

    int* arr = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int arr_item = parse_int(*(arr_temp + i));

        *(arr + i) = arr_item;
    }

    char** brr_temp = split_string(rtrim(readline()));

    int* brr = malloc(m * sizeof(int));

    for (int i = 0; i < m; i++) {
        int brr_item = parse_int(*(brr_temp + i));

        *(brr + i) = brr_item;
    }

    int total = getTotalX(n, arr, m, brr);

    fprintf(fptr, "%d\n", total);

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
