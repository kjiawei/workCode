#ifndef UARTORIIC_H
#define  UARTORIIC_H

extern char ComSet[];
extern char ComSearchCard[];
extern char ComSetBPS[];
extern char ComWriteOneBlock[];
extern char ComReadOneBlock[];
extern char ComWriteMulBlocks[];
extern char ComReadMulBlocks[];

int open_com();
void close_com(int);
void config_com(int,bool);
void write_com(char *,int);
void read_com(char *,int);
void printf_com(char *,int);
void UartSend(char *);

int searchcard(void);
int writeoneblock(unsigned char ,unsigned char *,unsigned char );
int writemultitudeblocks(unsigned char ,unsigned char ,unsigned char *,unsigned char );
int readoneblock(unsigned char ,unsigned char *);
int readmultitudeblocks(unsigned char ,unsigned char ,unsigned char *);

#endif
