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
int isValid(int x, int y, int n) {
    return (x >= 0 && y >= 0 && x < n && y < n);
}

int bfs(int n, int a, int b) {
    int dx[8] = {a, a, -a, -a, b, b, -b, -b};
    int dy[8] = {b, -b, b, -b, a, -a, a, -a};

    int visited[n][n];
    memset(visited, 0, sizeof(visited));

    // Queue for BFS
    int queue[n * n][3]; // x, y, distance
    int front = 0, rear = 0;

    queue[rear][0] = 0;
    queue[rear][1] = 0;
    queue[rear][2] = 0;
    rear++;

    visited[0][0] = 1;

    while (front < rear) {
        int x = queue[front][0];
        int y = queue[front][1];
        int dist = queue[front][2];
        front++;

        if (x == n - 1 && y == n - 1)
            return dist;

        for (int i = 0; i < 8; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (isValid(nx, ny, n) && !visited[nx][ny]) {
                visited[nx][ny] = 1;
                queue[rear][0] = nx;
                queue[rear][1] = ny;
                queue[rear][2] = dist + 1;
                rear++;
            }
        }
    }

    return -1;
}

int** knightlOnAChessboard(int n, int* result_rows, int* result_columns) {
    *result_rows = n - 1;
    *result_columns = n - 1;

    int** result = malloc((n - 1) * sizeof(int*));
    for (int i = 0; i < n - 1; i++) {
        result[i] = malloc((n - 1) * sizeof(int));
    }

    for (int a = 1; a < n; a++) {
        for (int b = 1; b < n; b++) {
            result[a - 1][b - 1] = bfs(n, a, b);
        }
    }

    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int n = parse_int(ltrim(rtrim(readline())));

    int result_rows;
    int result_columns;
    int** result = knightlOnAChessboard(n, &result_rows, &result_columns);

    for (int i = 0; i < result_rows; i++) {
        for (int j = 0; j < result_columns; j++) {
            fprintf(fptr, "%d", *(*(result + i) + j));

            if (j != result_columns - 1) {
                fprintf(fptr, " ");
            }
        }

        if (i != result_rows - 1) {
            fprintf(fptr, "\n");
        }
    }

    fprintf(fptr, "\n");

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
