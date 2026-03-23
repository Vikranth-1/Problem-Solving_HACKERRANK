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
    int r, c1, c2;
} Track;

int cmp(const void *a, const void *b) {
    Track *t1 = (Track*)a;
    Track *t2 = (Track*)b;
    if (t1->r != t2->r) return t1->r - t2->r;
    return t1->c1 - t2->c1;
}

long gridlandMetro(int n, int m, int k, int track_rows, int track_columns, int** track) {
    Track *arr = malloc(k * sizeof(Track));

    for (int i = 0; i < k; i++) {
        arr[i].r = track[i][0];
        arr[i].c1 = track[i][1];
        arr[i].c2 = track[i][2];
    }

    qsort(arr, k, sizeof(Track), cmp);

    long occupied = 0;

    int i = 0;
    while (i < k) {
        int row = arr[i].r;
        long start = arr[i].c1;
        long end = arr[i].c2;
        i++;

        while (i < k && arr[i].r == row) {
            if (arr[i].c1 <= end) {
                if (arr[i].c2 > end) end = arr[i].c2;
            } else {
                occupied += (end - start + 1);
                start = arr[i].c1;
                end = arr[i].c2;
            }
            i++;
        }

        occupied += (end - start + 1);
    }

    free(arr);

    return (long)n * m - occupied;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));
    int m = parse_int(*(first_multiple_input + 1));
    int k = parse_int(*(first_multiple_input + 2));

    int** track = malloc(k * sizeof(int*));

    for (int i = 0; i < k; i++) {
        track[i] = malloc(3 * sizeof(int));
        char** temp = split_string(rtrim(readline()));
        for (int j = 0; j < 3; j++) {
            track[i][j] = parse_int(temp[j]);
        }
    }

    long result = gridlandMetro(n, m, k, k, 3, track);

    fprintf(fptr, "%ld\n", result);

    fclose(fptr);
    return 0;
}

char* readline() {
    size_t alloc_length = 1024, data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);
        if (!line) break;
        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;
        alloc_length <<= 1;
        data = realloc(data, alloc_length);
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
    }

    return data;
}

char* ltrim(char* str) {
    while (*str && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");
    int count = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++count);
        splits[count - 1] = token;
        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    return strtol(str, NULL, 10);
}
