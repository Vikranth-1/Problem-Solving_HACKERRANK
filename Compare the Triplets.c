#include <stdio.h>

int main() {
    int a[3], b[3];
    int aliceScore = 0, bobScore = 0;

    for (int i = 0; i < 3; i++) {
        scanf("%d", &a[i]);
    }

    for (int i = 0; i < 3; i++) {
        scanf("%d", &b[i]);
    }

    for (int i = 0; i < 3; i++) {
        if (a[i] > b[i]) {
            aliceScore++;
        } else if (a[i] < b[i]) {
            bobScore++;
        }
    }

    printf("%d %d\n", aliceScore, bobScore);

    return 0;
}
