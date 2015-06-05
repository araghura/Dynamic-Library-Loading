//Author: Anantha Raghuraman
//Date: Dec 01, 2014
//Filename: unload_library.c
//Description:
//This removes a dynamically loaded library and frees up a spot in global library database
//Input variable: path to the library to be unloaded
//Return: Ok if successful, SYSERR otherwise.

#include<xinu.h>

syscall	unload_library(
	  char *path		/* Path to the library 		*/
        )
{
	char pathname[100];
	int i,j;
	extern library_loader libraries[3];
	extern free_libraries;
	for(j = 0;j<100;j++)
	{
		pathname[j]= (char)*(path+j);
		if(pathname[j] == '\0')
		{
			break;
		}
	}
	for(i= 0;i<3;i++)
	{
		if(libraries[i].used == 1)
		{
			if(! strcmp(libraries[i].lib_name,pathname))
			{
				libraries[i].used = 0;
				libraries[i].num_functions = 0;
				free_libraries++;
				return OK;
			}
		}
	}
	return SYSERR;
}
