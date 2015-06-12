//Author: Anantha Raghuraman Date: Dec 01, 2014 Filename: elfloader.c
//Description: This loads the elf file into memory and returns the address at
//which the elf file was loaded. 
//Input variable: Path to the elf file to be loaded. 
//Return: Starting address of the memory location at which the program
//or library was loaded.

#include<xinu.h>

void *elfloader(char *path)
{
	
	/*Load the program into memory first*/

	int fd = open(RFILESYS,path, "or");
	if (fd == SYSERR)
	{
		kprintf("Could not open the file\n");
		return (void *)SYSERR;
	}
	
	int32 filesize = control(RFILESYS, RFS_CTL_SIZE, fd, 0);

	char *filestart;
	
	filestart = getmem(filesize);

	if(filestart == (char *)SYSERR)
	{
		kprintf("Not enough memory to load the entire file\n");
		return (void *)SYSERR;
	}


	int rc = read(fd, filestart, filesize);

	if(rc == filesize)
	{
		kprintf("successfully read the file into memory at %u\n");
	}
	else
	{
		kprintf("Error reading file into memory\n");
		close(fd);
		return (void *)SYSERR;
	}

	close(fd);
return (void *)filestart;
}


