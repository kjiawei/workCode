#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h> 
#include <errno.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <stdbool.h>
#include <pthread.h>

#include "UARTorIIC.h"
#include "interface.h"

#define BUF_MAX 512

char ComSet[]                   = {0xAA, 0xBB, 0x03, 0x11, 0x00};
char ComSearchCard[]     = {0xAA, 0xBB, 0x03, 0x20, 0x00};
char ComSetBPS[]             = {0xAA, 0xBB, 0x03, 0x17, 0x01};
char ComWriteOneBlock[] = {0xAA, 0xBB, 0x1a, 0x22, 0x00, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	                                                     0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	                                                     0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
char ComReadOneBlock[] = {0xAA, 0xBB, 0x0a, 0x21, 0x00, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
char ComWriteMulBlocks[] = {0xAA, 0xBB, 0x1c, 0x2B, 0x00, 0x04, 0x02,0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
 	                                                     0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	                                                     0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00};
char ComReadMulBlocks[] = {0xAA, 0xBB, 0x0B, 0x2A, 0x00, 0x04, 0x02, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

char sendBuf[BUF_MAX]={0};
char reveBuf[BUF_MAX]={0};
unsigned char data_buf[256];
struct PersonInfo
{
    int ID;
    unsigned char age;
    char name[30];	
};
struct PersonInfo one;
pthread_t tid;



int open_com()
{
    int fd_com;

    fd_com = open("/dev/ttySAC2",O_RDWR);
    

    if (fd_com < 0)
    {
        printf("can not open serial port\n");
        return -1;
    }

    return fd_com;
}

void close_com(int fd_com)
{
    if (fd_com < 0)
        return ;

    close(fd_com);
}

void config_com(int fd_com,bool isRead)
{
    struct  termios Opt; 
    
    if (fd_com < 0)
        return;
        
    tcgetattr(fd_com, &Opt); 
    tcflush(fd_com,TCIFLUSH);
    
    cfsetispeed(&Opt,B115200);      //set to 115200BPS       
    cfsetospeed(&Opt,B115200);
    

    Opt.c_cflag |= CS8;                 //set to 8bits
    
    Opt.c_cflag &= ~PARENB;    
    Opt.c_oflag &= ~(OPOST); 
    Opt.c_cflag &= ~CSTOPB; 
    Opt.c_cflag &= ~CRTSCTS;      //no flow control
    Opt.c_lflag &= ~(ICANON|ISIG|ECHO|IEXTEN); 
    Opt.c_iflag &= ~(INPCK|BRKINT|ICRNL|ISTRIP|IXON|IXOFF|IXANY);
    
    Opt.c_cc[VMIN] = 64; 
    Opt.c_cc[VTIME] = 1; 
    
    
    if (isRead)
    {
        Opt.c_cflag |= CREAD;       //control mode, port can read data of input
        
    }

    tcflush(fd_com,TCIFLUSH);    //update, write into immediately
    
    
    if (tcsetattr(fd_com,TCSANOW,&Opt) != 0)  //load init arg
    {  
        perror("Setup Serial error!\n");  
        close(fd_com); 
        return ; 
    }

}

void write_com(char * buf,int len)
{
    int fd_com;

    fd_com = open_com();
    
    config_com(fd_com,false);

    write(fd_com,buf,len);

    close_com(fd_com);
}

void read_com(char * buf,int len)
{
    int fd_com;

    
    fd_com = open_com();

    config_com(fd_com,true);
   
    memset(buf,0, sizeof(char) * len);//length = 1byte * len

    read(fd_com,buf,len);

    close_com(fd_com);
}

void printf_com(char * buf,int len)
{
    int i;

    for (i=0;i<len;i++)
    {
        
        printf("%x,",buf[i]);
    }

    printf("\n");
}

void UartSend(char * buf)
{
    int len;
    
    unsigned char i, cCheckSum=0;

   len = *(buf+2);
   for (i=0; i<len; i++)
	   cCheckSum ^= *(buf+2+i);
   *(buf+2+len) = cCheckSum;
   
   len = *(buf+2) + 3;
   write_com(buf, len);
   printf("send data:\n");
   printf_com(buf, len);
}

void setmodel(void)
{
   printf("Into setmodel Function...\n");
   ComSet[4] = ((~ComSet[4]) & 0x02) | 0x01;// set search card model toggle
   UartSend(ComSet);
   read_com(&reveBuf[0],BUF_MAX);
   printf_com(&reveBuf[0],BUF_MAX);
   if(reveBuf[3] != ComSet[3])
     {
         printf("setmodel is fail\n");
     }
}


int searchcard(void)
{
   printf("Into searchcard Function...\n");
   UartSend(ComSearchCard);
   read_com(&reveBuf[0],BUF_MAX);
   printf_com(&reveBuf[0],BUF_MAX);
   if(reveBuf[3] != ComSearchCard[3])
     {
         printf("searchcard is fail\n");
	  return -1;
     }
   else     return 0;
}

int writeoneblock(unsigned char blockid,unsigned char *data,unsigned char len)
{
   
   //char buf[28];
   char buf[29];

   printf("Into writeoneblock Function...\n");
   memcpy(buf, ComWriteOneBlock, 28);
   buf[5] = blockid;
   memcpy(&buf[12], data, len);
   buf[2] = 10+len;
   UartSend(buf);
   read_com(&reveBuf[0],BUF_MAX);
   printf_com(&reveBuf[0],BUF_MAX);
   if(reveBuf[3] != buf[3])
     {
        printf("writeoneblock is fail\n");
	 
	 return 0;
     }
   
   else     return 1;
}

int writemultitudeblocks(unsigned char blockid,unsigned char num,unsigned char *data,unsigned char len)
{
   char buf[256];

   printf("Into writemultitudeblocks Function...\n");
   memcpy(buf, ComWriteMulBlocks, sizeof(ComWriteMulBlocks));
   buf[5] = blockid;
   buf[6] = num;
   memcpy(&buf[13], data, len);
   buf[2] = 11+len;
   UartSend(buf);
   read_com(&reveBuf[0],BUF_MAX);
   printf_com(&reveBuf[0],BUF_MAX);
   if(reveBuf[3] != buf[3])
     {
        printf("writemultitudeblocks is fail\n");
	 return -1;
     }
   else     return 0;
}

int readoneblock(unsigned char blockid,unsigned char *data)
{
   int len;
   
   //char buf[12];
   char buf[13];

   printf("Into readoneblock Function...\n");
   memcpy(buf, ComReadOneBlock, 12);
   buf[5] = blockid;
   UartSend(buf);
   read_com(&reveBuf[0],BUF_MAX);
   printf_com(&reveBuf[0],BUF_MAX);
   if(reveBuf[3] != buf[3])
     {
        printf("readoneblock is fail\n");
	 
	 return 0;
     }
   else
    {
        len = reveBuf[2]+3;
	 memcpy(data, reveBuf, len);
	 return len;
    }
}

int readmultitudeblocks(unsigned char blockid,unsigned char num,unsigned char *data)
{
   int len;
   //char buf[13];
   char buf[14];

   printf("Into readmultitudeblocks Function...\n");
   memcpy(buf, ComReadMulBlocks, 13);
   buf[5] = blockid;
   buf[6] = num;
   UartSend(buf);
   read_com(&reveBuf[0],BUF_MAX);
   printf_com(&reveBuf[0],BUF_MAX);
   if(reveBuf[3] != buf[3])
     {
        printf("readmultitudeblocks is fail\n");
	 return -1;
     }
   else
    {
        len = reveBuf[2]+3;
	 memcpy(data, reveBuf, len);
	 return len;
    }
}


int ReadPersonIDSexBorn(unsigned char *id,unsigned char *born)
{
     

   if(!searchcard())
     {
        
        if(readoneblock(1,data_buf))
          {
	      
	      memcpy(id, &data_buf[4], 9);
	      
	      memcpy(born, &data_buf[13], 4);
	      return 0;
          }
        else return 2;		
     }
   else return 1;
}

int ReadPersonName(unsigned char *name)
{
   

   if(!searchcard())
     {
        if(readmultitudeblocks(4,2,data_buf))
          {
	      memcpy(name, &data_buf[4], 32);
	      return 0;
          }
        else return 2;		
     }
   else return 1;
}

int ReadPersonSign(unsigned char *sign)
{
   

   if(!searchcard())
     {
        if(readmultitudeblocks(8,3,data_buf))
          {
	      memcpy(sign, &data_buf[4], 48);
	      return 0;
          }
        else return 2;		
     }
   else return 1;
}

int ReadPersonDepartment(unsigned char *department)
{
   

   if(!searchcard())
     {
        if(readmultitudeblocks(12,2,data_buf))
          {
	      memcpy(department, &data_buf[4], 32);
	      return 0;
          }
        else return 2;		
     }
   else return 1;
}

int ReadPersonDryWeight(unsigned char *dryweight)
{
      

   if(!searchcard())
     {
        if(readoneblock(2,data_buf))
          {
	      memcpy(dryweight, &data_buf[4], 4);
	      return 0;
          }
        else return 2;		
     }
   else return 1;
}

int ReadPersonDiagnose(unsigned char *diagnose)
{
   

   if(!searchcard())
     {
        if(readmultitudeblocks(16,3,data_buf))
          {
	      memcpy(diagnose, &data_buf[4], 48);
	      return 0;
          }
        else return 2;		
     }
   else return 1;
}

int ReadSetTimeTempNaHco3UFVRate(unsigned char *time,unsigned char *temp,unsigned char *na,unsigned char *hco3,unsigned char *ufv,unsigned char *rate)
{
      

   if(!searchcard())
     {
        if(readoneblock(6,data_buf))
          {
	      memcpy(time, &data_buf[4], 2);
	      memcpy(temp, &data_buf[6], 2);
	      memcpy(na, &data_buf[8], 2);
	      memcpy(hco3, &data_buf[10], 2);
	      memcpy(ufv, &data_buf[12], 2);
	      memcpy(rate, &data_buf[14], 2);
	      return 0;
          }
        else return 2;		
     }
   else return 1;
}

int WriteSetTimeTempNaHco3UFVRate(unsigned char *time,unsigned char *temp,unsigned char *na,unsigned char *hco3,unsigned char *ufv,unsigned char *rate)
{
   unsigned char data[16];   

   if(!searchcard())
     {
       memcpy(data, time, 2);
       memcpy(&data[2], temp, 2);
       memcpy(&data[4], na, 2);
       memcpy(&data[6], hco3, 2);
       memcpy(&data[8], ufv, 2);
       memcpy(&data[10], rate, 2);
	if(writeoneblock(6,data,12)) return 0;
       else return 2;		
     }
   else return 1;
}

