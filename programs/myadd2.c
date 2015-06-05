//Author: Anantha Raghuraman
//Date: Dec 01, 2014
//Filename: myadd2.c
//Description:
//Two funtions in the library mydd2 and a global variable to test load_library functionality

#include <xinu.h>
int anotherval = 2;
/*------------------------------------------------------------------------
 * add1  -  Adds one (1) to the argument and returns result
 *------------------------------------------------------------------------
 */
int32	add3 (
	int32	val	/* Value to be increased */
	)
{
	kprintf("I am in add1\n");
	return (val + 1);
}

/*------------------------------------------------------------------------
 * add2  -  Adds two (2) to the argument and returns result
 *------------------------------------------------------------------------
 */
int32	add4 (
	int32	val	/* Value to be increased */
	)
{
//	return (val + 2);
	kprintf("I am in add2\n");
	return (val + anotherval);
//	return (val + 2);
}
