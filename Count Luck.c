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

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int visited[101][101];
int n, m;
char** grid;

int isValid(int x, int y) {
    return (x >= 0 && y >= 0 && x < n && y < m &&
            grid[x][y] != 'X' && !visited[x][y]);
}

int dfs(int x, int y) {
    if (grid[x][y] == '*') return 0;

    visited[x][y] = 1;

    int moves = 0;

    // Count valid moves
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (isValid(nx, ny)) {
            moves++;
        }
    }

    int wand = (moves > 1) ? 1 : 0;

    // Continue DFS
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (isValid(nx, ny)) {
            int res = dfs(nx, ny);
            if (res != -1) {
                return wand + res;
            }
        }
    }

    return -1;
}

char* countLuck(int matrix_count, char** matrix, int k) {

    grid = matrix;
    n = matrix_count;
    m = strlen(matrix[0]);

    memset(visited, 0, sizeof(visited));

    int startX = 0, startY = 0;

    // Find starting point 'M'
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'M') {
                startX = i;
                startY = j;
            }
        }
    }

    int waves = dfs(startX, startY);

    if (waves == k)
        return "Impressed";
    else
        return "Oops!";
}
int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int t = parse_int(ltrim(rtrim(readline())));

    for (int t_itr = 0; t_itr < t; t_itr++) {
        char** first_multiple_input = split_string(rtrim(readline()));

        int n = parse_int(*(first_multiple_input + 0));

        int m = parse_int(*(first_multiple_input + 1));

        char** matrix = malloc(n * sizeof(char*));

        for (int i = 0; i < n; i++) {
            char* matrix_item = readline();

            *(matrix + i) = matrix_item;
        }

        int k = parse_int(ltrim(rtrim(readline())));

        char* result = countLuck(n, matrix, k);

        fprintf(fptr, "%s\n", result);
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
