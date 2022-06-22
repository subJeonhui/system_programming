#include "unistd.h"
#include "stdio.h"

int main() {
    alarm(5);
    for (int i = 0; i < 10; i++) {
        printf("%d\n", i);
        if (i == 3) { pause(); }
    }
}