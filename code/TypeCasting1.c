#include <stdio.h>

int main (){
    int a = -1;
    unsigned int b = 2;
    short c = -1;
    unsigned short d= 2;
    long e = -1;
    unsigned long f = 2;
    long long g = -1;
    unsigned long long h = 2;
    char i = -1;
    unsigned char j = 2;

    if (a>b)
        printf("a>b\n");
    else
        printf("a>b ???\n");
    
    if(c>d)
        printf("c>d\n");
    else
        printf("c>d ???\n");

    if(e>f)
        printf("e>f\n");
    else
        printf("e>f ???\n");

    if(g>h)
        printf("g>h\n");
    else
        printf("g>h ???\n");

    if(i>j)
        printf("i>j\n");
    else
        printf("i>j ???\n");
    
}