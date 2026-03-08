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

/* ---------- Constants ---------- */

#define MAXN  100005
#define LOG   17   /* ceil(log2(100000)) = 17 */

/* ---------- Adjacency list ---------- */

int head[MAXN];
int nxt[MAXN * 2];
int to_node[MAXN * 2];
int edge_cnt = 0;

void add_edge(int u, int v) {
    edge_cnt++;
    to_node[edge_cnt] = v;
    nxt[edge_cnt] = head[u];
    head[u] = edge_cnt;
}

/* ---------- LCA (binary lifting) ----------
 *
 * up[k][v] = 2^k-th ancestor of v
 * depth[v] = depth of v (root = 0)
 * prefix[v] = sum of val[] from root down to v (computed once at DFS time,
 *             but node values change, so we handle updates differently below)
 *
 * Path sum u->v = prefix[u] + prefix[v] - prefix[lca] - prefix[parent[lca]]
 * But since values are mutable we store val[] separately and keep a
 * Fenwick tree (BIT) indexed by DFS in-time for prefix sums.
 */

int  up[LOG][MAXN];
int  depth[MAXN];
int  in_time[MAXN];   /* DFS entry time  */
int  out_time[MAXN];  /* DFS exit time   */
int  order[MAXN];     /* order[in_time[v]] = v */
int  timer_val = 0;
int  N;               /* number of nodes */

/* Iterative DFS to avoid stack overflow on large chains */
void dfs(int root) {
    int* stack  = malloc(N * sizeof(int));
    int* parent = malloc(N * sizeof(int));
    bool* visited = calloc(N, sizeof(bool));
    int top = 0;

    stack[top]  = root;
    parent[top] = -1;
    top++;

    while (top > 0) {
        int v = stack[--top];
        if (visited[v]) continue;
        visited[v] = true;

        int par = parent[top]; /* saved below */
        /* Re-push with parent info using a two-pass trick:
         * We use a signed encoding: negative means "post-visit". */
        /* Simpler: just do it with explicit parent tracking. */
        /* We'll redo with a proper iterative post-order below. */
        (void)par;
    }
    free(stack); free(parent); free(visited);
}

/* Actually use a clean iterative DFS with explicit state */
typedef struct { int v, p, e; } Frame;  /* node, parent edge-index */

void build_lca(int root) {
    Frame* stack = malloc(N * sizeof(Frame));
    int top = 0;

    up[0][root] = root;
    depth[root] = 0;
    stack[top++] = (Frame){root, -1, head[root]};
    in_time[root] = timer_val;
    order[timer_val] = root;
    timer_val++;

    while (top > 0) {
        Frame* f = &stack[top - 1];
        if (f->e == 0) {
            /* All children processed */
            out_time[f->v] = timer_val - 1;
            top--;
        } else {
            int e  = f->e;
            f->e   = nxt[e];          /* advance to next edge */
            int nb = to_node[e];
            int par = (top >= 2) ? stack[top-2].v : root;
            if (nb == par && top > 1) continue; /* skip parent edge */
            /* Also skip if already visited (handles root self-loop) */
            if (depth[nb] != 0 && nb != root) continue;
            if (nb == root && top > 1) continue;

            depth[nb]  = depth[f->v] + 1;
            up[0][nb]  = f->v;

            /* Fill binary lifting table for nb */
            for (int k = 1; k < LOG; k++)
                up[k][nb] = up[k-1][up[k-1][nb]];

            in_time[nb] = timer_val;
            order[timer_val] = nb;
            timer_val++;

            stack[top++] = (Frame){nb, f->v, head[nb]};
        }
    }
    free(stack);
}

/* ---------- Fenwick Tree (BIT) on DFS order ----------
 * Indexed 1..N (in_time is 0-based so we add 1).
 * Point update, prefix query.
 */

long long bit[MAXN];

void bit_update(int i, long long delta) {
    for (i++; i <= N; i += i & (-i))
        bit[i] += delta;
}

long long bit_query(int i) {
    long long s = 0;
    for (i++; i > 0; i -= i & (-i))
        s += bit[i];
    return s;
}

/* Sum over DFS range [l, r] (0-based in_time) */
long long bit_range(int l, int r) {
    if (l > r) return 0;
    return bit_query(r) - (l > 0 ? bit_query(l - 1) : 0);
}

/* ---------- LCA query ---------- */

