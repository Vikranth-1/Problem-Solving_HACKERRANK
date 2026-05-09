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

int cutTheTree(int data_count, int* data, int edges_rows, int edges_columns, int** edges) {
    int* sum = malloc((data_count + 1) * sizeof(int));
    int* visited = calloc((data_count + 1), sizeof(int));
    int total = 0;
    
    for (int i = 1; i <= data_count; i++) {
        total += data[i - 1];
    }
    
    int* adj[data_count + 1];
    int adj_count[data_count + 1];
    for (int i = 1; i <= data_count; i++) {
        adj[i] = malloc(data_count * sizeof(int));
        adj_count[i] = 0;
    }
    
    for (int i = 0; i < edges_rows; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        adj[u][adj_count[u]++] = v;
        adj[v][adj_count[v]++] = u;
    }
    
    int stack[data_count + 1];
    int top = -1;
    int parent[data_count + 1];
    int order[data_count + 1];
    int order_idx = 0;
    
    stack[++top] = 1;
    parent[1] = -1;
    visited[1] = 1;
    
    while (top >= 0) {
        int u = stack[top--];
        order[order_idx++] = u;
        for (int i = 0; i < adj_count[u]; i++) {
            int v = adj[u][i];
            if (!visited[v]) {
                visited[v] = 1;
                parent[v] = u;
                stack[++top] = v;
            }
        }
    }
    
    for (int i = 1; i <= data_count; i++) {
        sum[i] = data[i - 1];
    }
    
    for (int i = order_idx - 1; i >= 0; i--) {
        int u = order[i];
        if (parent[u] != -1) {
            sum[parent[u]] += sum[u];
        }
    }
    
    int min_diff = total;
    for (int i = 2; i <= data_count; i++) {
        int diff = abs(total - 2 * sum[i]);
        if (diff < min_diff) {
            min_diff = diff;
        }
    }
    
    for (int i = 1; i <= data_count; i++) {
        free(adj[i]);
    }
    free(sum);
    free(visited);
    
    return min_diff;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int n = parse_int(ltrim(rtrim(readline())));

    char** data_temp = split_string(rtrim(readline()));

    int* data = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int data_item = parse_int(*(data_temp + i));

        *(data + i) = data_item;
    }

    int** edges = malloc((n - 1) * sizeof(int*));

    for (int i = 0; i < n - 1; i++) {
        *(edges + i) = malloc(2 * (sizeof(int)));

        char** edges_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < 2; j++) {
            int edges_item = parse_int(*(edges_item_temp + j));

            *(*(edges + i) + j) = edges_item;
        }
    }

    int result = cutTheTree(n, data, n - 1, 2, edges);

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
