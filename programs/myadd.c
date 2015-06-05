//Author: Anantha Raghuraman
//Date: Dec 01, 2014
//Filename: myadd.c
//Description:
//Two funtions in the library mydd to test load_library functionality


#include <xinu.h>

/*------------------------------------------------------------------------
 * add1  -  Adds one (1) to the argument and returns result
 *------------------------------------------------------------------------
 */
int32	add1 (
	int32	val	/* Value to be increased */
	)
{
	return (val + 1);
}

/*------------------------------------------------------------------------
 * add2  -  Adds two (2) to the argument and returns result
 *------------------------------------------------------------------------
 */
int32	add2 (
	int32	val	/* Value to be increased */
	)
{
	return (val + 2);
}
