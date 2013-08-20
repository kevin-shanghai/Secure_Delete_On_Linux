
#include "xadlib.h"
#include "stdio.h"
char NAMEOFSTANDARD[S_TOTAL][32]={"FAST","USDD","RGP","BHI","CRTO","CGB","TWO"};
xadfile(char *filename, int standard)
{
printf("sample printf: erase the file %s  with the standard %s. \n", filename, NAMEOFSTANDARD[standard]);
}


xaddev(char *devname, int standard, int blocksize,int progress(int indicator), int totalblock)
{
printf("sample printf: erase the device  %s  with the standard %s. block size: %d, totalblock %d \n", devname, NAMEOFSTANDARD[standard], blocksize,totalblock);
progress(1);
}

