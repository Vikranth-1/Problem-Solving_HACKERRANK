#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAXN 100005

int parent[MAXN];
int sz[MAXN];

int find(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];  /* path compression (halving) */
        x = parent[x];
    }
    return x;
}

void merge(int a, int b) {
    int ra = find(a);
    int rb = find(b);
    if (ra == rb) return;
    /* union by size: attach smaller tree under larger */
    if (sz[ra] < sz[rb]) { int t = ra; ra = rb; rb = t; }
    parent[rb] = ra;
    sz[ra] += sz[rb];
}

int main() {
    int n, q;
    scanf("%d %d", &n, &q);

    for (int i = 1; i <= n; i++) {
        parent[i] = i;
        sz[i] = 1;
    }

    while (q--) {
        char op[4];
        scanf("%s", op);

        if (op[0] == 'M') {
            int a, b;
            scanf("%d %d", &a, &b);
            merge(a, b);
        } else {
            int a;
            scanf("%d", &a);
            printf("%d\n", sz[find(a)]);
        }
    }

    return 0;
}
