#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "crc32.h"
#include "system.h"

//single send 
extern int sendSignal(char* process,int update_what,int progress);
extern int getUnpacktag(char* filename);

#define FPOS(type, field) ((uint32_t) & ((type*)0)->field)



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

int unpack(const char *product_tag, const char *pack_file_name, char* filepath,char *unpack_where,char** filename)
{
	int tag = 0;
	int filesize = 0;
	int writebyte = 0;
	long savewrite = 0;
	int writelen = 0;
    //包文件
    int pack_file;
    //暂时解压出来的文件
    FILE *tmp_file;
    //暂时解压出来的文件
    char tmp_file_name[128];
	char tmp_buff[128];
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

	memset(tmp_file_name,0,sizeof(tmp_file_name));
	sprintf(tmp_file_name,"%s/%s",filepath,pack_file_name);
	printf("tempfilename:%s\r\n",tmp_file_name);
    pack_file = open(tmp_file_name,O_RDONLY);
    if (pack_file == -1)
    {
        printf("can not open pack file\n");
		sendSignal(APP_NAME, FILENOTFIND, 0);
        return -1;
    }

    //读出文件本身大小
    file_size = get_file_size(tmp_file_name);
    //计算p_targets的偏移量
    p_targets_offset = FPOS(UPDATE_PKG,p_targets);

    //文件大小不对，直接退出
    if ( file_size < p_targets_offset )
    {
        printf("file error,the file is not pkg file\n");
        close(pack_file);
		sendSignal(APP_NAME, FILEISNOTPKG, 0);
        return -1;
    }

    //文件映射
    update_pkg = (UPDATE_PKG *)mmap(NULL,file_size,PROT_READ,MAP_SHARED,pack_file,0);
    if (update_pkg == MAP_FAILED)
    {
        printf("mmap fail\n");
        close(pack_file);
		sendSignal(APP_NAME, FILEISNOTPKG, 0);
        return -1;
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
		sendSignal(APP_NAME, FILEISNOTPKG, 0);
        return -1;
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
		sendSignal(APP_NAME, FILEISNOTPKG, 0);
        return -1;
    }

    //判断是不是要解压包的机器，如果不是，就退出
    if ( strcmp(product_tag,(char *)&update_pkg->product_tag[0]) != 0 )
    {
        printf("the product tag is unmatched.\n");
        munmap((void *)update_pkg,file_size);
        close(pack_file);
		sendSignal(APP_NAME, FILEPTAGNOTFIT, 0);
        return -1;
    }

    //计算包的crc
    crc = 0xffffffff;
	crc = my_crc32(crc, (const unsigned char *)&update_pkg->product_tag[0], size - sizeof(crc));

    //crc判断，判断计算出来的crc与包的crc不一样的话，就显示错误信息，并返回
    if (crc != update_pkg->CRC)
    {
        printf("crc error\n");
        munmap((void *)p_targets,sizeof(TARGET_COMPONENT) * update_pkg->target_components_num);
        munmap((void *)update_pkg,p_targets_offset);
        close(pack_file);
		sendSignal(APP_NAME, FILEISNOTPKG, 0);
        return -1;
    }

    getcwd(&old_dir[0],1024);
    chdir(unpack_where);
	
	sendSignal(APP_NAME, UNPACKFILE, 0);

	filesize = update_pkg->target_components_num;
    //解包
    for (i=0,offset=0;i<filesize;i++)
    {
        paddr = (char *) ( ( (char *) (update_pkg) ) + (data_offset + offset) );
        len = p_targets[i].data_size;
        memset(tmp_file_name,0,sizeof(tmp_file_name));
        strcpy(tmp_file_name,(char *)(&p_targets[i].tag[0]));
		tag = getUnpacktag(tmp_file_name);
		filename[i] = malloc(25);
		if(strcmp("UBOOT",tmp_file_name)==0){
			sprintf(filename[i] ,"AUBOOT");
		}else{
			sprintf(filename[i] ,"%s",tmp_file_name);
		}
		if(tag == UNPACK_FILEUNKNOW){
			sendSignal(APP_NAME, UNPACKFILENOTFIND, 0);
			return -1;
		}else{
			sendSignal(APP_NAME, tag, 0);
		}
	    memset(tmp_buff,0,sizeof(tmp_buff));
		sprintf(tmp_buff,"%s/%s",unpack_where,filename[i]);
		printf("filename:%s\n",tmp_buff);
        tmp_file = fopen(tmp_buff,"w+");
        if (tmp_file == NULL)
        {
            printf("can not create tmp file %s\n",tmp_buff);
            munmap((void *)update_pkg,file_size);
            close(pack_file);
            chdir(old_dir);
            return -1;
        }	
		writebyte = 0;
		writelen = 0;
		savewrite = 0;
		while(len > writebyte){
			writelen = (len-1024)>=writebyte?1024:len-writebyte;
			fwrite(&paddr[writebyte],writelen,1,tmp_file);
			writebyte +=writelen;
			if(savewrite!=writebyte*100/len){
				savewrite = writebyte*100/len;
				if(savewrite %10 ==0){
					sendSignal(APP_NAME,tag,savewrite);
				}
			}
			usleep(25);
		}
		sendSignal(APP_NAME,tag,100);
		if(len < 1024*1024)
			sleep(1);
		else
			usleep(1000);
   //   fwrite(paddr,len,1,tmp_file);
        fclose(tmp_file);
        offset += len;
    }

    //解包完成
    munmap((void *)update_pkg,file_size);
    close(pack_file);
    chdir(old_dir);
	return filesize;
}

