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

typedef struct {
    int* parent;
    int* size;
} DSU;

DSU* createDSU(int n) {
    DSU* dsu = malloc(sizeof(DSU));
    dsu->parent = malloc((n + 1) * sizeof(int));
    dsu->size = malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) {
        dsu->parent[i] = i;
        dsu->size[i] = 1;
    }
    return dsu;
}

int find(DSU* dsu, int x) {
    if (dsu->parent[x] != x) {
        dsu->parent[x] = find(dsu, dsu->parent[x]);
    }
    return dsu->parent[x];
}

void unionSet(DSU* dsu, int x, int y) {
    int rootX = find(dsu, x);
    int rootY = find(dsu, y);
    if (rootX != rootY) {
        if (dsu->size[rootX] < dsu->size[rootY]) {
            dsu->parent[rootX] = rootY;
            dsu->size[rootY] += dsu->size[rootX];
        } else {
            dsu->parent[rootY] = rootX;
            dsu->size[rootX] += dsu->size[rootY];
        }
    }
}

void freeDSU(DSU* dsu) {
    free(dsu->parent);
    free(dsu->size);
    free(dsu);
}

long roadsAndLibraries(int n, int c_lib, int c_road, int cities_rows, int cities_columns, int** cities) {
    if (c_lib <= c_road) {
        return (long)n * c_lib;
    }
    
    DSU* dsu = createDSU(n);
    
    for (int i = 0; i < cities_rows; i++) {
        unionSet(dsu, cities[i][0], cities[i][1]);
    }
    
    int* visited = calloc((n + 1), sizeof(int));
    long total_cost = 0;
    
    for (int i = 1; i <= n; i++) {
        int root = find(dsu, i);
        if (!visited[root]) {
            visited[root] = 1;
            total_cost += c_lib + (long)(dsu->size[root] - 1) * c_road;
        }
    }
    
    free(visited);
    freeDSU(dsu);
    return total_cost;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int q = parse_int(ltrim(rtrim(readline())));

    for (int q_itr = 0; q_itr < q; q_itr++) {
        char** first_multiple_input = split_string(rtrim(readline()));

        int n = parse_int(*(first_multiple_input + 0));

        int m = parse_int(*(first_multiple_input + 1));

        int c_lib = parse_int(*(first_multiple_input + 2));

        int c_road = parse_int(*(first_multiple_input + 3));

        int** cities = malloc(m * sizeof(int*));

        for (int i = 0; i < m; i++) {
            *(cities + i) = malloc(2 * (sizeof(int)));

            char** cities_item_temp = split_string(rtrim(readline()));

            for (int j = 0; j < 2; j++) {
                int cities_item = parse_int(*(cities_item_temp + j));

                *(*(cities + i) + j) = cities_item;
            }
        }

        long result = roadsAndLibraries(n, c_lib, c_road, m, 2, cities);

        fprintf(fptr, "%ld\n", result);
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
