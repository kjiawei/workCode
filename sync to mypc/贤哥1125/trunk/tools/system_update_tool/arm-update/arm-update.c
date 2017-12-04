#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>
#include <unistd.h>
#include "system.h"
#include "updatefw.h"

extern int sendSignal(char* process,int update_what,int progress);
extern int unpack(const char *product_tag, const char *pack_file_name, char* filepath,char *unpack_where,char** filename);

int operator_type = 0;

//get system result
int getResultFromSystemCall(const char* pCmd, char* pResult, int size)
{
	int fd[2];
	if(pipe(fd))   {
	  printf("pipe error!\n");
	  return -1;
	}
	//prevent content in stdout affect result
	fflush(stdout);
	//hide stdout
	int bak_fd = dup(STDOUT_FILENO);
	int new_fd = dup2(fd[1], STDOUT_FILENO);
	//the output of `pCmd` is write into fd[1]
	system(pCmd);
	read(fd[0], pResult, size-1);
	usleep(2000);
	pResult[strlen(pResult)-1] = 0;
	//resume stdout
	dup2(bak_fd, new_fd);
	return 0;
}

//get rootfs para1 and para2
int set_rootfs_argv(int rootfsSize,unsigned char* buf)
{
	int kernelSize = 0x300000;

	char * kernelOffset = "0x001a0000";
	char * rootfsOffset = "0x004a0000";

	char * kernelSdramAddr = "0x72000000";
	char * rootfsSdramAddr = "0x71000000";

	unsigned char *ptr;

	FILE * fp = NULL;

	char ethaddr[] = "ethaddr=08:00:3e:26:0a:55";
	char bootdelay[] =  "bootdelay=3";
	char baudrate[] = "baudrate=115200";
	char ipaddr[] = "ipaddr=192.168.1.219";
	char serverip[] = "serverip=192.168.1.218";
	char netmask[] = "netmask=255.255.255.0";
	char splashimage[] = "splashimage=0x71000000";
	char bootlogo[] = "bootlogo=1";
	char lcdtype[] = "lcdtype=1024x768";
	char ethact[] = "ethact=macb0";
	char _stdin[] = "stdin=serial";
	char _stdout[] = "stdout=serial";
	char _stderr[] = "stderr=serial";
	char bootargs[512];
	char bootcmd[512];

	int crc = 0x55aa55aa;

	sprintf(bootargs,"bootargs=console=ttySAC0,115200 tft=L mem=exactmap mem=128M@0x70000000 mem=128M@0x20000000 initrd=%s,0x%08X root=/dev/ram0 rw",rootfsSdramAddr,rootfsSize);

	sprintf(bootcmd,"bootcmd= nand read.jffs2 %s %s 0x%x; nand read.jffs2 %s %s 0x%08X; bootm %s;",
	kernelSdramAddr,kernelOffset,kernelSize,rootfsSdramAddr,rootfsOffset,rootfsSize,kernelSdramAddr);

	memset(buf,0,sizeof(buf));

	ptr = buf + ENV_HEADER_SIZE;

	memcpy ( ptr, ethaddr, strlen(ethaddr) + 1 );
	ptr = ptr + strlen(ethaddr) + 1;

	memcpy ( ptr, bootdelay, strlen(bootdelay) + 1 );
	ptr = ptr + strlen(bootdelay) + 1;

	memcpy ( ptr, baudrate, strlen(baudrate) + 1 );
	ptr = ptr + strlen(baudrate) + 1;

	memcpy ( ptr, ipaddr, strlen(ipaddr) + 1 );
	ptr = ptr + strlen(ipaddr) + 1;

	memcpy ( ptr, serverip, strlen(serverip) + 1 );
	ptr = ptr + strlen(serverip) + 1;

	memcpy ( ptr, netmask, strlen(netmask) + 1 );
	ptr = ptr + strlen(netmask) + 1;

	memcpy ( ptr,splashimage, strlen(splashimage) + 1 );
	ptr = ptr + strlen(splashimage) + 1;

	memcpy ( ptr,bootlogo, strlen(bootlogo) + 1 );
	ptr = ptr + strlen(bootlogo) + 1;

	memcpy ( ptr,lcdtype, strlen(lcdtype) + 1 );
	ptr = ptr + strlen(lcdtype) + 1;

	memcpy ( ptr,ethact, strlen(ethact) + 1 );
	ptr = ptr + strlen(ethact) + 1;

	memcpy ( ptr,_stdin, strlen(_stdin) + 1 );
	ptr = ptr + strlen(_stdin) + 1;

	memcpy ( ptr,_stdout, strlen(_stdout) + 1 );
	ptr = ptr + strlen(_stdout) + 1;

	memcpy ( ptr,_stderr, strlen(_stderr) + 1 );
	ptr = ptr + strlen(_stderr) + 1;

	memcpy ( ptr,bootargs, strlen(bootargs) + 1 );
	ptr = ptr + strlen(bootargs) + 1;

	memcpy ( ptr,bootcmd, strlen(bootcmd) + 1 );
	ptr = ptr + strlen(bootcmd) + 1;

	memcpy(buf, &crc, sizeof(int));
	
	return 0;
}

