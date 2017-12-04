#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdint.h>

#define CFG_ENV_SIZE 128*1024
#define ENV_HEADER_SIZE 4

//显示帮助
void show_help()
{
    printf("\nusage: ./updater dir_name\n");
    //printf("other arguments: -v(-version), -h(-?, -help) \n");
}

//显示版本信息
void show_version()
{
    printf("\n---------------------------------------------------------------------\n");
    printf("updater tool v0.9 rev 183, build 201207181035\n");
    printf("---------------------------------------------------------------------\n");
}

//分析目录是否已umount成功
//即分析目录下面是不是空的。
int isDirEmpty(char *path)
{
    DIR *pDir;
    struct dirent *ent;
    bool first;

    pDir = opendir(path);
    
    //如果没有那个目录，则返回-1
    if (pDir == NULL)
    {
        return -1;
    }

    //一个文件都没有找到
    first = false;

    while ( (ent=readdir(pDir)) != NULL )
    {
        if ( ( strcmp(ent->d_name,".") == 0 ) || ( strcmp(ent->d_name,"..") == 0 ) )
        {
            continue;
        }
        else
        {
            //找到一个文件
            first = true;
            break;
        }
    }
   
    //关闭打开的目录
    closedir(pDir);

    //如果那个目录存在，并找到文件，则返回1
    //如果那个目录存在，并没有找到文件，则返回0
    if (first == false)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}


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

int set_rootfs_argv()
{
    //char * kernelFileName = "KENCEL";
    char * rootfsFileName = "ROOTFS";

    //int kernelSize = get_file_size(kernelFileName);
    int kernelSize = 0x300000;
    int rootfsSize = get_file_size(rootfsFileName);

    char * kernelOffset = "0x001a0000";
    char * rootfsOffset = "0x004a0000";

    char * kernelSdramAddr = "0x72000000";
    char * rootfsSdramAddr = "0x71000000";

    unsigned char buf[CFG_ENV_SIZE] = {0};
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

    fp = fopen("tmp.sh","w");
    if (fp == NULL)
    {
        printf("can not open tmp.sh file\n");
        return false;
    }

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

    fwrite(buf, CFG_ENV_SIZE, 1, fp);

    fclose(fp);

    return true;
}

