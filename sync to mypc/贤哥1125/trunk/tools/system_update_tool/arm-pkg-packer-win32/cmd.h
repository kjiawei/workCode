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

#endif
