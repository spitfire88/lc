#include <stdio.h>
#include <limits.h>

int add(int a, int b) { return (a + b);}
int sub(int a, int b) { return (a - b);}
int mul(int a, int b) { return (a * b);}
int div(int a, int b) { if (b != 0) return (a / b); return INT_MIN;}

int (*ops[])(int, int) = { add, sub, mul, div};

int main() {
    printf("%d\n", (*ops)(5, 6));
    printf("%d\n", (*(ops + 1))(5, 6));
    printf("%d\n", (*(ops + 2))(5, 6));
    printf("%x\n", (*(ops + 3))(5, 0));
    return 0;
}