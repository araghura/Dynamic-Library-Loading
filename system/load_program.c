//Author: Anantha Raghuraman
//Date: Dec 01, 2014
//Filename: load_program.c
//Description:
//This loads the elf file into memory and resolves the addresses of global variables and system functions. the address at which the elf file was loaded.
//Input variable: Path to the program's elf file to be loaded.
//Return: Starting address of the memory location at which the program was loaded or SYSERR.

#include <xinu.h>
void*	load_program
(
	  char		*path		/* Path to the program 		*/
)
{
	char *filelocation;
	filelocation = (char *)elfloader(path);
	if (filelocation == (char *)SYSERR)
	{
		kprintf("File location is %u and invalid\n",filelocation);
		return (void*)SYSERR;
	}

	return elfparser(filelocation, 0, path);

}


