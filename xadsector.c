#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "xadlib.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
int verbose = 0;
char* parameter;
int standard = 0;
char swap_name[20] = {'\0'};
int Block_Size = 0;
int Total_Block = 0;
int is_regular_file = 0;
int is_regular_dir = 0;
void generate_mount_dir()
{
 system("rm -rf mount_dir.txt");
 system("df | awk '/\\//{print $6}' >> mount_dir.txt");
}

void find_block_size_count(char* disk_space_name)
{
 char p[15] = {'\0'};
 FILE* stream;
 char command_block_size[100] = "dumpe2fs -h ";
 strcat(command_block_size, disk_space_name);
 strcat(command_block_size, " | grep \"Block size\" | awk \'{print $3}\' > Block_size.txt");
 system(command_block_size);

 char command_block_count[100] = "dumpe2fs -h ";
 strcat(command_block_count, disk_space_name);
 strcat(command_block_count, " | grep \"Block count\" | awk \'{print $3}\' >> Block_size.txt");
 system(command_block_count);
 stream = fopen("Block_size.txt", "r+");
 if(stream < 0 )
	{
	 printf("open Block_size.txt failed.\n");
	 return ;
	}
if( fgets(p, 15, stream) != NULL ) 
	{
	 int i=0;
	  for(;i<15;i++)
		{
		  if(p[i] == '\n')                    //get rid of the '\n' at the end of each line in mount_dir.txt
		  	{
				p[i] = '\0';
				break;
			}
		}
	}
Block_Size = atoi(p);
if( fgets(p, 15, stream) != NULL ) 
	{
	 int i=0;
	  for(;i<15;i++)
		{
		  if(p[i] == '\n')                    //get rid of the '\n' at the end of each line in mount_dir.txt
		  	{
				p[i] = '\0';
				break;
			}
		}
	}
 Total_Block = atoi(p);
}



 find_swap_name()
{
 FILE* stream;

 char p[15] = {'\0'};
 if( system("fdisk -l | grep \"swap\" | awk \'{print $1}\' > swap_name.txt") < 0) 
	{
		printf("can not generate the swap_name.txt\n");
		return;
	}
  stream = fopen("swap_name.txt", "r+");
 if(fgets(p, 15, stream) != NULL)
	{
	 int i=0;
	  for(;i<15;i++)
		{
		  if(p[i] == '\n')                    //get rid of the '\n' at the end of each line in mount_dir.txt
		  	{
				p[i] = '\0';
				break;
			}
		}
	}
 	strcpy(swap_name, p);
}


void help () 
{
    printf("Syntax: %s [-svh] file1 file2 etc.\n\n", parameter);
    printf("\t-s  standard name\n");
    printf("\t-v  is verbose mode.\n");
    printf("Default is fast mode (fast writes 0).\n");
    exit(1);
}

int decide_standard(char* standard_name)
{
	if(strcmp(standard_name, "FAST") == 0) standard = S_FAST;
	else if(strcmp(standard_name, "USDD") == 0) standard = S_USDD;
	else if(strcmp(standard_name, "RGP") == 0)  standard = S_RGP;
	else if(strcmp(standard_name, "BHI") == 0)  standard = S_BHI;
	else if(strcmp(standard_name, "CRTO") == 0) standard = S_CRTO;
	else if(strcmp(standard_name, "CGB") == 0)  standard = S_CGB;
	else if(strcmp(standard_name, "TWO") == 0)  standard = S_TWO;
	return standard;
}

int  decide_mount_dir(char* filename)
{
  char p[15] ={'\0'};
  int i = 0;
  FILE* stream;
  stream = fopen("./mount_dir.txt", "r+");
  while(fgets(p, 15, stream) != NULL)                 
  	{
	 
	 for(;i<15;i++)
		{
		  if(p[i] == '\n')                    //get rid of the '\n' at the end of each line in mount_dir.txt
		  	{
				p[i] = '\0';
				break;
			}
		}
	 //if( ((strlen(p) > 2)  &&  (strncmp(p, filename, strlen(p) - 1) == 0)) || strcmp(filename, "/") == 0 )	   return 1;
	 if(strcmp(p, filename) == 0)  return 1;
	}
   return 0;
}

int decide_disk_space_name(char* file_path)
{
 char* device_name = NULL;
 struct stat file_stat;
 lstat(file_path,  &file_stat);
 find_swap_name();
 if( S_ISBLK(file_stat.st_mode)) 
	{
	 if(*file_path == '/')                      //absolutelly path
		{
		 char* p  = strrchr(file_path, '/');
		 if(p != NULL) 
		 	{
			 p++;
			 device_name = p;
			}
		 if( (device_name != NULL) && ( strlen(device_name) == 4) && strcmp(file_path, swap_name)) 
			{
			 printf("%s is a disk space name.\n", file_path);
			 return 1;
			}
		 
		}
	 printf("The file you specified is not a disk space name, maybe a disk name or a swap space name. Please check it out.\n");
	 return 2;
	}
 else 
	{
	 printf("%s is not a disk space device.\n", file_path);
	 return 0;
	}	
}

progress(int indicator)
{
  printf("this is the progress display. %d \n", indicator);
}

int decide_special_file(char* filename)
{
  struct stat file_stat;
  lstat(filename,  &file_stat);
  if(S_ISCHR(file_stat.st_mode) || S_ISBLK(file_stat.st_mode) || S_ISFIFO(file_stat.st_mode) || S_ISLNK(file_stat.st_mode) || S_ISSOCK(file_stat.st_mode))
	return 1;
  if(S_ISREG(file_stat.st_mode)) is_regular_file = 1;
  if(S_ISDIR(file_stat.st_mode)) is_regular_dir = 1; 
  return 0;
}


int decide_file_exist(char* filename)
{
 if(access(filename, F_OK) < 0) 
	return 0;
 else 
	return 1;
}




int main(int argc, char* argv[])
{
 FILE *stream;
 int loop;
 int result;
 parameter = argv[0];
 generate_mount_dir();
 if (argc < 2 || strncmp(argv[1], "-h", 2) == 0|| strncmp(argv[1], "--h", 3) == 0)
    help();
 while(1)
  {
	result = getopt(argc, argv, "s:rvh");
	if(result < 0) break;
	switch(result)
	{
	case 'v': verbose++;
		  break;
	case 's': 
		if(strncmp(optarg, "-", 1) == 0)      //get the standard after the -s option
			{
			printf("The standard is missing after the option -s.\n");
			}
		else
			{
				decide_standard(optarg);
				printf("The standard is:%d\n", standard);
			}
		  break;
	default:  help();
	}
  }

   loop = optind;
   if(loop == argc) help();
   stream = fopen("./mount_dir.txt", "r+");
   while(loop < argc)
	{
	
	is_regular_file = 0;
	is_regular_dir = 0;
	char rm_file_dir[strlen(argv[loop]) + 1];   //get the file name which the user want to delete
	strcpy(rm_file_dir, argv[loop]);
	loop++;
	if(decide_file_exist(rm_file_dir) == 0) 
		{
		printf("file:%s not exist.\n", rm_file_dir);
		continue;
		}	
	if(decide_disk_space_name(rm_file_dir) == 1)
		{
//		  char command[30] = "umount -l ";
//		  strcat(command, rm_file_dir);	
//		  system(command);
		  find_block_size_count(rm_file_dir) ;
		  xaddev(rm_file_dir, standard, Block_Size, &progress, Total_Block);
		}

	}

fclose(stream);
}
