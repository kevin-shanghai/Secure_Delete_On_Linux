#ifndef XADLIB_H
#define XADLIB_H

#define S_TOTAL 7
#define S_FAST 0
#define S_USDD 1
#define S_RGP  2
#define S_BHI  3
#define S_CRTO 4
#define S_CGB  5
#define S_TWO  6

extern char NAMEOFSTANDARD[S_TOTAL][32];
extern xadfile(char *filename, int standard);
extern xaddev(char *devname, int standard, int blocksize, int progress(int indicator), int totalblock);
#endif