//get update what
int getUpdateWhat(char* filename){
	int what = 0;
	if(strcmp(filename,FILENAME_BOOTSTRAP)==0 || strcmp(filename,FILENAME_BOOTSTRAP_LOW)==0){
		what = UPDATE_BOOTSTRAP;
	}else if(strcmp(filename,FILENAME_UBOOT)==0 || strcmp(filename,FILENAME_UBOOT_LOW)==0 || strcmp(filename,FILENAME_AUBOOT)==0){
		what = UPDATE_UBOOT;
	}else if(strcmp(filename,FILENAME_LOGO)==0 || strcmp(filename,FILENAME_LOGO_LOW)==0 ){
		what = UPDATE_LOGO;
	}else if(strcmp(filename,FILENAME_KERNEL)==0 || strcmp(filename,FILENAME_KERNEL_LOW)==0){
		what = UPDATE_KERNEL;
	}else if(strcmp(filename,FILENAME_ROOTFS)==0 || strcmp(filename,FILENAME_ROOTFS_LOW)==0 ){
		what = UPDATE_ROOTFS;
	}else if(strcmp(filename,FILENAME_APPFS)==0 || strcmp(filename,FILENAME_APPFS_LOW)==0){
		what = UPDATE_APPFS;
	}
	return what;
}

//get dev name
int getUpdateDev(char* filename,char* dev){
	if(strcmp(filename,FILENAME_BOOTSTRAP)==0 || strcmp(filename,FILENAME_BOOTSTRAP_LOW)==0){
		strcpy(dev,MTD_BOOTSTRAP);
	}else if(strcmp(filename,FILENAME_UBOOT)==0 || strcmp(filename,FILENAME_UBOOT_LOW)==0 || strcmp(filename,FILENAME_AUBOOT)==0){
		strcpy(dev,MTD_UBOOT);
	}else if(strcmp(filename,FILENAME_LOGO)==0 || strcmp(filename,FILENAME_LOGO_LOW)==0){
		strcpy(dev,MTD_LOGO);
	}else if(strcmp(filename,FILENAME_KERNEL)==0 || strcmp(filename,FILENAME_KERNEL_LOW)==0){
		strcpy(dev,MTD_KERNEL);
	}else if(strcmp(filename,FILENAME_ROOTFS)==0 || strcmp(filename,FILENAME_ROOTFS_LOW)==0){
		strcpy(dev,MTD_ROOTFS);
	}else if(strcmp(filename,FILENAME_APPFS)==0 || strcmp(filename,FILENAME_APPFS_LOW)==0){
		strcpy(dev,MTD_APPFS);
	}
	return 0;
}

//get dev name
int getUnpacktag(char* filename){
	if(strcmp(filename,FILENAME_BOOTSTRAP)==0 || strcmp(filename,FILENAME_BOOTSTRAP_LOW)==0){
		return UNPACK_BOOTSTRAP;
	}else if(strcmp(filename,FILENAME_UBOOT)==0 || strcmp(filename,FILENAME_UBOOT_LOW)==0 || strcmp(filename,FILENAME_AUBOOT)==0){
		return UNPACK_UBOOT;
	}else if(strcmp(filename,FILENAME_LOGO)==0 || strcmp(filename,FILENAME_LOGO_LOW)==0 ){
		return UNPACK_LOGO;
	}else if(strcmp(filename,FILENAME_KERNEL)==0 || strcmp(filename,FILENAME_KERNEL_LOW)==0){
		return UNPACK_KERNEL;
	}else if(strcmp(filename,FILENAME_ROOTFS)==0 || strcmp(filename,FILENAME_ROOTFS_LOW)==0){
		return UNPACK_ROOTFS;
	}else if(strcmp(filename,FILENAME_APPFS)==0 || strcmp(filename,FILENAME_APPFS_LOW)==0){
		return UNPACK_APPFS;
	}
	return UNPACK_FILEUNKNOW;
}


