#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct _TARGET_COMPONENT
{
    uint8_t tag[16];
    uint32_t data_size;
    uint8_t reserved[16];
}TARGET_COMPONENT, *P_TARGET_COMPONENT;

typedef struct _UPDATE_PKG
{
    uint32_t CRC;
    uint8_t product_tag[16];
    uint32_t target_components_num;
    P_TARGET_COMPONENT p_targets;
    uint8_t *data;
}UPDATE_PKG;

struct TMP_DATA
{
    uint8_t *data;
    uint32_t size;
    struct TMP_DATA * next;
};

enum UNPACK_ERR_CODE
{
    ERR_NONE = 0,
    ERR_RW,
    ERR_CRC_CHECK,
    ERR_PRODUCT_TAG,
    ERR_UNKNOWN,

    /* ... */
    ERRCODE_NUM
};

enum OPERATOR_TYPE 
{
    FLASHSINGLEBIN = 0,
    ONLYUNPACKFILE,
    SYSTEMUPDATE,
    ONLYUPDATEARM
};



struct Args_t
{
    char * productName;
	char * path;
	char * unPath;
	char * flashFileName;
	int armNum;						//arm can 更新类型
};

#define MAX_PID_NUM     32
#define APP_NAME 						  	"app_test"					//上传信号的应用程序名称
#define UNPACK_NAME						"update.pkg"					//解压的文件
#define CAN_UPDATE_BINARY_FILE          	"CANopen.hex"					//can 更新文件名称
#define LOCAL_APP_NAME 				     "update_system"

//UPDATE CODE
#define UPDATE_BOOTSTRAP	1
#define UPDATE_UBOOT		2
#define UPDATE_PARA1	 	3
#define UPDATE_PARA2     	4
#define UPDATE_LOGO	 	5
#define UPDATE_KERNEL    	6
#define UPDATE_ROOTFS	 	7
#define UPDATE_APPFS	 	8
#define UPDATE_DATA	 	9
#define UPDATEFINISH		10

//ERROR_CODE
#define ERROR_OK			20
#define FILENOTFIND		21
#define FILEISNOTPKG		22
#define MTDISBUSY			23
#define READWRITEERROR	24
#define INPUTPARAMERROR	25
#define UNPACK_ERROR		26
#define FILEPTAGNOTFIT	27
#define UNPACKFILENOTFIND 28
#define CREATETMPFILEERROR 29
#define UPDATEERROR		  30
#define INPARMERROR          31

//UNPACK_CODE
#define UNPACK_BOOTSTRAP	40
#define UNPACK_UBOOT	    41
#define UNPACK_LOGO      	42
#define UNPACK_KERNEL    	43
#define UNPACK_ROOTFS		44
#define UNPACK_APPFS		45
#define UNPACK_DATA		46
#define UNPACK_FILEUNKNOW	47
#define UNPACKFILE			48			
#define UNPACKFINISH     	49 
#define UNPACKCHECKPKG	50



//设备
#define MTD_BOOTSTRAP   	"/dev/mtd0"
#define MTD_UBOOT   		"/dev/mtd1"
#define MTD_PARA1   		"/dev/mtd2"
#define MTD_PARA2 			"/dev/mtd3"
#define MTD_LOGO   		"/dev/mtd4"
#define MTD_KERNEL   		"/dev/mtd5"
#define MTD_ROOTFS   		"/dev/mtd6"
#define MTD_APPFS   		"/dev/mtd7"
#define MTD_DATA   		"/dev/mtd8"
//文件名
#define FILENAME_BOOTSTRAP 		"BOOTSTRAP"
#define FILENAME_UBOOT  			"UBOOT"
#define FILENAME_AUBOOT  			"AUBOOT"
#define FILENAME_LOGO    			"LOGO"
#define FILENAME_KERNEL 			"KERNEL"
#define FILENAME_ROOTFS 			"ROOTFS"
#define FILENAME_APPFS  			"APPFS"

#define FILENAME_BOOTSTRAP_LOW 	"bootstrap.bin"
#define FILENAME_UBOOT_LOW  		"uboot.bin"
#define FILENAME_LOGO_LOW    		"logo.bin"
#define FILENAME_KERNEL_LOW 		"kernel.bin"
#define FILENAME_ROOTFS_LOW 		"rootfs.bin"
#define FILENAME_APPFS_LOW  		"appfs.bin"

#define CFG_ENV_SIZE 128*1024
#define ENV_HEADER_SIZE 4

#endif

