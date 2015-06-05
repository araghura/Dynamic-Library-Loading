//Author: Anantha Raghuraman
//Date: Dec 01, 2014
//Filename: load_library.c
//Description:
//This loads the elf file of library into memory, resolves all the addresses of global variables and system functions and 
// adds the library and functions to a global datastructure. 
//Input variable: Path to the library elf file to be loaded.
//Return: It returns OK if library was loaded properly, else returns SYSERR.

#include <xinu.h>
syscall	load_library
(
	  char		*path		/* Path to the library 		*/
)

{
	char *filelocation;
	filelocation = (char *)elfloader(path);
	if (filelocation == (char *)SYSERR)
	{
		kprintf("File location is %u and invalid\n",filelocation);
		return SYSERR;
	}
	
	if(filelocation == (char *)elfparser(filelocation,1,path))
	{
		kprintf("proper file location is returned\n");
		return OK;
	}
	else
	{
		kprintf("Some problem with returning the file location\n");
		return SYSERR;
	}
}