//update dev
int updateDev(char* filepath,char* filename,char* devname,int bytes,int updatewhat,char* buf){
	int filefd = 0;
	int devfd = 0;
	char buff[1024];							
	int writebyte;
	long savewrite = 0;
	int len = 0;
	char* buf1 = NULL;
	int addr = 0;
	int retOOB = 0; 
	int nand_addr = 0;
	int hadBadBlock = 0;
	int save_nand_addr = 0;
	char badBlackTable[2048];
	struct erase_info_user erase;
	struct mtd_info_user mtd;
	
	devfd=open(devname,O_SYNC|O_RDWR);
	if(devfd <0){
		printf("%s is not exist",devname);
		return -1;
	}
	if(ioctl(devfd, MEMGETINFO, &mtd)<0)return -1;
	erase.start=0;
	erase.length = mtd.size;

	nand_addr = 0;
	for (len = 0; len < erase.length/128/1024; len++) {		
		addr = len * 128 *1024;
		retOOB = ioctl(devfd, MEMGETBADBLOCK , &addr);
		if(retOOB == 1) {
			printf("have bad block :addr:%d\r\n", len);
			badBlackTable[len] = 1;
			hadBadBlock = 1;
		}
	}
	if(hadBadBlock == 1) {
		memset(buff,0,sizeof(buff));
		sprintf(buff,"flash_eraseall %s", devname);	
		system(buff);
	} else	{
		if(ioctl(devfd, MEMERASE, &erase) < 0) {
			return -1;
		}
	}
	printf("erase %s length %x\n",devname,mtd.size);
	
	if(filename !=NULL ){
		memset(buff,0,sizeof(buff));
		sprintf(buff,"%s/%s",filepath,filename);
		filefd = open(buff,O_RDWR,0);
		if(filefd <0){
			printf("file :%s is not exist\r\n",buff);
			close(devfd);
			return -1;
		}
	}
	writebyte = 0;
	addr = 0;
	len = 0;
	nand_addr = 0;
	save_nand_addr = -1;
	sendSignal(APP_NAME,updatewhat,savewrite);
	while(bytes > writebyte){
		if (hadBadBlock == 1 && save_nand_addr != nand_addr ) {
			//printf("check flash oob addr :%d\r\n", nand_addr);
			save_nand_addr = nand_addr;
			if(badBlackTable[nand_addr] == 1) {
				lseek(devfd, 0x20000, 1);
				printf("skipping bad block %d \r\n", nand_addr);
				nand_addr ++;
				continue;
			}
		}
		if(filename !=NULL && updatewhat!=UPDATE_BOOTSTRAP ){
			memset(buff,0,sizeof(buff));	
			read(filefd,buff,1024);
			len = write(devfd, buff, 1024);
			if(len <=0){
				if(filename != NULL)
					close(filefd);
				close(devfd);
				return -1;
			}
			writebyte +=len;
			if(savewrite!=writebyte*100/bytes){
				savewrite = writebyte*100/bytes;			
				//printf("%d\%\r\n",savewrite);
				if(savewrite%10==0)
					sendSignal(APP_NAME,updatewhat,savewrite);
			}
			usleep(20);
		}else if(buf!=NULL){
			write(devfd,buf,bytes);
			writebyte = bytes;
			sendSignal(APP_NAME,updatewhat,100);
		}else if(updatewhat == UPDATE_BOOTSTRAP){
			printf("UPDATE_BOOTSTRAP:bytes:%d\r\n",bytes);
			buf1 = (unsigned char*)malloc(bytes);
			memset(buf1,0,sizeof(buf1));
			read(filefd,buf1,bytes);
			buf1[0x16]=0;					//这里必须设置为0，否则会启动不成功
			write(devfd,buf1,bytes);
			sendSignal(APP_NAME,updatewhat,100);	
			writebyte = bytes;	
			free(buf1);
		}
		if(hadBadBlock ==1 && writebyte %( 1024 * 128) == 0) 
			nand_addr ++;
	}
	sendSignal(APP_NAME,updatewhat,100);
	sleep(1);
	if(filename!=NULL)
		close(filefd);
	close(devfd);
	return 0;
}

