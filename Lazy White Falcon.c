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

/* ---------- Adjacency list ---------- */

#define MAXN 100005

int head[MAXN];   /* head of adjacency list for each node */
int nxt[MAXN*2];  /* next pointer in linked list          */
int to[MAXN*2];   /* destination of this edge             */
int edge_cnt = 0;

void add_edge(int u, int v) {
    edge_cnt++;
    to[edge_cnt] = v;
    nxt[edge_cnt] = head[u];
    head[u] = edge_cnt;
}

/* ---------- BFS to find path u -> v ----------
 * We do a single BFS from u, recording the parent of every visited node.
 * Then we walk back from v to u using the parent array to collect the path.
 * Time: O(n) per query - fine for n,q <= 1e4.
 */

int parent[MAXN];
bool visited[MAXN];

/* Returns number of nodes on path; path[] is filled root-to-leaf order */
int find_path(int u, int v, int n, int* path) {
    /* BFS from u */
    int* queue = malloc(n * sizeof(int));
    int front = 0, back = 0;

    for (int i = 0; i < n; i++) { visited[i] = false; parent[i] = -1; }

    visited[u] = true;
    queue[back++] = u;

    while (front < back) {
        int cur = queue[front++];
        if (cur == v) break;
        for (int e = head[cur]; e; e = nxt[e]) {
            int nb = to[e];
            if (!visited[nb]) {
                visited[nb] = true;
                parent[nb] = cur;
                queue[back++] = nb;
            }
        }
    }

    /* Walk back from v to u to collect path */
    int len = 0;
    int cur = v;
    while (cur != -1) {
        path[len++] = cur;
        cur = parent[cur];
    }

    /* Reverse so path goes u -> v */
    for (int i = 0, j = len - 1; i < j; i++, j--) {
        int tmp = path[i]; path[i] = path[j]; path[j] = tmp;
    }

    free(queue);
    return len;
}

/* ---------- Node values ---------- */

long long val[MAXN];  /* current value of each node; initially 0 */

/* ---------- solve ---------- */

int* solve(int tree_rows, int tree_columns, int** tree,
           int queries_rows, int queries_columns, int** queries,
           int* result_count) {

    int n = tree_rows + 1;  /* number of nodes = edges + 1 */
    int q = queries_rows;

    /* Build adjacency list (undirected) */
    for (int i = 0; i < tree_rows; i++) {
        int u = tree[i][0];
        int v = tree[i][1];
        add_edge(u, v);
        add_edge(v, u);
    }

    /* Initialise node values to 0 */
    for (int i = 0; i < n; i++) val[i] = 0;

    /* We may have up to q type-2 results */
    int* results = malloc(q * sizeof(int));
    int  rc = 0;

    int* path = malloc(n * sizeof(int));

    for (int i = 0; i < q; i++) {
        int type = queries[i][0];

        if (type == 1) {
            /* Update: set val[u] = x */
            int u = queries[i][1];
            int x = queries[i][2];
            val[u] = x;

        } else {
            /* Query: sum of node values on path u -> v */
            int u = queries[i][1];
            int v = queries[i][2];

            int len = find_path(u, v, n, path);

            long long sum = 0;
            for (int j = 0; j < len; j++) {
                sum += val[path[j]];
            }

            results[rc++] = (int)sum;
        }
    }

    free(path);
    *result_count = rc;
    return results;
}

/* ---------- main (provided, unchanged except freeing memory) ---------- */

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));
    int q = parse_int(*(first_multiple_input + 1));

    int** tree = malloc((n - 1) * sizeof(int*));

    for (int i = 0; i < n - 1; i++) {
        *(tree + i) = malloc(2 * sizeof(int));

        char** tree_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < 2; j++) {
            int tree_item = parse_int(*(tree_item_temp + j));
            *(*(tree + i) + j) = tree_item;
        }

        free(tree_item_temp);
    }

    int** queries = malloc(q * sizeof(int*));

    for (int i = 0; i < q; i++) {
        *(queries + i) = malloc(3 * sizeof(int));

        char** queries_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < 3; j++) {
            int queries_item = parse_int(*(queries_item_temp + j));
            *(*(queries + i) + j) = queries_item;
        }

        free(queries_item_temp);
    }

    int result_count;
    int* result = solve(n - 1, 2, tree, q, 3, queries, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", *(result + i));
        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }

    fprintf(fptr, "\n");

    free(result);
    for (int i = 0; i < n - 1; i++) free(tree[i]);
    free(tree);
    for (int i = 0; i < q; i++) free(queries[i]);
    free(queries);

    fclose(fptr);
    return 0;
}

/* ---------- Helper functions (provided, unchanged) ---------- */

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

        alloc_length <<= 1;
        data = realloc(data, alloc_length);

        if (!data) { data = '\0'; break; }
    }

    if (data_length == 0) { free(data); return NULL; }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = realloc(data, data_length);
        if (!data) { data = '\0'; }
    } else {
        data = realloc(data, data_length + 1);
        if (!data) { data = '\0'; }
        else { data[data_length] = '\0'; }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) return '\0';
    if (!*str) return str;
    while (*str != '\0' && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str) return '\0';
    if (!*str) return str;
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");
    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);
        if (!splits) return splits;
        splits[spaces - 1] = token;
        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') exit(EXIT_FAILURE);
    return value;
}
