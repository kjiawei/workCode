#ifndef __CMD_H__
#define __CMD_H__

#include <stdio.h>

struct Args_t
{
    char * productName;
    const char *outFileName;    /* -o option */
    FILE *outFile;
    char *configFileName;          /**/
};

extern void show_version(void);
extern void show_help(void);
extern int get_arguments(int argc,char *argv[],struct Args_t * args);

#endif
