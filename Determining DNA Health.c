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

int main() {
    int n = parse_int(ltrim(rtrim(readline())));
    
    char** genes_temp = split_string(rtrim(readline()));
    char** genes = malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        char* genes_item = *(genes_temp + i);
        *(genes + i) = genes_item;
    }
    
    char** health_temp = split_string(rtrim(readline()));
    int* health = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        int health_item = parse_int(*(health_temp + i));
        *(health + i) = health_item;
    }
    
    int s = parse_int(ltrim(rtrim(readline())));
    
    long long min_health = LLONG_MAX;
    long long max_health = LLONG_MIN;
    
    for (int s_itr = 0; s_itr < s; s_itr++) {
        char** first_multiple_input = split_string(rtrim(readline()));
        
        int first = parse_int(*(first_multiple_input + 0));
        int last = parse_int(*(first_multiple_input + 1));
        char* d = *(first_multiple_input + 2);
        
        long long total_health = 0;
        int d_len = strlen(d);
        
        for (int i = first; i <= last; i++) {
            int gene_len = strlen(genes[i]);
            for (int j = 0; j <= d_len - gene_len; j++) {
                if (strncmp(d + j, genes[i], gene_len) == 0) {
                    total_health += health[i];
                }
            }
        }
        
        if (total_health < min_health) {
            min_health = total_health;
        }
        if (total_health > max_health) {
            max_health = total_health;
        }
        
        free(first_multiple_input);
    }
    
    printf("%lld %lld\n", min_health, max_health);
    
    free(genes);
    free(health);
    free(genes_temp);
    free(health_temp);
    
    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);
    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);
        if (!line) break;
        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;
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
        if (!data) data = '\0';
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
    if (!str) return '\0';
    if (!*str) return str;
    while (*str != '\0' && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str) return '\0';
    if (!*str) return str;
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");
    int spaces = 0;
    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);
        if (!splits) return splits;
        splits[spaces - 1] = token;
        token = strtok(NULL, " ");
    }
    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') exit(EXIT_FAILURE);
    return value;
}
