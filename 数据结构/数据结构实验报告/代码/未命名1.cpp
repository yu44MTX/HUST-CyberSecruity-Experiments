#include<stdio.h>

void swap(int &a)
{
	a++;
}
int main()
{	
	int a=4;
	swap(a);
	printf("%d",a);
	return 0;
}
