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

#define MAX 1000001

int downToZero(int n) {

    int visited[MAX] = {0};
    int dist[MAX];

    int queue[MAX];
    int front = 0, rear = 0;

    queue[rear++] = n;
    visited[n] = 1;
    dist[n] = 0;

    while(front < rear) {

        int current = queue[front++];
        int moves = dist[current];

        if(current == 0)
            return moves;

        /* Operation 1: decrease by 1 */
        if(!visited[current - 1]) {
            visited[current - 1] = 1;
            dist[current - 1] = moves + 1;
            queue[rear++] = current - 1;
        }

        /* Operation 2: factor move */
        for(int i = 2; i * i <= current; i++) {
            if(current % i == 0) {

                int next = current / i;

                if(!visited[next]) {
                    visited[next] = 1;
                    dist[next] = moves + 1;
                    queue[rear++] = next;
                }
            }
        }
    }

    return -1;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int q = parse_int(ltrim(rtrim(readline())));

    for (int q_itr = 0; q_itr < q; q_itr++) {
        int n = parse_int(ltrim(rtrim(readline())));
        int result = downToZero(n);
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

        if (!line)
            break;

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n')
            break;

        alloc_length <<= 1;
        data = realloc(data, alloc_length);
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);
        data[data_length] = '\0';
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) return '\0';
    while (*str && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str) return '\0';

    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;

    *(end + 1) = '\0';
    return str;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0')
        exit(EXIT_FAILURE);

    return value;
}
