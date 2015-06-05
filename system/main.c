//Author: Anantha Raghuraman
//Date: Dec 01, 2014
//Filename: main.c
//Description:
//This tests the functioning of all the files
//Input variable: Nothing
//Return: OK if everything works fine, SYSERR otherwise.

#include <xinu.h>
#include <stdio.h>

extern uint32 nsaddr;

#define max_libraries 3

process	main(void)
{
	/* Start the network */

	netstart();
	/* Code starts here */
	void *helloworld;
	int myvalue =2;
	int myval2;
	extern int free_libraries;
	extern library_loader libraries[max_libraries];



//Testcases

    //Testcase 1
	//Load helloworld program into memory
	kprintf("Testcase 1\n");
	kprintf("Now, running Hello World!\n");	

	helloworld = (void *)load_program("helloworld");

	kprintf("\n\n\n");
	if (helloworld == (void *)SYSERR)
	{
		kprintf("Could not load the program\n");
		return;
	}

	resume(create(helloworld,4096,20,"kprintfproc",2,0,NULL));

	kprintf("\n\n\n");

	//Testcase 2
	kprintf("Testcase 2\n");
	kprintf("I am now loading a library myadd2 with global variables\n");
	if(load_library("myadd2") == SYSERR)
	{
		kprintf("load library has given syserr\n");
		return;
	}

	//Find the add3 function 
	int32 (*add3)(int32) = find_library_function("add3");
	if((int32)add3 == SYSERR) 
	{
		return;
	}

	// Call the add3 function
	myval2 = add3(myvalue);
	kprintf("result of addition should be 3 and is %d\n",myval2);


	//Testcase 3
	kprintf("\n\n\n");
	kprintf("Testcase 3\n");
	kprintf("I am now loading a library myadd without global variables\n");

	if(load_library("myadd") == SYSERR)
	{
		kprintf("load library has given syserr\n");
		return;
	}
	
	// Find the add1 function
	int32 (*add1)(int32) = find_library_function("add1");
	if((int32)add1 == SYSERR) {
		return;
	}
	//Call the add1 function
	myval2 = add1(myvalue);
	kprintf("result of addition should be 3 and is %d\n",myval2);

	//Testcase 4
	kprintf("\n\n\n");
	kprintf("Testcase 4\n");
	kprintf("I am now, unloading myadd library\n");
	if(unload_library("myadd") ==  SYSERR)
	{
		kprintf("unload_library has given an error\n");
	}
	

	//Testcase 5
	kprintf("\n\n\n");
	kprintf("Testcase 5\n");
	kprintf("\n...creating a shell\n");
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
}
