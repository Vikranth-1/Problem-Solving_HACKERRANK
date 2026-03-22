#include <stdio.h>
#include <stdlib.h>

int diagonalDifference(int n, int arr[n][n]) {
    int primary_sum = 0;
    int secondary_sum = 0;

    for (int i = 0; i < n; i++) {
        primary_sum += arr[i][i];
        secondary_sum += arr[i][n - 1 - i];
    }

    return abs(primary_sum - secondary_sum);
}

int main() {
    int n;
    scanf("%d", &n);

    int arr[n][n];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &arr[i][j]);
        }
    }

    int result = diagonalDifference(n, arr);
    printf("%d\n", result);

    return 0;
}
