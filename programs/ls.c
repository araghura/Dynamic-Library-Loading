/* ls.c - ls */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * ls - list contents of a directory
 *------------------------------------------------------------------------
 * B
 * B
 */
shellcmd ls(int nargs, char *args[]) 
{
	int i;
	char dirname[100];
	struct rfdirent 
	{
		byte	d_type;
		byte	d_name[256];
	};
	struct rfdirent rfdentry;
//	kprintf("Number of arguments is %d\n",nargs);
	int32 dirfd; 
	int32 rc;

	if(nargs == 1)
	{
		dirfd = open(RFILESYS, ".", "ro");
		
	}
			
	else if(nargs == 2)
	{
		for(i=0;i<100;i++)
		{
			dirname[i] = *(args[1]+i);
			if(*(args[1]+i) == '\0')
			{
				break;
			}
		}
		dirfd = open(RFILESYS,dirname,"ro");
	}
	else
	{
		kprintf("More than 1 argument was given\n");
		return SYSERR;
	}
	rc = read(dirfd, (char *)&rfdentry, sizeof(struct rfdirent));

	while(rc !=0)
	{
		if(rc == SYSERR) 
		{ /* error accured while reading */
			kprintf("Error occured while reading\n");
			return SYSERR;
		} 
		else
		{
			if(rfdentry.d_type == 1) //file etc
			{
				kprintf("\n%s",rfdentry.d_name);
			}
			else if(rfdentry.d_type == 2) // directory
			{
				kprintf("\n%s/",rfdentry.d_name);
			}
				rc = read(dirfd, (char *)&rfdentry, sizeof(struct rfdirent));
		}
	}
	kprintf("\n");
	if(rc ==0)
	{
//		kprintf("Reached the end of the list\n");
	}

	close(dirfd);
	return 0;
}
