#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "public.h"
#include "crc32.h"

#define FPOS(type, field) ((uint32_t) & ((type*)0)->field)

//显示帮助
void show_help()
{
    printf("\nusage: ./unpacker PRODUCT_TAG filename [where_unpack_to]\n");
    //printf("other arguments: -v(-version), -h(-?, -help) \n");
}

//显示版本信息
void show_version()
{
    printf("\n---------------------------------------------------------------------\n");
    printf("unpacker tool v0.9 rev 183, build 201207181135\n");
    printf("---------------------------------------------------------------------\n");
}

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

int unpack(const char *product_tag, const char *pack_file_name, char *unpack_where)
{
    //包文件
    int pack_file;
    //暂时解压出来的文件
    FILE *tmp_file;
    //暂时解压出来的文件
    char tmp_file_name[16];
    //计算一下包的data在什么位置
    uint32_t data_offset = 0;
    //计算一下p_targets在什么位置
    uint32_t p_targets_offset = 0;
    //相对data而言，各个组件的偏移位置
    uint32_t offset = 0;
    //计算包的crc
    uint32_t crc;
    //计数器
    uint32_t i;
    //包结构
    UPDATE_PKG * update_pkg = NULL;
    //计算要读取的长度
    uint32_t len;
    //
    char *paddr;
    //
    P_TARGET_COMPONENT p_targets;
    //整个解压包的大小
    uint32_t file_size;
    //目录地址
    char old_dir[1024];
    //计算出来的文件大小
    uint32_t size;

    pack_file = open(pack_file_name,O_RDONLY);
    if (pack_file == -1)
    {
        printf("can not open pack file\n");
        return ERR_RW;
    }

    //读出文件本身大小
    file_size = get_file_size(pack_file_name);
    //计算p_targets的偏移量
    p_targets_offset = FPOS(UPDATE_PKG,p_targets);

    //文件大小不对，直接退出
    if ( file_size < p_targets_offset )
    {
        printf("file error,the file is not pkg file\n");
        close(pack_file);
        return ERR_UNKNOWN;
    }

    //文件映射
    update_pkg = (UPDATE_PKG *)mmap(NULL,file_size,PROT_READ,MAP_SHARED,pack_file,0);
    if (update_pkg == MAP_FAILED)
    {
        printf("mmap fail\n");
        close(pack_file);
        return ERR_RW;
    }

    //计算data的偏移量
    data_offset = sizeof(update_pkg->CRC) +
                  sizeof(update_pkg->product_tag) +
                  sizeof(update_pkg->target_components_num) +
                  ( sizeof(TARGET_COMPONENT) * update_pkg->target_components_num );

    //文件大小不对，直接退出
    if ( file_size < data_offset )
    {
        printf("file error,the file is not pkg file\n");
        munmap((void *)update_pkg,file_size);
        close(pack_file);
        return ERR_UNKNOWN;
    }

    p_targets = (P_TARGET_COMPONENT) ( ( (char *) (update_pkg) ) + (p_targets_offset) );

    //计算出来的文件大小，不等于文件本身大小的话，就退出
    size = 0;
    size = data_offset;
    for (i=0;i<update_pkg->target_components_num;i++)
    {
        size += p_targets[i].data_size;
    }

    if (size != file_size)
    {
        printf("file error,the file is not pkg file\n");
        munmap((void *)update_pkg,file_size);
        close(pack_file);
        printf("\n");
        return ERR_UNKNOWN;
    }

    //判断是不是要解压包的机器，如果不是，就退出
    if ( strcmp(product_tag,(char *)&update_pkg->product_tag[0]) != 0 )
    {
        printf("the product tag is unmatched.\n");
        munmap((void *)update_pkg,file_size);
        close(pack_file);
        return ERR_PRODUCT_TAG;
    }

    //计算包的crc
    crc = 0xffffffff;

    crc = my_crc32(crc,
                    (const unsigned char *)&update_pkg->product_tag[0],
                    sizeof(update_pkg->product_tag));

    crc = my_crc32(crc,
                    (const unsigned char *)&update_pkg->target_components_num,
                    sizeof(update_pkg->target_components_num));

    crc = my_crc32(crc,
                    (const unsigned char *)p_targets,
                    sizeof(TARGET_COMPONENT)*update_pkg->target_components_num);

    for (i=0,offset=0;i<update_pkg->target_components_num;i++)
    {
        paddr = (char *) ( ( (char *) (update_pkg) ) + (data_offset + offset) );
        len = p_targets[i].data_size;
        offset += len;
        crc = my_crc32(crc,(const unsigned char *)paddr,len);
    }


    //crc判断，判断计算出来的crc与包的crc不一样的话，就显示错误信息，并返回
    if (crc != update_pkg->CRC)
    {
        printf("crc error\n");
        munmap((void *)p_targets,sizeof(TARGET_COMPONENT) * update_pkg->target_components_num);
        munmap((void *)update_pkg,p_targets_offset);
        close(pack_file);
        return ERR_CRC_CHECK;
    }

    getcwd(&old_dir[0],1024);
    chdir(unpack_where);

    //解包
    for (i=0,offset=0;i<update_pkg->target_components_num;i++)
    {
        paddr = (char *) ( ( (char *) (update_pkg) ) + (data_offset + offset) );
        len = p_targets[i].data_size;
        memset(tmp_file_name,0,sizeof(tmp_file_name));
        strcpy(tmp_file_name,(char *)(&p_targets[i].tag[0]));

        tmp_file = fopen(tmp_file_name,"w+");
        if (tmp_file == NULL)
        {
            printf("can not create tmp file %s\n",tmp_file_name);
            munmap((void *)update_pkg,file_size);
            close(pack_file);
            chdir(old_dir);
            return ERR_UNKNOWN;
        }

        fwrite(paddr,len,1,tmp_file);
        fclose(tmp_file);
        offset += len;
    }

    //解包完成
    munmap((void *)update_pkg,file_size);
    close(pack_file);
    chdir(old_dir);
    printf("unpack finish\n");
    return true;
}

int get_unpack_where(int argc,char *argv[],char *where)
{
    if ( (argc!=3) && (argc!=4) )
    {
        //printf("arg error\n");
        show_version();
        show_help();

        return false;
    }

    if ( argc == 4 )
    {
        strcpy(where,argv[3]);
    }
    else
    {
        strcpy(where,".");
    }

    return true;
}

int get_file_name(int argc,char *argv[],char * file_name)
{
    if ( (argc!=3) && (argc!=4) )
    {
        show_version();
        show_help();
        return false;
    }

    strcpy(file_name,argv[2]);

    return true;
}

int get_product_tag(int argc,char *argv[],char *product_tag)
{
   if ( (argc!=3) && (argc!=4) )
    {
        show_version();
        show_help();
        return false;
    }

    strcpy(product_tag,argv[1]);

    return true;
}

int main(int argc,char *argv[])
{
    char pack_file_name[1024] = {0};
    char where_file[1024] = {0};
    char product_tag[1024] = {0};

    if (!get_product_tag(argc,argv,product_tag))
        return -1;

    //读要解包的文件名
    if (!get_file_name(argc,argv,pack_file_name))
        return -1;

    if (!get_unpack_where(argc,argv,where_file))
        return -1;

    //解包
    if (!unpack(product_tag,pack_file_name,where_file))
    {
        return -1;
    }

    return 0;
}
