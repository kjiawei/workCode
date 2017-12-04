#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scan_information.h"

int scan_information(char *path, char *ver, char *date, char *node)
{
	FILE *fp = NULL;
	
	char buf[128];
    unsigned int  iTmp[128];
	unsigned char stringTmp[128];
	char *p;

    //版本信息的头
	unsigned char verString[] = {"jh1:v"};
	//日期信息的头
    unsigned char dateString[] = {"jh1:d"};
	//节点信息的头
    unsigned char nodeString[] = {"jh1:n"};
	
    //记录的长度
	unsigned int nByte;
	//记录的地址
    unsigned int nAddr;
	//记录的类型
    unsigned int nType;

	int i;

    //如果没有传输参数,或者参数错误
	if ( (path == NULL) || (ver == NULL) || (date == NULL) || (node == NULL) )
	{
		return -1;
	}

    //打开文件，分析文本
	fp = fopen(path,"rb");

    //如果打开失败，则返回
	if (fp == NULL)
	{
		return -1;
	}

    //如果文件没有结束，就一直分析。
	while(!feof(fp))
	{
        //读一行文本
		fgets(buf,128,fp); 
		
        //读取，长度，地址，类型
		if (sscanf(&buf[1],"%02x%4x%2x",&nByte,&nAddr,&nType) != 3 )
		{
			return -1;
		}

		//如果地址是版本记录的地址
		if (nAddr == 0xDFC0)
		{
			memset(stringTmp,0,sizeof(stringTmp));
			sscanf((&buf[1])+8, "%2x%2x%2x%2x%2x",&iTmp[0],&iTmp[1],&iTmp[2],&iTmp[3],&iTmp[4]);

            stringTmp[0]= iTmp[0];
            stringTmp[1]= iTmp[1];
            stringTmp[2]= iTmp[2];
            stringTmp[3]= iTmp[3];
            stringTmp[4]= iTmp[4];
            
            //因为地址不具有唯一性，所以必需分析数据的头部，看一看是不是版本信息的头部
            //如果是版本信息的头部，则copy数据。。
			if ( strcmp( (const char *)stringTmp, (const char *)verString) == 0 )
			{
                //buf[1]是因为一行数据的开头，是一个冒号，要跳过。
                //8，是要跳过长度，地址，类型 
                //5＊2，是要跳过jh1:v。。。
                //后面才是版本信息
				p = ( (&buf[1]) + 8 + (5*2));
				i = 0;
				//减5是因为jh1:v 的长度是5
                nByte -= 5;
				
				while(nByte)
				{
					sscanf(p+(2*i),"%2x",&iTmp[0]);
                    *(ver+i) = iTmp[0];
					i++;
					nByte--;
				}
			}
		}
	
		
		//如果地址是日期记录的地址
		if (nAddr == 0xDFD0)
		{
			memset(stringTmp,0,sizeof(stringTmp));
			sscanf((&buf[1])+8, "%2x%2x%2x%2x%2x",&iTmp[0],&iTmp[1],&iTmp[2],&iTmp[3],&iTmp[4]);

            stringTmp[0]= iTmp[0];
            stringTmp[1]= iTmp[1];
            stringTmp[2]= iTmp[2];
            stringTmp[3]= iTmp[3];
            stringTmp[4]= iTmp[4];
            
            //因为地址不具有唯一性，所以必需分析数据的头部，看一看是不是日期信息的头部
            //如果是日期信息的头部，则copy数据。。
			if ( strcmp( (const char *)stringTmp, (const char *) dateString) == 0 )
			{
                
                //buf[1]是因为一行数据的开头，是一个冒号，要跳过。
                //8，是要跳过长度，地址，类型 
                //5＊2，是要跳过jh1:v。。。
                //后面才是日期信息
				p = ( (&buf[1]) + 8 + (5*2));
				i = 0;
				nByte -= 5;
				
				while(nByte)
				{
					sscanf(p+(2*i),"%2x",&iTmp[0]);
                    *(date + i) = iTmp[0];
					i++;
					nByte--;
				}
			}
		}


		//如果地址是节点记录的地址
		if (nAddr == 0xDFE0)
		{
			memset(stringTmp,0,sizeof(stringTmp));

			sscanf((&buf[1])+8, "%2x%2x%2x%2x%2x",&iTmp[0],&iTmp[1],&iTmp[2],&iTmp[3],&iTmp[4]);

            stringTmp[0]= iTmp[0];
            stringTmp[1]= iTmp[1];
            stringTmp[2]= iTmp[2];
            stringTmp[3]= iTmp[3];
            stringTmp[4]= iTmp[4];
            
            //因为地址不具有唯一性，所以必需分析数据的头部，看一看是不是节点信息的头部
            //如果是节点信息的头部，则copy数据。。
			if ( strcmp((const char *)stringTmp,(const char *)nodeString) == 0 )
			{
                //buf[1]是因为一行数据的开头，是一个冒号，要跳过。
                //8，是要跳过长度，地址，类型 
                //5＊2，是要跳过jh1:v。。。
                //后面才是节点信息
				p = ( (&buf[1]) + 8 + (5*2));
								i = 0;
				nByte -= 5;
				
				while(nByte)
				{
					sscanf(p+(2*i),"%2x",&iTmp[0]);
                    *(node + i) = iTmp[0];
					i++;
					nByte--;
				}
			}
		}

	}

	fclose(fp);
	return 0;
}
