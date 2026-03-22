#include <stdio.h>

int birthdayCakeCandles(int arr[], int n) {
    int max = arr[0];
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
            count = 1;
        } else if (arr[i] == max) {
            count++;
        }
    }

    return count;
}

int main() {
    int n;
    scanf("%d", &n);

    int arr[n];

    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    int result = birthdayCakeCandles(arr, n);
    printf("%d\n", result);

    return 0;
}
