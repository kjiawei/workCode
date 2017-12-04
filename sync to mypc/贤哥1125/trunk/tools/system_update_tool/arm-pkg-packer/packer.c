#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "cmd.h"
#include "public.h"
#include "crc32.h"

//计算文件大小
uint32_t get_file_size(const char *filename)  
{  
    unsigned long size;  
    FILE* fp = fopen( filename, "rb" );  
    
    if(fp==NULL)  
    {  
        printf("ERROR: Open file %s failed.\n", filename);  
        return 0;  
    }

    fseek( fp, SEEK_SET, SEEK_END );  
    size=ftell(fp);  
    
    fclose(fp);  
    return size;  
}

int pkg(struct Args_t args)
{
    //配置文件，就是说明要打包那些文件的文件
    FILE *config_file = NULL;
    //输入文件，就是配置文件里面的一项的文件
    FILE *input_file = NULL;
    //用来读配置文件，一行数据的用。
    char linebuf[4096];
    //输入文件的名笱
    char input_file_name[1024];
    //用于分析和定位tag和input等关键的坐标
    char *tag_pos,*input_pos,*tag_value_pos,*input_value_pos;
    //打包的结构
    UPDATE_PKG pkg;
    UPDATE_PKG *update_pkg;
    //计算现在有多少个需要打包的组件
    uint32_t target_components_num;
    //用于暂时存放打包组件的数据data，一个链表
    struct TMP_DATA * data = NULL;
    struct TMP_DATA * first_data = NULL;
    struct TMP_DATA * tmp_data = NULL;
    //用于暂时存放打包组件的compoent
    TARGET_COMPONENT component[256];
    P_TARGET_COMPONENT p_component;
    //计数器
    uint32_t i;
    int CRC_size = 0;

    //打开配置文件
    config_file = fopen(args.configFileName,"r");
    if (config_file == NULL)
    {
        printf("can not open config file\n");
        return false;
    }

    //读到的配置项
    target_components_num = 0;  

    //循环来读取配置文件的每一行，然后暂时放在缓存里面
    while(fgets(linebuf,4096,config_file))
    {
        //查找tag项
        tag_pos = strstr(linebuf,"tag");
        //查找input项
        input_pos = strstr(linebuf,"input");
        
        //没有找到tag项，则错误输出，并返回
        if (tag_pos == NULL)
        {
            printf("can not find tag,must be have tag\n");
            fclose(config_file);
            return false;
        }
        
        //没有找到input项，则错误输出，并返回
        if (input_pos == NULL)
        {
            printf("can not find input,must be have input\n");
            fclose(config_file);
            return false;
        }
        
        //查找tag项和input项的＝号
        tag_value_pos = strstr(tag_pos,"=");
        input_value_pos = strstr(input_pos,"=");

        //没有查找到=号，则错误输出，并返回。有找到，就读=后一个位置
        if ( tag_value_pos != NULL )
        {
            tag_value_pos++;
        }
        else
        {
            printf("can not find = , must be have =\n");
            return false;
        }

        //没有查找到=号，则错误输出，并返回。有找到，就读=后一个位置
        if ( input_value_pos != NULL )
        {
            input_value_pos++;
        }
        else
        {
            printf("can not find = , must be have =\n");
            printf("\n");
            return false;
        }
       
        //读缓存comonent的位置
        p_component = (P_TARGET_COMPONENT) (&component[target_components_num]);
        
        memset(&p_component->reserved[0],0,sizeof(p_component->reserved));

        //input的名称读取，即input等号后面的字符串
        i = 0;
        memset(&input_file_name[0],0,sizeof(input_file_name));
        while ( ( *input_value_pos != ' ' ) &&  ( *input_value_pos != '\r' ) &&( *input_value_pos != '\n' )  ) 
        {
            *(input_file_name + i) = *input_value_pos;
            input_value_pos++;
            i++;
        }

        //tag的名称读取，即tag等号后面的字符串
        i = 0;
        memset(&p_component->tag[0],0,sizeof(p_component->tag));
        while ( ( *tag_value_pos != ' ' ) &&  ( *tag_value_pos != '\r' ) && ( *tag_value_pos != '\n' )  )
        {
            p_component->tag[i] = *tag_value_pos;
            tag_value_pos++;
            i++;
        }
        
        //如果组件文件，是一个不存在的文件，就错误通知，并退出
        input_file = fopen(input_file_name,"rb");
        if ( input_file == NULL )
        {
            printf("can not find input file,input_file = %s\n",input_file_name);
            return false;
        }
        
        //读取组件文件的大小
        p_component->data_size = get_file_size(input_file_name);
        
        //如果组件文件，小于等于0，则错误，并返回
        if (p_component->data_size <= 0)
        {
            printf("input file length is less than or equ 0\n");
            fclose(config_file);
            fclose(input_file);
            return false;
        }
        
        //分配缓冲组件项
        data = malloc(sizeof(struct TMP_DATA));
        if (data == NULL)
        {
            printf("can't malloc\n");
        }
        
        //根据组件大小，分配缓冲组件的文件大小
        data->data = malloc(p_component->data_size);
        if (data->data == NULL)
        {
            printf("can't malloc\n");
        }
        
        //把组件的内容，写到缓冲组件去。
        fread(data->data,p_component->data_size,1,input_file);
        fclose(input_file);

        //设置缓冲组件的大小
        data->size = p_component->data_size;
        
        //把缓冲组件，加去缓冲组件的链表里面去
        if ( target_components_num == 0 )
        {
            tmp_data = data;
            first_data = data;
            data->next = NULL;
        }
        else
        {
            tmp_data->next = data;
            tmp_data = tmp_data->next;
            data->next = NULL;
        }
        
        //读取到的组件数加一
        target_components_num++; 
    }

    fclose(config_file);

    //读取配置文件，看是不是只输入了0项。
    if ( target_components_num <= 0 )
    {
        printf("target_componets_num is less than or equ 0\n");
        return false;
    }
    
    //配置pkg，只配置一部分。
    memset(&pkg.product_tag[0],0,sizeof(pkg.product_tag));
    memcpy(&pkg.product_tag[0],args.productName,strlen(args.productName)); 
    pkg.target_components_num = target_components_num;
    

    //生成打包文件
    args.outFile = fopen(args.outFileName,"wb+");

    if (args.outFile == NULL)
    {
        printf("can not create output file\n");
        tmp_data = first_data;
        for(i=0;i<pkg.target_components_num;i++)
        {
            first_data = tmp_data->next;
            tmp_data->next = NULL;
            free(tmp_data->data);
            free(tmp_data);
            tmp_data = first_data;    
        }
        return false;
    }

    fseek(args.outFile, sizeof(uint32_t), SEEK_SET);
    fwrite(&pkg.product_tag[0],sizeof(pkg.product_tag),1,args.outFile);
    fwrite(&pkg.target_components_num,sizeof(pkg.target_components_num),1,args.outFile);
    fwrite(&component[0],sizeof(TARGET_COMPONENT) * pkg.target_components_num,1,args.outFile);
    
    tmp_data = first_data;

    for(i=0;i<pkg.target_components_num;i++)
    {
        fwrite(tmp_data->data,tmp_data->size,1,args.outFile);
        CRC_size += tmp_data->size;
        first_data = tmp_data->next;
        tmp_data->next = NULL;
        free(tmp_data->data);
        free(tmp_data);
        tmp_data = first_data;    
    }
    fclose(args.outFile);

    int outFileFd;
    outFileFd = open(args.outFileName, O_RDWR);
    unsigned long file_size;

    file_size = lseek(outFileFd, 0, SEEK_END );

    update_pkg = (UPDATE_PKG *)mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, outFileFd, 0);
    if (update_pkg == MAP_FAILED) {
        printf("mmap fail\n");
        fclose(args.outFile);
        return -1;
    }

    //计算crc
    pkg.CRC = 0xffffffff;

    CRC_size += sizeof(pkg.product_tag) + sizeof(pkg.target_components_num) +\
            sizeof(TARGET_COMPONENT) * pkg.target_components_num;

    pkg.CRC = my_crc32(pkg.CRC, (void*)&update_pkg->product_tag, CRC_size);

    munmap((void *)update_pkg,file_size);

    lseek(outFileFd, 0, SEEK_SET);
    write(outFileFd, &pkg.CRC,sizeof(pkg.CRC));

    close(outFileFd);
    return true;
}

int main(int argc,char *argv[])
{

    //命令行输入参数的结果
    struct Args_t Args;

    //读命令行参数
    if (!get_arguments(argc,argv,&Args))
        return -1;

    //打包
    if(!pkg(Args))
        return -1;

    return 0;
}
