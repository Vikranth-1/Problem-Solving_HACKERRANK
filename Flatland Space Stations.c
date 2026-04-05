#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char** split_string(char*);

int flatlandSpaceStations(int n, int c_count, int* c) {
    // Sort the space station indices using bubble sort
    for (int i = 0; i < c_count - 1; i++) {
        for (int j = 0; j < c_count - i - 1; j++) {
            if (c[j] > c[j + 1]) {
                int temp = c[j];
                c[j] = c[j + 1];
                c[j + 1] = temp;
            }
        }
    }
    
    // Find maximum distance from first city to first station
    int max_dist = c[0];
    
    // Find maximum distance between consecutive stations
    for (int i = 1; i < c_count; i++) {
        int dist = (c[i] - c[i - 1]) / 2;
        if (dist > max_dist) {
            max_dist = dist;
        }
    }
    
    // Find distance from last station to last city
    int last_dist = (n - 1) - c[c_count - 1];
    if (last_dist > max_dist) {
        max_dist = last_dist;
    }
    
    return max_dist;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** nm = split_string(readline());

    char* n_endptr;
    char* n_str = nm[0];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char* m_endptr;
    char* m_str = nm[1];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char** c_temp = split_string(readline());

    int* c = malloc(m * sizeof(int));

    for (int i = 0; i < m; i++) {
        char* c_item_endptr;
        char* c_item_str = *(c_temp + i);
        int c_item = strtol(c_item_str, &c_item_endptr, 10);

        if (c_item_endptr == c_item_str || *c_item_endptr != '\0') { exit(EXIT_FAILURE); }

        *(c + i) = c_item;
    }

    int c_count = m;

    int result = flatlandSpaceStations(n, c_count, c);

    fprintf(fptr, "%d\n", result);

    fclose(fptr);
    free(c);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) { break; }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') { break; }

        size_t new_length = alloc_length << 1;
        data = realloc(data, new_length);

        if (!data) { break; }

        alloc_length = new_length;
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
    }

    data = realloc(data, data_length);

    return data;
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
