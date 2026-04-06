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

int twoPluses(int grid_count, char** grid) {
    int n = grid_count;
    int m = strlen(grid[0]);
    int max_product = 0;
    
    // Store all possible pluses with their center and size
    typedef struct {
        int r, c, size;
    } Plus;
    
    Plus pluses[1000];
    int plus_count = 0;
    
    // Find all possible pluses
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'G') {
                int size = 0;
                while (1) {
                    int s = size + 1;
                    if (i - s >= 0 && i + s < n && j - s >= 0 && j + s < m &&
                        grid[i - s][j] == 'G' && grid[i + s][j] == 'G' &&
                        grid[i][j - s] == 'G' && grid[i][j + s] == 'G') {
                        size++;
                    } else {
                        break;
                    }
                }
                // Add all possible pluses at this center (size 0 means just the center)
                for (int s = 0; s <= size; s++) {
                    pluses[plus_count].r = i;
                    pluses[plus_count].c = j;
                    pluses[plus_count].size = s;
                    plus_count++;
                }
            }
        }
    }
    
    // Check all pairs of pluses
    for (int i = 0; i < plus_count; i++) {
        for (int j = i + 1; j < plus_count; j++) {
            Plus p1 = pluses[i];
            Plus p2 = pluses[j];
            
            // Check if the two pluses overlap
            int overlap = 0;
            
            // Get all cells of first plus
            int cells1[1000][2];
            int count1 = 0;
            for (int d = -p1.size; d <= p1.size; d++) {
                if (d == 0) {
                    cells1[count1][0] = p1.r;
                    cells1[count1][1] = p1.c;
                    count1++;
                } else {
                    cells1[count1][0] = p1.r + d;
                    cells1[count1][1] = p1.c;
                    count1++;
                    cells1[count1][0] = p1.r;
                    cells1[count1][1] = p1.c + d;
                    count1++;
                }
            }
            
            // Check if any cell of second plus overlaps with first
            for (int d = -p2.size; d <= p2.size && !overlap; d++) {
                int r2, c2;
                if (d == 0) {
                    r2 = p2.r;
                    c2 = p2.c;
                    for (int k = 0; k < count1; k++) {
                        if (cells1[k][0] == r2 && cells1[k][1] == c2) {
                            overlap = 1;
                            break;
                        }
                    }
                } else {
                    r2 = p2.r + d;
                    c2 = p2.c;
                    for (int k = 0; k < count1; k++) {
                        if (cells1[k][0] == r2 && cells1[k][1] == c2) {
                            overlap = 1;
                            break;
                        }
                    }
                    if (!overlap) {
                        r2 = p2.r;
                        c2 = p2.c + d;
                        for (int k = 0; k < count1; k++) {
                            if (cells1[k][0] == r2 && cells1[k][1] == c2) {
                                overlap = 1;
                                break;
                            }
                        }
                    }
                }
            }
            
            if (!overlap) {
                int area1 = 4 * p1.size + 1;
                int area2 = 4 * p2.size + 1;
                int product = area1 * area2;
                if (product > max_product) {
                    max_product = product;
                }
            }
        }
    }
    
    return max_product;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int m = parse_int(*(first_multiple_input + 1));

    char** grid = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* grid_item = readline();

        *(grid + i) = grid_item;
    }

    int result = twoPluses(n, grid);

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
