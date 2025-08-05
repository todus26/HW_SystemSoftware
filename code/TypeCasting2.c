#include <stdio.h>

void print_bits(void *ptr, int size) {
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    printf("\n");
}

int main() {
    signed short b = -10;
    unsigned short c = -10;
    signed int a = -10;
    signed int d = -10;

    printf("b: ");
    print_bits(&b, sizeof(b));
    printf("c: ");
    print_bits(&c, sizeof(c));
    printf("a: ");
    print_bits(&a, sizeof(a));
    printf("d: ");
    print_bits(&d, sizeof(d));

    printf("%d, %d \n", b, (unsigned short)b);
    printf("%d, %d \n", (signed short)c, c);
    printf("%d, %d \n", a, (unsigned int)a);
    printf("%d, %d \n", (signed int)d, d);
    printf("b==c : %d\n", b==c);
    printf("a==d : %d\n", a==d);
}