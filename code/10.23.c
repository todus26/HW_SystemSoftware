#include <stdio.h>

void main() {
	char i = -17;
	i = i/8;
	printf("%d\n", i);
	i = -17;
	i = i >> 3;
	printf("%d\n", i);
}