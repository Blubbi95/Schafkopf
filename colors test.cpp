#include <stdio.h>
#include <windows.h> 


void color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}


int main()
{
	int k;
	
	
	// you can loop k higher to see more color choices
	for(k = 1; k < 255; k++)
	{
		color(k);
		printf("%3i  %s\n", k, "This is a color");
	}
	getchar();  // wait
	return 0;
}