//update mtd device
int updateARMMTD(char* filepath,char* filename)
{
	struct stat st;
	int blocks = 0;
	long bytes = 0;	
	int updateWhat = 0;
	int updateProgress = 0;
	char devName[30]; 	
	char buff[1024];
	char buf[128];
	char* p = NULL;
	pid_t pid[MAX_PID_NUM]; 
	pid_t pid_newProgress;
	int cmd = -1;
	int syspid = 0;
	int isOwn = 0;
	unsigned char rootArgsbuf[CFG_ENV_SIZE] = {0};

	struct erase_info_user erase;
	struct mtd_info_user mtd;
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%s/%s",filepath,filename);
	stat(buf,&st);			
	blocks = st.st_size>>11;
	if(blocks<=0){
		printf("%s is not exist",filename);
		return -1;
	}
	bytes = (blocks+1)<<11;				//get file length
	updateWhat = getUpdateWhat(filename);
	//printf("filesize:%d,filewhat:%d", bytes, updateWhat);
	memset(devName,0,sizeof(devName));
	getUpdateDev(filename,devName);
	if(updateWhat != UPDATE_APPFS){
		cmd = updateDev(filepath,filename,devName,bytes,updateWhat,NULL);
		if(cmd == -1){
			printf("updateerror\n");
			return -1;
		}
		if(updateWhat == UPDATE_ROOTFS){
			memset(rootArgsbuf,0,sizeof(rootArgsbuf));
			set_rootfs_argv(bytes,rootArgsbuf);
			cmd = updateDev(filepath,NULL,MTD_PARA1,CFG_ENV_SIZE,UPDATE_PARA1,rootArgsbuf);	
			if(cmd == -1){
				printf("updateerror\n");
				return -1;
			}
			cmd = updateDev(filepath,NULL,MTD_PARA2,CFG_ENV_SIZE,UPDATE_PARA2,rootArgsbuf);	
			if(cmd == -1){
				printf("updateerror\n");
				return -1;
			}
		}
	}else if(updateWhat == UPDATE_APPFS){
		sendSignal(APP_NAME,UPDATE_APPFS,0);	
		//添加复制触摸屏的pointercal的操作
		memset(buff,0,sizeof(buff));
		sprintf(buff,"cp /usr/local/etc/pointercal %s ",filepath);
		system(buff);
	//	sleep(1);
		system("fuser -km /usr/local/data-disk/");
		usleep(500);
		system("umount -f /usr/local/data-disk/");
 	    printf("umount /usr/local/data-disk success\n"); 
		system("fuser -km /usr/local");
		usleep(500);
		printf("umount\n");
		system("umount -f /usr/local");
		system("ubidetach /dev/ubi_ctrl -d 0");
		memset(buff,0,sizeof(buff));
		sprintf(buff,"ubiformat /dev/mtd7 -f %s ",buf);
		system(buff);
		//重新挂载/usr/local并且复制pointercal 到目录/usr/local/etc/下
		printf("flash filesystem success ...\n");
		system("ubiattach /dev/ubi_ctrl -m 7");
		system("mount -t ubifs -o sync ubi0:appfs /usr/local");
		memset(buff,0,sizeof(buff));
		sprintf(buff,"cp %s/pointercal /usr/local/etc/",filepath);
		system(buff);
		memset(buff,0,sizeof(buff));
		sprintf(buff,"rm -rf %s/pointercal",filepath);
		system(buff);
	}
	return 0;
}

//显示帮助
void show_help()
{
    printf("\nusage: ./arm-update [-d filepath] [-s filename] [-t PRODUCT_TAG filepath (unfilepath)] [-a num filepath]\n");
}

//显示版本信息
void show_version()
{
    printf("\n---------------------------------------------------------------------\n");
    printf("arm-update tool v0.1 rev 1, build 201304231421\n");
    printf("---------------------------------------------------------------------\n");
}

