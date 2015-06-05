Design a feature to dynamically load libraries and programs in XINU OS environment
==================================================================================

Author: Anantha Raghuraman 
--------------------------

# Goal of the Project

i) To design a feature to dynamically load programs and libraries in XINU OS environment.

ii) The XINU OS is assumed to be implemented according to the methodology described in "Operating System Design - The XINU Approach", Second Edition, 2015 by Prof. Douglas Comer

iii) The load program design was made to dynamically load any program with only one function (main). 

iv) The load library design was made to dynamically load upto 3 libraries, each with 10 functions. Repetition  of a single function across / within a library will not be allowed.


Design and Files
-----------------
Program Loading: This is carried out by the function load_program. 
	Input: Path (from programs folder) to the program. The remote file system is configured to look for functions in the "Program" directory. So, all the programs and libraries are compiled (into elf files) and stored there. More information about how to configure the remote file system can be obtained from "Operating System Design - The XINU Approach", Second Edition, 2015 by Prof. Douglas Comer.

	Return value: The function returns a void pointer to the location where the program is loaded after resolving all system functions' and global variables' addresses. Or, the function returns a SYSERR if anything goes wrong.

Library Loading: This is carried out by the function load_library. 
	Input: Path (from programs folder) to the program. The remote file system is configured to look for functions in the "Program" directory. So, all the programs and libraries are compiled (into elf files) and stored there. More information about how to configure the remote file system can be obtained from "Operating System Design - The XINU Approach", Second Edition, 2015 by Prof. Douglas Comer.

	Return value: The function returns a void pointer to the location where the library is loaded into memory after resolving all system functions' and global variables' addresses. Or, the function returns a SYSERR if anything goes wrong. There is a global data structure called libraries, that stores the set of libraries, the functions in them and their addresses.

Library function lookup: This is carried out by the function find_library_function.
	Input: The name of the function

	Return value: Function pointer to the library function (if exists), else SYSERR. 

Unloading a Library: This is carried out by the function unload_library.
	Input: Path to the library that has to be removed (from the programs folder).

	Return value: OK, if successfull and SYSERR otherwise.


Implementation
--------------

# load_program and load_library

The load_program and load_library functions, both use two main functions:

a) elfloader: 
i) This function takes one input, the name of the elf file to load. It just finds out the size of the elf file and then loads it onto the memory after getting enough memory using get_mem. 

ii) If the file could not be opened or if enough memory is not available or if there is some problem with reading the entire file into memory, then we get SYSERR.

b) elfparser: 
) This function takes three inputs. The first is the location of the file in the memory (after doing elfloader) and second is a binary integer (0 or 1) that indicates where we are doing load_program (0) or load_library(1). and the third is is the name of the library. In case of load_program, we will not use the second variable at all. 

ii) For both load_program and load_library, the elfparser.c resolves all the relocatable variables in the relocation sections, after parsing through various sections of the elf file. The difference arises in what happens after all the relocatables are resolved.

iii) This also loads the xinu elf onto the memory to resolve the relocatables. elfparser checks for errors like problems with loading xinu elf and if enough memory is not available to resolve uninitialized global variables. 

iv) In the case of load_program, it checks that there is only one function and then returns the pointer to the beginning of that function. Otherwise, it returns syserr. In case of load_library, it checks that the library is not already loaded onto the memory and that we have space to upload (all three slots must not be taken up). 

v) We have a global variable (libraries) that acts like a table look up for the library functions and their addresses. So, the elfparser updates the entry in the structure libraries. There is a field in the libraries structure (used) that indicates if a library is free or not, it is also set to 1. While it updates the global variable, it checks that there are at most ten functions and that the function names are not already present in other libraries. Then, after updating, it returns the location of the file in memory (for safety checking).

# find_library_function
This goes through the loaded libraries and checks if the function is present. If the function is present, then it returns the starting address, or else returns SYSERR.

# unload_library
This updates the global libraries C structure to indicate that the library is not being used. It also updates the count of no: of libraries being used stored in the variable free_libraries(reduces it by one).


Header files
------------
i) elf.h contains the definitions of all the structures to be used to parse elf files.

ii) elfload.h contains global variables and definitions of structures used in elfloader and elfparser functions.

iii) All the other header files must be created as mentioned in "Operating System Design - The XINU Approach", Second Edition, 2015 by Prof. Douglas Comer.

Compilation
-----------
i) Generate all the OS files as described in "Operating System Design - The XINU Approach", Second Edition, 2015 by Prof. Douglas Comer. 

ii) Set up a remote file system connection between programs directory in the local machine and another machine in which XINU OS runs.

iii) The Makefile for running the xinu OS is described in the book mentioned in part i) of this section. That can be used to compile XINU OS and run the system.

iv) Make sure that the programs and libraries in the programs folder are compiled before compiling the OS.


Test Run
--------
i) A set of test cases is provided in main.c in the systems folder. 
ii) A few libraries are designed and compiled ELF files are stored in the programs directory.
iii) Couple of programs are also designed and compiled ELF file is stored in programs directory.