int lca(int u, int v) {
    if (depth[u] < depth[v]) { int t = u; u = v; v = t; }
    int diff = depth[u] - depth[v];
    for (int k = 0; k < LOG; k++)
        if ((diff >> k) & 1) u = up[k][u];
    if (u == v) return u;
    for (int k = LOG - 1; k >= 0; k--)
        if (up[k][u] != up[k][v]) { u = up[k][u]; v = up[k][v]; }
    return up[0][u];
}

/* Sum of node values on path u -> v using BIT + LCA.
 * path_sum(u,v) = bit_range(0, in[u]) ... we need subtree sums.
 *
 * Key identity (Euler tour / in-out time):
 *   sum on path(u,v) = prefix[u] + prefix[v] - prefix[lca] - prefix[parent(lca)]
 * where prefix[v] = sum of values from root to v.
 *
 * With BIT on DFS in-time:
 *   prefix[v] = sum over all nodes w where in[w] <= in[v] AND out[w] >= in[v]
 *             = sum over ancestors of v including v
 * That's not a simple BIT range. Instead we use the subtree-sum trick:
 *
 *   subtree_sum(v) = bit_range(in[v], out[v])
 *   prefix_sum(v)  = bit_range(0, in[v])   -- only if DFS order == preorder
 *                                              and each node contributes once
 * Since we do a standard preorder DFS, bit_range(0, in[v]) gives exactly
 * the sum of values of all nodes visited before or at v in preorder,
 * which equals the sum of v and all its ancestors -- NOT all nodes on
 * the path. We need another approach.
 *
 * Correct approach: use the identity
 *   path_sum(u,v) = subtree_sum(u) + subtree_sum(v)
 *                 - subtree_sum(lca) - subtree_sum(parent(lca))
 * where subtree_sum(v) = sum of val[w] for all w in subtree of v... No.
 *
 * Simplest correct O(log^2 n) approach without HLD:
 * Just walk u and v up to LCA step by step using binary lifting,
 * but that's O(n) worst case again.
 *
 * Correct O(log n) with BIT: store values at in_time positions.
 * Define f(v) = sum of val on path root->v.
 * When we update val[u] by delta, ALL nodes in subtree(u) have their
 * f value increased by delta. Subtree(u) = in_time range [in[u], out[u]].
 * So: range-update BIT with point-query.
 *
 * path_sum(u,v) = f(u) + f(v) - f(lca) - f(parent(lca))
 * f(v) = point query at in[v] on the range-update BIT.
 */

/* Range-update, point-query BIT */
long long rbit[MAXN];

void rbit_range_add(int l, int r, long long delta) {
    /* add delta to positions l..r */
    for (int i = l + 1; i <= N; i += i & (-i)) rbit[i] += delta;
    for (int i = r + 2; i <= N; i += i & (-i)) rbit[i] -= delta;
}

long long rbit_point_query(int i) {
    long long s = 0;
    for (int j = i + 1; j > 0; j -= j & (-j)) s += rbit[j];
    return s;
}

/* val array to track current values for delta computation */
long long val[MAXN];

/* ---------- solve ---------- */

