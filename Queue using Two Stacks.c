#include <stdio.h>
#include <stdlib.h>

#define MAX 100000

int stack1[MAX], stack2[MAX];
int top1 = -1, top2 = -1;

void moveStacks() {
    if(top2 == -1) {
        while(top1 != -1) {
            stack2[++top2] = stack1[top1--];
        }
    }
}

int main() {

    int q;
    scanf("%d", &q);

    for(int i = 0; i < q; i++) {

        int type;
        scanf("%d", &type);

        if(type == 1) {   // Enqueue

            int x;
            scanf("%d", &x);
            stack1[++top1] = x;

        }
        else if(type == 2) {   // Dequeue

            moveStacks();
            if(top2 != -1)
                top2--;

        }
        else if(type == 3) {   // Print front

            moveStacks();
            if(top2 != -1)
                printf("%d\n", stack2[top2]);

        }
    }

    return 0;
}
