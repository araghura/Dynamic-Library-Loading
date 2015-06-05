//Author: Anantha Raghuraman
//Date: Dec 01, 2014
//Filename: elfparser.c
//Description:
//This parses the elf file present at location (input).
//If type is 0, elfparser was called by load_program, then it returns the address of main function (or the only function)
//if type is 1, elfparser was called by load_library, then it changes the global library DS stored in the memory and returns location
//path name stores the library name
//Input variables: Location of loaded program file / library, type and path to the library if type is 1)
//Return: location of loaded program after resolving all the addresses of global variables and functions.

#include<xinu.h>

#define max_libraries 3
#define max_func_p_lib 10

void *elfparser(char *location, int type, char *path)
{	

	Elf32_Ehdr *file_header;
	int i,count,bss,symstrtab;
	count = 0;
	extern int xinu_in_mem; //To check if OS is already loaded in memory
	extern char *load_xinu; //Address of xinu

	extern library_loader libraries[max_libraries];
	extern int free_libraries;

	//READ ELF HEADER AND PARSE
	file_header = (Elf32_Ehdr *)location;
	count = file_header->e_shoff;
	int num_sections = file_header->e_shnum;

	//memory to store all section headers
	Elf32_Shdr *section_headers[num_sections];
	char section_names[num_sections][100];
	uint32 tab,sym_tab, text;
	char *string_start;

	//READ ALL THE SECTIONS HEADERS AND STORE IT IN section_headers ARRAY
	for(i=0;i<num_sections;i++)
	{
		section_headers[i] = (Elf32_Shdr *)(location + count);
		count = count + file_header->e_shentsize;
	}

    //Starting address of the string that contains all section names one after the other
	string_start = (char *)section_headers[file_header->e_shstrndx]->sh_offset;


	for(i=0;i<num_sections;i++)
	{
		//FOR EACH SECTION, get the SECTION NAME FROM STRING TABLE
		for(tab = 0;tab<100;tab ++)
		{
			section_names[i][tab]= (char)*(location+(uint32)string_start+tab+(uint32)(section_headers[i]->sh_name));
					
			if(section_names[i][tab] == '\0')
			{
				break;
			}
		}
	
	
		// IDENTIFY SYMBOL TABLE SECTION, BSS, STRING TABLE AND TEXT SECTION
	
		if(!strcmp(section_names[i],".symtab"))//Symbol Table Section
		{
			sym_tab = i;
		}
		
		if(!strcmp(section_names[i],".text"))//Text
		{
			text = i;
		}
		
		if(!strcmp(section_names[i],".bss"))//bss
		{
			bss = i;
		}

		if(!strcmp(section_names[i],".strtab")) //String Table
		{
			symstrtab = i;
		}

	}
	
	// PARSE THE SYMBOLE TABLE
	uint32 sym_entries;

	//No: of symbol table entries
	sym_entries = section_headers[sym_tab]->sh_size/section_headers[sym_tab]->sh_entsize;

	Elf32_Sym *the_symbol_table[sym_entries];
	
	count = 0;
	for(tab =0;tab<sym_entries;tab++)
	{
		the_symbol_table[tab] = (Elf32_Sym *)(location + (uint32)(section_headers[sym_tab]->sh_offset) + count);
		count = count + section_headers[sym_tab]->sh_entsize;
	}
	

//If xinu is already in memory then, use it 
//Otherwise load xinu
//	LOAD XINU ELF
	if(! xinu_in_mem)
	{
		xinu_in_mem = 1;	
		load_xinu = (char *)elfloader("xinu.elf");
		if(load_xinu == (char *)SYSERR)
		{
			kprintf("could not load xinu image\n");
			return (void *)SYSERR;
		}
	}
	else
	{
		kprintf("Xinu image already exists in memory\n");
		if(load_xinu == (char *)SYSERR)
		{
			kprintf("Invalid xinu location\n");
			return (void *)SYSERR;
		}
		kprintf("xinu is located at %x\n",(uint32)load_xinu);
	}

	Elf32_Ehdr *xinu_file_header;
	uint32 xinu_count,xinu_symstrtab;
	xinu_count = 0;

	//READ ELF HEADER of XINU AND PARSE
	xinu_file_header = (Elf32_Ehdr *)load_xinu;

	xinu_count = xinu_file_header->e_shoff;
	int xinu_num_sections = xinu_file_header->e_shnum;
	Elf32_Shdr *xinu_section_headers[xinu_num_sections];
	char xinu_section_names[xinu_num_sections][100];
	int xinu_tab,xinu_sym_tab,xinu_text;
	char *xinu_string_start;
 	
	//READ ALL THE SECTIONS HEADERS of XINU AND STORE IT IN xinu_section_headers ARRAY
	for(i=0;i<xinu_num_sections;i++)
	{
	
		xinu_section_headers[i] = (Elf32_Shdr *)(load_xinu + xinu_count);	
		xinu_count = xinu_count + xinu_file_header->e_shentsize;

	}

	xinu_string_start = (char *)xinu_section_headers[xinu_file_header->e_shstrndx]->sh_offset;

	for(i=0;i<xinu_num_sections;i++)
	{
	//FOR EACH SECTION, get the SECTION NAME FROM STRING TABLE
		for(xinu_tab = 0;xinu_tab<100;xinu_tab ++)
		{
			xinu_section_names[i][xinu_tab]= (char)*(load_xinu+(uint32)xinu_string_start+(uint32)xinu_tab+(uint32)(xinu_section_headers[i]->sh_name));
				
			if(xinu_section_names[i][xinu_tab] == '\0')
			{
				break;
			}
		}
			
	// IDENTIFY SYMBOL TABLE SECTION, STRING TABLE AND TEXT SECTION of XINU
		if(!strcmp(xinu_section_names[i],".symtab"))//Symbol Table Section
		{
			xinu_sym_tab = i;
		}
		
		if(!strcmp(xinu_section_names[i],".text"))//Symbol Table Section
		{
			xinu_text = i;
		}

		if(!strcmp(xinu_section_names[i],".strtab"))//String Table Section
		{
			xinu_symstrtab = i;
		}


	}

	
	// PARSE THE SYMBOLE TABLE of XINU
	uint32 xinu_sym_entries;
	xinu_sym_entries = xinu_section_headers[xinu_sym_tab]->sh_size/xinu_section_headers[xinu_sym_tab]->sh_entsize;
	Elf32_Sym *xinu_symbol_table[xinu_sym_entries];
	
	xinu_count = 0;
	for(xinu_tab =0;xinu_tab<xinu_sym_entries;xinu_tab++)
	{
		xinu_symbol_table[xinu_tab] = (Elf32_Sym *)(load_xinu + xinu_section_headers[xinu_sym_tab]->sh_offset + xinu_count);
		xinu_count = xinu_count + xinu_section_headers[xinu_sym_tab]->sh_entsize;
	}


//
// 	PARSE THE RELOCATION TABLE AND CHANGE THE ENTRIES TO THE RIGHT VALUE

	int j,k;
	Elf32_Sym *a_sym_entry;
	int32 *rel_locn_final,*rel_locn_final_t2;
	int32 addend;
	char *bss_data;
	char *value, *rel_locations;
	bss_data = getmem(section_headers[bss]->sh_size);

	if(bss_data == (char *)SYSERR)
	{
		kprintf("not enough memory to allocate for bss variables\n");
		return (void *)SYSERR;
	}

	for(i=0;i<num_sections;i++)
	{
		if(!strcmp(section_names[i],".rel.text")) //If Relocation section
		{
			int num_entries = section_headers[i]->sh_size/section_headers[i]->sh_entsize; //Number of relocation entries
			if(num_entries == 0)
			{
				kprintf("No entries in the relocation table\n");
				continue;
			}

			count = section_headers[i]->sh_offset;
			Elf32_Rel *reloc_entries[num_entries];
			uint32 r_symbol[num_entries];
			uint32 r_type[num_entries];
			char symbolname[100];
			char xinusymbolname[100];
	
			//Parse Each relocation entry
			for (j=0;j<num_entries;j++)
			{
				reloc_entries[j] = (Elf32_Rel *)(location+count);
				count = count + section_headers[i]->sh_entsize;
				//r_offset, r_info
	
				r_symbol[j] = (uint32)(reloc_entries[j]->r_info >> 8);
				r_type[j] = (unsigned char)reloc_entries[j]->r_info;
					
				a_sym_entry = the_symbol_table[r_symbol[j]];
	
				//Check if the symbol table entry's section index is not zero	
				if(a_sym_entry->st_shndx != 0)
				{
					if(!strcmp(section_names[a_sym_entry->st_shndx],".bss")) //BSS section
					{
						rel_locations = location + (uint32)(section_headers[text]->sh_offset) + (uint32)(reloc_entries[j]->r_offset);
						value = (char *)a_sym_entry->st_value;
						value = (uint32)value + bss_data;
					}
					else
					{
						//For all relocation table entries, do address resolution
						//rel_locations: The section where needs to have the correct value changed
						rel_locations = location + section_headers[a_sym_entry->st_shndx]->sh_offset;

						//Value: Contains address from begining of file at which resolution needs to be done
						value = (char *)a_sym_entry->st_value;

						value = (char *)((uint32)value + (uint32)rel_locations);

						//Find the absolute location from location
						rel_locations = location + (uint32)(section_headers[text]->sh_offset) + (uint32)(reloc_entries[j]->r_offset);
	//					kprintf("case 1 value is %u, location to put is %x\n", (uint32)value,(uint32)locations);
					}
				}

				else
				{
					//If symbol table section index is zero
					if(a_sym_entry->st_shndx ==0)
					{

						//rel_locations: The section where needs to have the correct value changed	
						rel_locations = location + (uint32)(section_headers[text]->sh_offset) + (uint32)(reloc_entries[j]->r_offset);
	
						//Get the symbol name of the relocation entry in the ELF file, for which we need address resolution
						for(tab = 0;tab<100;tab++)
						{
							symbolname[tab]= (char)*(location + (uint32)(section_headers[symstrtab]->sh_offset)+tab+(uint32)(a_sym_entry->st_name));
						
							if(symbolname[tab] == '\0')
							{
								break;
							}
						}

	
						//For the same relocation entry symbol name as above, get the address of the symbol from XINU's symbol table
						for(xinu_tab =0;xinu_tab<xinu_sym_entries;xinu_tab++)
						{
							for(tab=0;tab<100;tab++)
							{
								xinusymbolname[tab] = (char)*(load_xinu+(uint32)(xinu_symbol_table[xinu_tab]->st_name) + (uint32)(xinu_section_headers[xinu_symstrtab]->sh_offset)+tab);
								
								if(xinusymbolname[tab] == '\0')
								{
									break;
								}
							}

							//Check if we got the right symbol table entry
							if(!strcmp(xinusymbolname,symbolname))
							{
								value = (char *)(xinu_symbol_table[xinu_tab]->st_value);
								break;
							}
						}

					}
				}
			
				if(r_type[j] == 1) //Relocation type 1
				{
				
					rel_locn_final = (int32 *)locations;
					addend = (int32) *rel_locn_final;
					*(rel_locn_final) = (uint32)((uint32)value + (int32)addend);
				}
	
				if(r_type[j] == 2) //Relocation type 2
				{
					//Get the address from xinu.elf and update
					rel_locn_final_t2 = (int32 *)locations;
					addend = *rel_locn_final_t2;
					*(rel_locn_final_t2) = (int32)((int32)addend + (uint32)value - (uint32)locations);	
				}
	
	
			}
		}
	}			
	
//Finished completely changing the relocation table

//If we are loading a dynamic library into memory, make sure that the function names are not repeated

	int num_func = 0; //No: of functions that are defined in the symbol table
	char func_name[100];
	int main_entry;

	char pathname[100];
	for(j = 0;j<100;j++)
	{
		pathname[j]= (char)*(path+j);
		if(pathname[j] == '\0')
		{
			break;
		}
	}

	
	if(type == 0) //Elfparser called by load program
	{
	// check if there is only one function in the program, if more than one function, return error
		for(tab = 0; tab<sym_entries ;tab++)
		{
			//check if the type is FUNC (or 2)
			if((int)((unsigned char)the_symbol_table[tab]->st_info & 0xf) == 2)
			{	
				for(j = 0;j<100;j++)
				{
					func_name[j]= (char)*(location + (uint32)(section_headers[symstrtab]->sh_offset)+j+(uint32)(the_symbol_table[tab]->st_name));
					if(func_name[j] == '\0')
					{
						break;
					}
				}

				num_func++; 
				//if there are more than 1 fns or if the func's name is not main then, report error
				if(num_func > 1 || strcmp(func_name,"main"))
				{
					kprintf("Error: num_func is %d and func_name is %s\n",num_func,func_name);
					return (void *)SYSERR;
				}
				main_entry = tab;
			}
		}
		// return the starting address of main
		return (void *)(location + (uint32)section_headers[the_symbol_table[main_entry]->st_shndx]->sh_offset + the_symbol_table[main_entry]->st_value);
	}

	else if(type == 1) //Elfparser called by load library
	{
		// Check that there are free libraries 
		if(free_libraries == 0)
		{
			kprintf("Three libraries are already loaded\n");
			return (void *)SYSERR;
		}

		// check that the current library is not already loaded
		for(i = 0;i<max_libraries;i++)
		{
			if(libraries[i].used == 1)
			{
				if(! strcmp(libraries[i].lib_name,pathname))
				{	
					kprintf("Library %s is already loaded in memory\n",pathname);
					return (void *)SYSERR;
				}
			}
		}

		//Find a library structure that is unused
		for(j=0;j< max_libraries;j++)
		{
			if(libraries[j].used == 0)
			{
				free_libraries--;
				libraries[j].used =1;
				strcpy(libraries[j].lib_name, pathname);
				i = j;
				break;
			}
		}

		//Check that the library does not have more than 10 functions and load the library into memory
		for(tab = 0; tab<sym_entries ;tab++)
		{
			if((int)((unsigned char)the_symbol_table[tab]->st_info & 0xf) == 2)//implies that it is of typ "FUNC"
			{	
				if(libraries[i].num_functions == max_func_p_lib)
				{
					kprintf("Trying to load more than 10 functions in a library\n");
					return (void *)SYSERR;
				}	
				for(j = 0;j<100;j++)
				{
					func_name[j]= (char)*(location + (uint32)(section_headers[symstrtab]->sh_offset)+j+(uint32)(the_symbol_table[tab]->st_name));
					if(func_name[j] == '\0')
					{
						break;
					}
				}

				strcpy(libraries[i].function_names[libraries[i].num_functions],func_name);

				libraries[i].function_ptrs[libraries[i].num_functions] = location + (uint32)section_headers[the_symbol_table[tab]->st_shndx]->sh_offset + the_symbol_table[tab]->st_value;
				(libraries[i].num_functions)++;
			}
		}
		
		if(libraries[i].num_functions ==0)
		{
			kprintf("The library does not contain any functions\n");
			return (void *)SYSERR;
		}

		//Check if any of the library functions are alrady loaded
		for(tab=0;tab<max_libraries;tab++)
		{
			if(libraries[tab].used == 1 && tab !=i)
			{
				for(k = 0; k< libraries[i].num_functions; k++)
				{
					for(j = 0; j<libraries[tab].num_functions;j++)
					{
					
						if(!strcmp(libraries[i].function_names[k],libraries[tab].function_names[j]))
						{
							kprintf("The function %s exists in multiple libraries\n",libraries[tab].function_names[j]);
							return (void *)SYSERR;
						}
					}
				}	
			}
		}
		return (void *)location;
	}
	else
	{
		return (void *)SYSERR;
	
	}
}
