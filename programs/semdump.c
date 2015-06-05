//Author: Anantha Raghuraman
//Date: Dec 01, 2014
//Filename: semdump.c
//Description:
//Get and print the semaphore table

#include <xinu.h>
#include <string.h>
#include <stdio.h>


shellcmd semdump(int nargs, char *args[]) 
{
	int i;
	extern int sem_counter;
	struct sentry *a_sem;
	if(nargs>1 || nargs == 0)
	{
		return SYSERR;
	}
	kprintf("\nEntry\t");
	kprintf("State\t");
	kprintf("Count\t");
	kprintf("Queue\n");
	for(i = 0;i<NSEM;i++)
	{	
		a_sem = &semtab[i];
		kprintf("%d\t",i);
		if(a_sem->sstate == 0)
		{
			kprintf("S_FREE\t");
		}
		else
		{
			kprintf("S_USED\t");
		}
		kprintf("%d\t",a_sem->scount);
		kprintf("%d\t",a_sem->squeue);
		kprintf("\n");
	}
	kprintf("\n");
	return 0;
}



