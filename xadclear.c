#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "xadlib.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "xadclear.h"
#define Standard_Erase 1
#define Deep_Erase 2
#define BUFFER_SIZE 65535
int verbose = 0;
char* parameter;
int level = 1;
//char* newname;
char newname[50];
int decide_file_exist(char* filename)
{
 if(access(filename, F_OK) < 0) 
	return 0;
 else 
	return 1;
}

void clean(int signal)
{
 int file_exist;
 printf("Terminated by sinal, Clean process will exit.\n");
 if(access(newname, F_OK) < 0) 
	file_exist = 0;
 else 
	file_exist = 1;
 sync();
 if((unlink(newname) != 0) && file_exist) 
	{
	 printf("Can not remove the temporary file:%s", newname);
	}
 int result = unlink(newname);
 if(result == 0)
	printf("The temporary file:%s was delete.\n", newname);
 exit(1);
}


char*  create_temp_file(char* name)
{
 //char newname[50];
 //char *p = NULL;
 struct stat file_stat;
 strcpy(newname, name);
 if(opendir(name) == NULL) 
	{
	 printf("can not open: %s\n", name);
	 return;
	}
 if(newname[strlen(newname) - 1] != '/') strcat(newname, "/");
 strcat(newname, "XXXXXXXX.XXXX");
 int result = lstat(newname, &file_stat);
 if(result < 0) 
 	return newname;
 else
	printf("The file:%s is alredy exist.\n", newname);
}


void xad_fill_buf(char pattern[3], unsigned long bufsize, char *buf) 
{
    int loop;
    //int where;
    
    for (loop = 0; loop < (bufsize / 3); loop++)
   {
        //where = loop * 3;
	*buf++ = pattern[0];
	*buf++ = pattern[1];
	*buf++ = pattern[2];
    }
}


int xad_overwrite(char* filename, unsigned long buffer_size)
{
 int fd;
 FILE* f;
 char buf[BUFFER_SIZE];
 int count = 0;
 int total_bytes = 0;
 if(create_temp_file(filename) == NULL) 
	{
	 printf("Failed.\n");
	 return -1;
	}
 printf("The temp file is:%s\n", newname);
 signal(SIGHUP, clean);
 signal(SIGTERM, clean);
 signal(SIGINT, clean);
 if( (fd = open(newname,  O_RDWR | O_EXCL | O_CREAT, 0600) ) < 0)
	{
	 printf("Create temporary file failed.\n");
	 return -1;
	}
 if((f = fdopen(fd, "r+") ) == NULL)
	{
	
	 printf("ERROR.\n");
	 close(fd);
 	 return -1;
	}

 else
	{
	 printf("start to erase:\n");
	 while(fwrite(&buf, 1, buffer_size, f) == buffer_size)
	 	{
		 count++;
		 xad_fill_buf(std_array_ff, buffer_size, buf);
		 if(count == 1000)
	 	 	{
			 
			 total_bytes += 65; 
			 printf("***%dM bytes was writen in the temporary file %s.***\n",total_bytes, newname);
			 count = 0;
			}
		}
	 printf("\nCongratulations, It's done.\n");
	}
 fclose(f);
 return 0;
}



void help () 
{
    printf("Syntax: %s [-svh] file1 file2 etc.\n\n", parameter);
    printf("\t-l(1|2)  standard name\n");
    printf("\t-v  is verbose mode.\n");
    printf("Default is fast mode (fast writes 0).\n");
    exit(1);
}

int decide_level(char* standard_name)
{
	if(strcmp(standard_name, "2") == 0) level = Deep_Erase;
	else level = Standard_Erase;
	return level;
}

void standard_erase(char* pathname)
{
 //lchdir(pathname)
}

int main(int argc, char* argv[])
{
 FILE *stream;
 int loop;
 int result;
 parameter = argv[0];
 if(getuid() != 0)  printf("Warning: you are not root. You might not be able to wipe the whole filesystem.\n");

 
 if (argc < 2 || strncmp(argv[1], "-h", 2) == 0|| strncmp(argv[1], "--h", 3) == 0)
    help();
 while(1)
  {
	result = getopt(argc, argv, "l:vh");
	if(result < 0) break;
	switch(result)
	{
	case 'v': verbose++;
		  break;
	case 'l': 
		if(strncmp(optarg, "-", 1) == 0)      //get the standard after the -s option
			{
			printf("The standard is missing after the option -l.\n");
			}
		else
			{
				decide_level(optarg);
				printf("The standard is:%d\n", level);
			}
		  break;
	default:  help();
	}
  }

   loop = optind;
   if(loop == argc) help();
   while(loop < argc)
	{

	char rm_file_dir[strlen(argv[loop]) + 1];   //get the file name which the user want to delete
	strcpy(rm_file_dir, argv[loop]);
	loop++;
	if(decide_file_exist(rm_file_dir) == 0) 
		{
		 printf("file:%s not exist.\n", rm_file_dir);
		 continue;
		}
	
	if(level == 2)
		{
		 printf("level:Deep_Erase.\n");
		}	
	else
		{
		 printf("level:Standard_Erase.\n");
 		 int result = xad_overwrite(rm_file_dir, BUFFER_SIZE);
		printf("result is:%d\n", result);
		 if(!result)
			{
			 unlink(newname);
			 printf("Done!");
			}
		}
	}

return 0;
}
