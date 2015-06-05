//Author: Anantha Raghuraman
//Date: Dec 01, 2014
//Filename: find_library_function.c
//Description:
// Finds and returns the pointer to the library function
// Input: Name of the library
// Output: Pointer to the library function if exists, or SYSERR.

#include <xinu.h>
void*	find_library_function
(
	  char		*name		/* Function name to locate	*/
)

{

	int i,j;
	char libname[100];
	for(i=0;i<100;i++)
	{
		libname[i] = (char)*(name+i);
		if(libname[i] == '\0')
		{
			break;
		}
	}

	for(i=0;i<3;i++)
	{
		if(libraries[i].used == 1)
		{
			for (j=0;j<libraries[i].num_functions;j++)
			{
				if(!strcmp(libraries[i].function_names[j],libname))
				{
					kprintf("Function name is %s\n", libname);
					kprintf("Function location is %u\n", (uint32)libraries[i].function_ptrs[j]);
					return (void *)libraries[i].function_ptrs[j];
				}
			}
		}
	}

		
	return (void*)SYSERR;
}