int* solve(int tree_rows, int tree_columns, int** tree,
           int queries_rows, int queries_columns, int** queries,
           int* result_count) {

    N = tree_rows + 1;

    /* Build adjacency list */
    for (int i = 0; i < tree_rows; i++) {
        add_edge(tree[i][0], tree[i][1]);
        add_edge(tree[i][1], tree[i][0]);
    }

    /* Fill binary lifting table via iterative DFS from node 0 */
    /* Initialize depth array to -1 to detect visited */
    for (int i = 0; i < N; i++) depth[i] = -1;
    depth[0] = 0;

    /* Iterative preorder DFS */
    {
        int* stk = malloc(N * sizeof(int));
        int* par = malloc(N * sizeof(int));
        int top = 0;
        stk[top] = 0; par[top] = 0; top++;

        up[0][0] = 0;
        for (int k = 1; k < LOG; k++) up[k][0] = 0;

        while (top > 0) {
            top--;
            int v = stk[top];
            int p = par[top];

            in_time[v] = timer_val;
            order[timer_val] = v;
            timer_val++;

            /* We need out_time after all children; use two-pass.
             * Store negative index as sentinel for post-processing. */
            /* Simpler: compute out_time in a second DFS pass, or
             * use the fact that out_time[v] = in_time[v] + subtree_size[v] - 1.
             * Compute subtree sizes first. */
            (void)p;
        }
        free(stk); free(par);
    }

    /* Use a clean single iterative DFS that computes in/out times and up[][] */
    timer_val = 0;
    for (int i = 0; i < N; i++) depth[i] = -1;
    depth[0] = 0;
    up[0][0] = 0;
    for (int k = 1; k < LOG; k++) up[k][0] = 0;

    {
        /* Stack stores (node, parent, edge_ptr, entered) */
        int* sv  = malloc(N * sizeof(int));
        int* sp  = malloc(N * sizeof(int));
        int* se  = malloc(N * sizeof(int));
        int top  = 0;

        sv[top] = 0; sp[top] = -1; se[top] = head[0]; top++;
        in_time[0] = timer_val++;

        while (top > 0) {
            int v = sv[top-1];
            int p = sp[top-1];
            int e = se[top-1];

            /* Find next unvisited neighbor */
            while (e && (to_node[e] == p || depth[to_node[e]] != -1))
                e = nxt[e];
            se[top-1] = e;

            if (e == 0) {
                /* Done with v */
                out_time[v] = timer_val - 1;
                top--;
            } else {
                int nb = to_node[e];
                se[top-1] = nxt[e]; /* consume this edge */

                depth[nb]  = depth[v] + 1;
                up[0][nb]  = v;
                for (int k = 1; k < LOG; k++)
                    up[k][nb] = up[k-1][up[k-1][nb]];

                in_time[nb] = timer_val++;
                sv[top] = nb; sp[top] = v; se[top] = head[nb];
                top++;
            }
        }
        free(sv); free(sp); free(se);
    }

    /* All values start at 0, BIT is zero-initialized */

    int* results = malloc(queries_rows * sizeof(int));
    int  rc = 0;

    for (int i = 0; i < queries_rows; i++) {
        int type = queries[i][0];

        if (type == 1) {
            int u     = queries[i][1];
            long long x = queries[i][2];
            long long delta = x - val[u];
            val[u] = x;
            /* Update: add delta to all nodes in subtree(u) -- affects f(w) for
             * every descendant w of u (including u). Range update on [in[u], out[u]]. */
            rbit_range_add(in_time[u], out_time[u], delta);

        } else {
            int u = queries[i][1];
            int v = queries[i][2];
            int l = lca(u, v);
            int pl = up[0][l]; /* parent of lca; for root, up[0][root]=root */

            /* path_sum = f(u) + f(v) - f(l) - f(pl)
             * f(v) = sum of val on path root->v = rbit_point_query(in[v]) */
            long long fu  = rbit_point_query(in_time[u]);
            long long fv  = rbit_point_query(in_time[v]);
            long long fl  = rbit_point_query(in_time[l]);
            long long fpl = (l == pl) ? 0 : rbit_point_query(in_time[pl]);

            results[rc++] = (int)(fu + fv - fl - fpl);
        }
    }

    *result_count = rc;
    return results;
}

/* ---------- main ---------- */

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));
    int n = parse_int(*(first_multiple_input + 0));
    int q = parse_int(*(first_multiple_input + 1));
    free(first_multiple_input);

    int** tree = malloc((n - 1) * sizeof(int*));
    for (int i = 0; i < n - 1; i++) {
        tree[i] = malloc(2 * sizeof(int));
        char** t = split_string(rtrim(readline()));
        tree[i][0] = parse_int(t[0]);
        tree[i][1] = parse_int(t[1]);
        free(t);
    }

    int** queries = malloc(q * sizeof(int*));
    for (int i = 0; i < q; i++) {
        queries[i] = malloc(3 * sizeof(int));
        char** t = split_string(rtrim(readline()));
        for (int j = 0; j < 3; j++) queries[i][j] = parse_int(t[j]);
        free(t);
    }

    int result_count;
    int* result = solve(n - 1, 2, tree, q, 3, queries, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", result[i]);
        if (i != result_count - 1) fprintf(fptr, "\n");
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

/* ---------- Helpers ---------- */

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
        if (!data) { data = '\0'; break; }
    }
    if (data_length == 0) { free(data); return NULL; }
    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = realloc(data, data_length);
        if (!data) data = '\0';
    } else {
        data = realloc(data, data_length + 1);
        if (!data) data = '\0';
        else data[data_length] = '\0';
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
