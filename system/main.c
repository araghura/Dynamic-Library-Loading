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
extern int free_libraries;
extern library_loader libraries[max_libraries];

#define max_libraries 3

void testcase1(void);
void testcase2(int myvalue);
void testcase3(int myvalue);
void testcase4(void);
void testcase5(void);

process	main(void)
{
	/* Start the network */

	netstart();
	/* Code starts here */
	int myvalue =2;
		
	//Testcases

	//Testcase 1
	//Run Helloworld program
	testcase1();
	
	//Testcase 2
	//Load a library without global variables
	testcase2(myvalue);
	
	//Testcase 3
	//Load a library with global variables
	testcase3(myvalue);

	//Testcase 4
	//Unload a library
	testcase4();

	//Testcase 5
	//Dynamicall load a program
	testcase5();

	return OK;
}

void testcase1(void)
{
	//Run Helloworld program
	void *helloworld;

	kprintf("Testcase 1\n");
	kprintf("Now, running Hello World!\n");	

	helloworld = (void *)load_program("helloworld");

	kprintf("\n\n\n");
	if (helloworld == (void *)SYSERR)
	{
		kprintf("Could not load the program\n");
		return;
	}
	else
	{
		resume(create(helloworld,4096,20,"kprintfproc",2,0,NULL));
		kprintf("Testcase 1 was successful\n");
	}

	kprintf("\n\n\n");
	
}

void testcase2(int myvalue)
{
	//Load a library without global variables and use one of its functions
	int myval2;

	kprintf("Testcase 2\n");
	
	
	kprintf("I am now loading a library myadd without global variables\n");

	if(load_library("myadd") == SYSERR)
	{
		kprintf("load library has given syserr\n");
		return;
	}
	
	// Find the add1 function
	int32 (*add1)(int32) = find_library_function("add1");
	if((int32)add1 == SYSERR)) 
	{
		kprintf("library function add1 could not be found\n");
		return;
	}
	//Call the add1 function
	myval2 = add1(myvalue);
	
	kprintf("result of addition should be %d and is %d\n",(myvalue + 1),myval2);

	if(myvalue+1 == myval2)
	{
		kprintf("Testcase 2 was successful\n");
	}

	kprintf("\n\n\n");
}

void testcase3(int myvalue)
{
	//Load a library with global variables and use one of its functions
	int myval2;
	
	kprintf("Testcase 3\n");
	kprintf("I am now loading a library myadd2 with global variables\n");
	if(load_library("myadd2") == SYSERR)
	{
		kprintf("load library has given syserr\n");
		return;
	}
		
	int32 (*add3)(int32) = find_library_function("add3");
	if((int32)add3 == SYSERR) 
	{
		kprintf("library function add3 could not be found\n");
		return;
	}

	// Call the add3 function
	myval2 = add3(myvalue);
	kprintf("result of addition should be %d and is %d\n",(myvalue + 1),myval2);
	if(myvalue+1 == myval2)
	{
		kprintf("Testcase 3 was successful\n");
	}

	kprintf("\n\n\n");
}

void testcase4(void)
{
	//Unload a library

	kprintf("Testcase 4\n");
	
	kprintf("I am now, unloading myadd library\n");
	if(unload_library("myadd") ==  SYSERR)
	{
		kprintf("unload_library has given an error\n");
	}
	else
	{
		kprintf("Testcase 4 was successful\n");
	}

	kprintf("\n\n\n");
}

void testcase5(void)
{
	//Dynamically load shell program and run it
	kprintf("Testcase 5\n");
	kprintf("\n...creating a shell\n");
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) 
	{
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}

	kprintf("\n\n\n");
}