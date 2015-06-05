//Author: Anantha Raghuraman
//Date: Dec 01, 2014
//Filename: helloworld.c
//Description:
// A hello world program to test load program 


#include <xinu.h>
int trial;

int	main (
	int	argc,	/* Number of arguments	*/
	char	*argv[]	/* Arguments array	*/
	)
{	
	trial = 5;
	trial = 6;
	kprintf("Hello World! %d\n",trial);
	return 0;
}
