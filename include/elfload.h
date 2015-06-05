int xinu_in_mem; //checks if xinu elf is already there in memory
char *load_xinu; //if xinu elf is there in memory, then this points to that
extern void *elfparser(char * loction, int type, char *path);//function to parse elf files
extern void *elfloader(char *path); //function to load elf files
int free_libraries; //Number of library places free
int sem_counter;
typedef struct{
	char lib_name[100]; //Location of this library in memory
	int used; //1 : used, 0: ununsed
	int num_functions; // Num functions in this library
	char function_names[10][100]; //names of functions in this lib
	char *function_ptrs[10];
	}library_loader;
library_loader libraries[3];

	