int my_system(const char * cmdstring,char * ps_argv[])
{
    pid_t pid;
    int stat_val;

    if (cmdstring == NULL)
    {
        return -1;
    }

    if ((pid = fork())<0)
    {
        return -1;
    }
    else if (pid == 0)
    {
        execvp(cmdstring,ps_argv);
    }
    else
    {
        wait(&stat_val);

        if (WIFEXITED(stat_val))
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

    return 0;
}


int check(char * file_name)
{
    if ( ( strcmp(file_name,"U-BOOT") ) == 0  )
    {
        return true;
    }
    else if ( ( strcmp(file_name,"U-BOOT-LOGO") ) == 0 )
    {
        return true;
    }
    else if ( ( strcmp(file_name,"KERNEL") ) == 0  )
    {
        return true;
    }
    else if ( ( strcmp(file_name,"ROOTFS") ) == 0 )
    {
        return true;
    }
    else if ( ( strcmp(file_name,"APPFS") ) == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

int update_file(char * file_name)
{
    char * ps_argv[1024] = {0};
    int val;

    if ( ( strcmp(file_name,"U-BOOT") ) == 0 )
    {
        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "flash_eraseall";
        ps_argv[1] = "/dev/mtd1";
        ps_argv[2] = 0;
        val = my_system("flash_eraseall",ps_argv);
        if (val)
        {
            printf("flash_eraseall /dev/mtd1 error\n");
            return false;
        }
        printf("flash_eraseall finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "nandwrite";
        ps_argv[1] = "/dev/mtd1";
        ps_argv[2] = "U-BOOT";
        ps_argv[3] = "-f";
        ps_argv[4] = "-p";
        ps_argv[5] = 0;
        val = my_system("nandwrite",ps_argv);
        if (val)
        {
            printf("nandwrite /dev/mtd1 error\n");
            return false;
        }
        printf("nandwrite finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "rm";
        ps_argv[1] = "U-BOOT";
        ps_argv[2] = 0;
        val = my_system("rm",ps_argv);
        if (val)
        {
            printf("rm U-BOOT error\n");
            return false;
        }

        printf("%s is finish\n",file_name);

        return true;
    }
    else if ( ( strcmp(file_name,"U-BOOT-LOGO") ) == 0 )
    {

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "flash_eraseall";
        ps_argv[1] = "/dev/mtd4";
        ps_argv[2] = 0;
        val = my_system("flash_eraseall",ps_argv);
        if (val)
        {
            printf("flash_eraseall /dev/mtd4 error\n");
            return false;
        }
        printf("flash_eraseall finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "nandwrite";
        ps_argv[1] = "/dev/mtd4";
        ps_argv[2] = "U-BOOT-LOGO";
        ps_argv[3] = "-f";
        ps_argv[4] = "-p";
        ps_argv[5] = 0;
        val = my_system("nandwrite",ps_argv);
        if (val)
        {
            printf("nandwrite /dev/mtd4 error\n");
            return false;
        }
        printf("nandwrite finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "rm";
        ps_argv[1] = "U-BOOT-LOGO";
        ps_argv[2] = 0;
        val = my_system("rm",ps_argv);
        if (val)
        {
            printf("rm U-BOOT-LOGO error\n");
            return false;
        }
        printf("%s is finish\n",file_name);
        return true;
    }
    else if ( ( strcmp(file_name,"KERNEL") ) == 0 )
    {

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "flash_eraseall";
        ps_argv[1] = "/dev/mtd5";
        ps_argv[2] = 0;
        val = my_system("flash_eraseall",ps_argv);
        if (val)
        {
            printf("flash_eraseall /dev/mtd5 error\n");
            return false;
        }
        printf("flash_eraseall finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "nandwrite";
        ps_argv[1] = "/dev/mtd5";
        ps_argv[2] = "KERNEL";
        ps_argv[3] = "-f";
        ps_argv[4] = "-p";
        ps_argv[5] = 0;
        val = my_system("nandwrite",ps_argv);
        if (val)
        {
            printf("nandwrite /dev/mtd5 error\n");
            return false;
        }
        printf("nandwrite finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "rm";
        ps_argv[1] = "KERNEL";
        ps_argv[2] = 0;
        val = my_system("rm",ps_argv);
        if (val)
        {
            printf("rm KERNEL error\n");
            return false;
        }
        printf("%s is finish\n",file_name);
        return true;
    }
    else if ( ( strcmp(file_name,"ROOTFS") ) == 0 )
    {
        if (!set_rootfs_argv())
        {
            printf("build rootfs argv error\n");
            return false;
        }

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "nandwrite";
        ps_argv[1] = "/dev/mtd6";
        ps_argv[2] = "ROOTFS";
        ps_argv[3] = "-f";
        ps_argv[4] = "-p";
        ps_argv[5] = 0;
        val = my_system("nandwrite",ps_argv);
        if (val)
        {
            printf("nandwrite /dev/mtd6 error\n");
            return false;
        }
        printf("nandwrite finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "flash_eraseall";
        ps_argv[1] = "/dev/mtd6";
        ps_argv[2] = 0;
        val = my_system("flash_eraseall",ps_argv);
        if (val)
        {
            printf("flash_eraseall /dev/mtd6 error\n");
            return false;
        }
        printf("flash_eraseall finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "nandwrite";
        ps_argv[1] = "/dev/mtd6";
        ps_argv[2] = "ROOTFS";
        ps_argv[3] = "-f";
        ps_argv[4] = "-p";
        ps_argv[5] = 0;
        val = my_system("nandwrite",ps_argv);
        if (val)
        {
            printf("nandwrite /dev/mtd6 error\n");
            return false;
        }
        printf("nandwrite finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "flash_eraseall";
        ps_argv[1] = "/dev/mtd2";
        ps_argv[2] = 0;
        val = my_system("flash_eraseall",ps_argv);
        if (val)
        {
            printf("flash_eraseall /dev/mtd2 error\n");
            return false;
        }

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "nandwrite";
        ps_argv[1] = "/dev/mtd2";
        ps_argv[2] = "tmp.sh";
        ps_argv[3] = "-f";
        ps_argv[4] = "-p";
        ps_argv[5] = 0;
        val = my_system("nandwrite",ps_argv);
        if (val)
        {
            printf("nandwrite /dev/mtd2 error\n");
            return false;
        }
        printf("nandwrite finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "flash_eraseall";
        ps_argv[1] = "/dev/mtd3";
        ps_argv[2] = 0;
        val = my_system("flash_eraseall",ps_argv);
        if (val)
        {
            printf("flash_eraseall /dev/mtd3 error\n");
            return false;
        }

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "nandwrite";
        ps_argv[1] = "/dev/mtd3";
        ps_argv[2] = "tmp.sh";
        ps_argv[3] = "-f";
        ps_argv[4] = "-p";
        ps_argv[5] = 0;
        val = my_system("nandwrite",ps_argv);
        if (val)
        {
            printf("nandwrite /dev/mtd3 error\n");
            return false;
        }
        printf("nandwrite finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "rm";
        ps_argv[1] = "ROOTFS";
        ps_argv[2] = 0;
        val = my_system("rm",ps_argv);
        if (val)
        {
            printf("rm ROOTFS error\n");
            return false;
        }

        printf("%s is finish\n",file_name);
        return true;
    }
    else if ( ( strcmp(file_name,"APPFS") ) == 0 )
    {

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "umount";
        ps_argv[1] = "/usr/local/";
        ps_argv[2] = 0;
        val = my_system("umount",ps_argv);
        if (val)
        {
            printf("umount /usr/local error\n");
            return false;
        }

        if ( (isDirEmpty("/usr/local/")) != 1 )
        {
            printf("umount /usr/local error\n");
            return false;
        }

        printf("umount finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "ubidetach";
        ps_argv[1] = "/dev/ubi_ctrl";
        ps_argv[2] = "-d";
        ps_argv[3] = "0";
        ps_argv[4] = 0;
        val = my_system("ubidetach",ps_argv);
        if (val)
        {
            printf("ubidetach error\n");
            return false;
        }
        printf("ubidetach finish\n");

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "ubiformat";
        ps_argv[1] = "/dev/mtd7";
        ps_argv[2] = "-f";
        ps_argv[3] = "APPFS";
        val = my_system("ubiformat",ps_argv);
        if (val)
        {
            printf("ubiformat error\n");
            return false;
        }

        memset(ps_argv,0,sizeof(ps_argv));
        ps_argv[0] = "rm";
        ps_argv[1] = "APPFS";
        ps_argv[2] = 0;
        val = my_system("rm",ps_argv);
        if (val)
        {
            printf("rm APPFS error\n");
            return false;
        }
        printf("%s is finish\n",file_name);
        return true;
    }
    else
    {
        return false;
    }
}

int update(char * dir_name)
{
    //
    DIR *dirp;
    //
    struct dirent *ptr;
    //
    char old_dir[1024];

    char * ps_argv[1024] = {0};

    //打开指定目录
    dirp = opendir(dir_name);
    if (dirp == NULL)
    {
        printf("can not open this dir\n");
        return false;
    }

    getcwd(&old_dir[0],1024);
    chdir(dir_name);

    //扫描目录，如果有需要更新的文件，就去更新文件
    while( ( ptr = readdir(dirp) ) != NULL )
    {
        if ( check(ptr->d_name) )
        {
            if ( !update_file(ptr->d_name) )
            {
                chdir(old_dir);
                //关闭目录
                closedir(dirp);
                return false;
            }
        }
    }

    chdir(old_dir);

    //关闭目录
    closedir(dirp);
    //重启
    memset(ps_argv,0,sizeof(ps_argv));
    ps_argv[0] = "reboot";
    ps_argv[1] = 0;
    my_system("reboot",ps_argv);

    return true;
}

int get_dir_name(int argc,char *argv[],char * dir_name)
{
    if ( argc == 1 )
    {
        show_version();
        show_help();
        return false;
    }

    if ( argc != 2 )
    {
        printf("please input only one dir name\n");
        return false;
    }

    strcpy(dir_name,argv[1]);

    return true;
}

int main(int argc,char *argv[])
{
    char dir_name[1024] = {0};

    if (!get_dir_name(argc,argv,dir_name))
    {
        return -1;
    }

    if (!update(dir_name))
    {
        return -1;
    }

    return 0;
}