int check_arg_param(int argc,char *argv[],struct Args_t *args)
{
	pid_t pid;
    int opt = 0;
	char buff[30];
    //需要读取那些命令行参数 ,-d 只解压，后面加路径,-s 单个程序下载后加文件名,-t 执行系统更新后加更新机器类型如，JH5058,-a: can更新选项
    //c 触摸屏校准
    static const char *optString = "d:p:s:t:a:-help:-version:c";
	if(argc !=3 && argc!= 4 && argc!=5 && argc !=2){
		printf("input params error\n");
		return -1;
	}
	args->unPath = NULL;
	args->armNum = 0;
	args->path = NULL;
	args->productName = NULL;
	args->flashFileName = NULL;
    //读命令行参数
    opt = getopt( argc, argv, optString );
    if (opt==-1)
    {
        show_version();
        show_help();
        return -1;
    }
    while ( opt != -1 )
    {
        switch ( opt )
        {
        	case 'a':					//./arm-update -a num path   ,num 更新类型，path 更新文件路径
        		if(argc !=3 && argc !=4){
					printf("params is not correct\n");		
					break;
				}
				args->armNum = atoi(optarg);
				memset(buff,0,sizeof(buff));
				sprintf(buff,"%s/%s",argv[3]==NULL?".":argv[3],CAN_UPDATE_BINARY_FILE);
				args->path = buff;
				operator_type = ONLYUPDATEARM;
				break;
			case 'c':
				if(argc !=2){			//./arm-update -c  
					printf("params is not correct\n");
					break;
				}
				system("fuser -km /usr/local");
			    pid = fork();
			    if(pid < 0){
			        perror("create process");
			        exit(1);
			    }
			    if (pid == 0){
			        execvp("ts_calibrate",NULL);
			    }else if (pid > 0){
			        waitpid(pid, 0, 0);
			    }
				system("cd /usr/local/app");
				system("./app_test -qws &");
				break;
        	case 'd':					//解压 ./arm-update -d JH5058 .
        		if(argc !=4){
					printf("params is not correct\n");
					break;
				}
        		args->productName = optarg;  
				args->path = argv[3];
				operator_type = ONLYUNPACKFILE;
				break;
			case 's':					//单个文件下载 ./arm-update -s appfs.bin /usr
			    if(argc !=3 && argc!=4 ){
					printf("params is not correct\n");
					break;
				}
			    args->flashFileName = optarg;
				args->path = argv[3]==NULL?".":argv[3];
				printf("filename:%s,path:%s\n", args->flashFileName, args->path);
				operator_type = FLASHSINGLEBIN;
				break;
            case 't':					//升级		./arm-update -t JH5058 . .
            	if(argc !=4 && argc !=5){
					printf("params is not correct\n");
					break;
				}
        		args->productName = optarg;  
				operator_type = SYSTEMUPDATE;
				args->path = argv[3];
				args->unPath = argv[4]==NULL?argv[3]:argv[4];
				break;
			case 'h':			//帮助
				show_help();
				break;
			case 'v':			//版本
				show_version();
				break;
        }

        //读命令行参数
        opt = getopt( argc, argv, optString );
    }
	if(args->path ==NULL && args->flashFileName ==NULL && args->productName ==NULL && args->unPath == NULL){
		printf("param error");
		return -1;
	}
    return 0;
}


/**
操作步骤
*/
int main(int argc,char *argv[])
{
	char *filename[20];
	char buff[128];
	//命令行输入参数的结果
    struct Args_t Args;
	int ret = -1;
	char i=0;
	int cmd = -1;

	int fdNum;

char pwdPath[128];
getcwd(pwdPath, 128);
printf("update arm pwd is %s\n",pwdPath);
	for (fdNum = 3; fdNum < sysconf(_SC_OPEN_MAX); fdNum++) {
		close(fdNum);
	}

	if(check_arg_param(argc,argv,&Args)==-1){
		sendSignal(APP_NAME, INPARMERROR, 0);	
		return 0;		
	}
	
	switch(operator_type){
		case ONLYUNPACKFILE:
			ret = unpack(Args.productName,UNPACK_NAME,Args.path,Args.unPath,filename);
			if(ret !=-1){
				printf("\nunpack succ , path is : %s\n",Args.path);
			}else{
				printf("\nunpack error\n");
			}
			break;
		case SYSTEMUPDATE:		
			ret = unpack(Args.productName,UNPACK_NAME,Args.path,Args.unPath,filename);			
			if(ret != -1){
				sendSignal(APP_NAME,UNPACKFINISH,0);
				sleep(1);
				for(i = 0;i<ret ;i++){
					cmd = updateARMMTD(Args.unPath,filename[i]);
					if(cmd == -1){
						printf("updateARMMTD error\n");
						sendSignal(APP_NAME,UPDATEERROR,0);
						return 0;
					}
					memset(buff,0,sizeof(buff));
					sprintf(buff,"rm -rf %s/%s",Args.unPath,filename[i]);
					system(buff);
				}
				sendSignal(APP_NAME,UPDATEFINISH,0);
				if(strcmp(Args.unPath,"/mnt/usbStorage")==0){
					system("fuser -km /mnt/usbStorage");							//卸载U盘
           			system("umount -f /mnt/usbStorage");
				}
				system("reboot");
			} else { 
				printf("update fail \r\n");
			}
			break;
		case FLASHSINGLEBIN:
			updateARMMTD(Args.path,Args.flashFileName);
			break;
		case ONLYUPDATEARM:
			CanFirmware_Update(Args.armNum, Args.path);
	}
	printf("done!\n");
	return 0;
}



