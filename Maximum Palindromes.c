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

#define MOD 1000000007
#define MAX_N 100005
#define MAX_CHAR 26

long long fact[MAX_N], inv_fact[MAX_N];
int prefix_count[MAX_N][MAX_CHAR];

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);
int parse_int(char*);

long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

void initialize(char* s) {
    int len = strlen(s);
    
    fact[0] = 1;
    for (int i = 1; i <= len; i++) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
    
    inv_fact[len] = mod_pow(fact[len], MOD - 2, MOD);
    for (int i = len - 1; i >= 0; i--) {
        inv_fact[i] = (inv_fact[i + 1] * (i + 1)) % MOD;
    }
    
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < MAX_CHAR; j++) {
            prefix_count[i + 1][j] = prefix_count[i][j];
        }
        prefix_count[i + 1][s[i] - 'a']++;
    }
}

int answerQuery(int l, int r) {
    int odd_count = 0;
    int total_pairs = 0;
    int char_counts[MAX_CHAR];
    
    for (int i = 0; i < MAX_CHAR; i++) {
        char_counts[i] = prefix_count[r][i] - prefix_count[l - 1][i];
        if (char_counts[i] % 2 == 1) {
            odd_count++;
        }
        total_pairs += char_counts[i] / 2;
    }
    
    long long result = fact[total_pairs];
    for (int i = 0; i < MAX_CHAR; i++) {
        result = (result * inv_fact[char_counts[i] / 2]) % MOD;
    }
    
    if (odd_count > 0) {
        result = (result * odd_count) % MOD;
    }
    
    return (int)result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char* s = readline();

    initialize(s);

    int q = parse_int(ltrim(rtrim(readline())));

    for (int q_itr = 0; q_itr < q; q_itr++) {
        char** first_multiple_input = split_string(rtrim(readline()));

        int l = parse_int(*(first_multiple_input + 0));

        int r = parse_int(*(first_multiple_input + 1));

        int result = answerQuery(l, r);

        fprintf(fptr, "%d\n", result);
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
