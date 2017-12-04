#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "cmd.h"
#include "public.h"

//显示帮助
void show_help()
{
    printf("\nusage: ./packer -P PRODUCT_MODEL -C cfg_filename -o pkg_filename \n");
    printf("other arguments: -v(-version), -h(-?, -help) \n");
}

//显示版本信息
void show_version()
{
    printf("\n---------------------------------------------------------------------\n");
    printf("pack tool v1.0 rev 1135, build 20140116\n");
    printf("---------------------------------------------------------------------\n");

}

int get_arguments(int argc,char *argv[],struct Args_t *args)
{
    int opt = 0;

    //需要读取那些命令行参数
    static const char *optString = "P:C:o:v:h:?:-help:-version";

    /* Initialize globalArgs before we get to work. */
    args->productName = NULL;
    args->outFileName = "arm-update.pkg";
    args->outFile = NULL;
    args->configFileName = NULL;

    //读命令行参数
    opt = getopt( argc, argv, optString );

    if (opt==-1)
    {
        show_version();
        show_help();
        return false;
    }

    while ( opt != -1 )
    {
        switch ( opt )
        {
            //遇到-o，就保存命令行输入
            case 'o':
                args->outFileName = optarg;
                break;

            //遇到-C，就保存命令行输入
            case 'C':
                args->configFileName = optarg;
                break;

            //遇到-P，就保存命令行输入
            case 'P':
                args->productName = optarg;
                break;

            //遇到-v或--version，就显示版本信息
            case 'v':
                show_version();
                return false;
                break;

            //遇到-h或-?或--help，就显示帮助信息
            case 'h':
            case '?':
                show_help();
                return false;
                break;
        }

        //读命令行参数
        opt = getopt( argc, argv, optString );
    }

    //如果没有输入配置文件名，则错误退出
    if (args->configFileName == NULL)
    {
        printf("no input config file\n");
        return false;
    }

    //如果没有输入产品信息，则错误退出
    if (args->productName == NULL)
    {
        printf("no input product name\n");
        return false;
    }

    return true;
}
