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

/* ---------- Heap Utility Functions ---------- */

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/* Max Heap (for lower half) */

void maxHeapifyUp(int heap[], int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[parent] >= heap[index]) break;
        swap(&heap[parent], &heap[index]);
        index = parent;
    }
}

void maxHeapifyDown(int heap[], int size, int index) {
    while (1) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size && heap[left] > heap[largest])
            largest = left;

        if (right < size && heap[right] > heap[largest])
            largest = right;

        if (largest == index) break;

        swap(&heap[index], &heap[largest]);
        index = largest;
    }
}

/* Min Heap (for upper half) */

void minHeapifyUp(int heap[], int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[parent] <= heap[index]) break;
        swap(&heap[parent], &heap[index]);
        index = parent;
    }
}

void minHeapifyDown(int heap[], int size, int index) {
    while (1) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size && heap[left] < heap[smallest])
            smallest = left;

        if (right < size && heap[right] < heap[smallest])
            smallest = right;

        if (smallest == index) break;

        swap(&heap[index], &heap[smallest]);
        index = smallest;
    }
}

/*
 * Complete the 'runningMedian' function below.
 */
double* runningMedian(int a_count, int* a, int* result_count) {

    *result_count = a_count;
    double* result = malloc(a_count * sizeof(double));

    int *maxHeap = malloc(a_count * sizeof(int));
    int *minHeap = malloc(a_count * sizeof(int));

    int maxSize = 0;
    int minSize = 0;

    for (int i = 0; i < a_count; i++) {

        int x = a[i];

        if (maxSize == 0 || x <= maxHeap[0]) {
            maxHeap[maxSize] = x;
            maxHeapifyUp(maxHeap, maxSize);
            maxSize++;
        } else {
            minHeap[minSize] = x;
            minHeapifyUp(minHeap, minSize);
            minSize++;
        }

        if (maxSize > minSize + 1) {
            int move = maxHeap[0];
            maxHeap[0] = maxHeap[--maxSize];
            maxHeapifyDown(maxHeap, maxSize, 0);

            minHeap[minSize] = move;
            minHeapifyUp(minHeap, minSize);
            minSize++;
        }
        else if (minSize > maxSize) {
            int move = minHeap[0];
            minHeap[0] = minHeap[--minSize];
            minHeapifyDown(minHeap, minSize, 0);

            maxHeap[maxSize] = move;
            maxHeapifyUp(maxHeap, maxSize);
            maxSize++;
        }

        if (maxSize == minSize)
            result[i] = (maxHeap[0] + minHeap[0]) / 2.0;
        else
            result[i] = maxHeap[0];
    }

    return result;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int a_count = parse_int(ltrim(rtrim(readline())));

    int* a = malloc(a_count * sizeof(int));

    for (int i = 0; i < a_count; i++) {
        int a_item = parse_int(ltrim(rtrim(readline())));
        *(a + i) = a_item;
    }

    int result_count;
    double* result = runningMedian(a_count, a, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%.1f", *(result + i));

        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }

    fprintf(fptr, "\n");

    fclose(fptr);

    return 0;
}

/* ---------- Helper Functions ---------- */

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) break;

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n')
            break;

        alloc_length <<= 1;
        data = realloc(data, alloc_length);

        if (!data) break;
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

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
