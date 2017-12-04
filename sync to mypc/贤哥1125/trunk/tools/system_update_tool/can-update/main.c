#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "updatefw.h"

void show_help()
{
    printf("\n------------------------------------------------------------------\n");
    printf("---------- note: ./can_updata CANNodeNumber HexFileName ----------\n");
    printf("------------------------------------------------------------------\n");
}

int check_argv(int argc, char *argv[])
{
    argv = argv;

    if (argc != 2)
    {
        printf("\nthis argument is too more or too less\n");
        printf("please check your input argument\n");
        printf("this app only accpet two argument\n");
        return -1;
    }

    return 0;
}

int get_argv_fileName(int argc, char *argv[], char *fileName)
{
    argc = argc;

    if (fileName == NULL)
    {
        printf("file Name pointer is NULL\n");
        return -1;
    }

    strcpy(fileName,argv[1]);

    return 0;
}

int main(int argc, char *argv[])
{
    int iNode;
    char fileName[256] = {0};
    char ver[256] = {0};
    char date[256] = {0};
    char node[256] = {0};
    
    if (check_argv(argc,argv))
    {
        return 0;
    }

    memset(fileName,0,sizeof(fileName));

    if (get_argv_fileName(argc,argv,fileName))
    {
        return 0;
    }

    scan_information(fileName,ver,date,node);
    
    printf("ver = %s\n",ver);
    printf("date = %s\n",date);
    printf("node = %s\n",node);
   
    iNode = atoi(node);

    printf("Node = %d\n",iNode);

    return CanFirmware_Update(iNode,fileName);
}
