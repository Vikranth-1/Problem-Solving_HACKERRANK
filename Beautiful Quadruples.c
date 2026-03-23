#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char** split_string(char*);

int beautifulQuadruples(int a, int b, int c, int d) {
    int arr[4] = {a, b, c, d};
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (arr[i] > arr[j]) {
                int t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
        }
    }

    a = arr[0]; b = arr[1]; c = arr[2]; d = arr[3];

    static int cnt[4096][4096];
    static int prefix[4096][4096];

    for (int x = 1; x <= b; x++) {
        for (int w = 1; w <= a && w <= x; w++) {
            cnt[x][w ^ x]++;
        }
    }

    for (int x = 1; x <= b; x++) {
        for (int v = 0; v < 4096; v++) {
            prefix[x][v] = prefix[x - 1][v] + cnt[x][v];
        }
    }

    long long result = 0;

    for (int y = 1; y <= c; y++) {
        for (int z = y; z <= d; z++) {
            int val = y ^ z;
            int total = y <= b ? prefix[y][val] : prefix[b][val];
            result += total;
        }
    }

    long long total = 0;

    for (int w = 1; w <= a; w++) {
        for (int x = w; x <= b; x++) {
            for (int y = x; y <= c; y++) {
                total += (d - y + 1);
            }
        }
    }

    return (int)(total - result);
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** abcd = split_string(readline());

    int a = strtol(abcd[0], NULL, 10);
    int b = strtol(abcd[1], NULL, 10);
    int c = strtol(abcd[2], NULL, 10);
    int d = strtol(abcd[3], NULL, 10);

    int result = beautifulQuadruples(a, b, c, d);

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
        if (!line) break;
        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;
        alloc_length <<= 1;
        data = realloc(data, alloc_length);
        if (!data) break;
    }

    if (data[data_length - 1] == '\n') data[data_length - 1] = '\0';
    data = realloc(data, data_length);
    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");
    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);
        splits[spaces - 1] = token;
        token = strtok(NULL, " ");
    }

    return splits;
}
