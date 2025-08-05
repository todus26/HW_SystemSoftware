#include <stdio.h>

void main()
{
	char i = -2;
	
	 printf("%d\n", i);  // 2 
	i = i >> -2;
	 printf("%d\n", i);  // -1
}