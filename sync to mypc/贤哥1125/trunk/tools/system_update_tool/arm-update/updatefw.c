#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "protocol.h"
#include "file_hex.h"
#include "config_updatefw.h"

int CanFirmware_Update(const char id, char *path)
{
    int ret = -1;
    int pageEx = 0;
    struct HexInfoHeader header;
    unsigned char *ptr = NULL;
    unsigned char *pnext = NULL;
    unsigned char *buf = NULL;
    unsigned char data[HEX_FLASH_PAGE_SIZE*2];
    unsigned char chk;

    CanInit(id);
    if (CanOpen_Slave(id) != 0) {
        printf("Error: CanOpen_Slave\n");
        goto ErrorUpdate;
    }

    buf = File_OpenRead(path);
    if (buf == NULL) {
        goto ErrorUpdate;
    }

    ptr = buf;
    memset(&header, 0, sizeof(struct HexInfoHeader));
    while (ptr != NULL) {
       if (header.size < HEX_FLASH_PAGE_SIZE) {
            ret = ParseIHexPage(ptr, &header, data, &chk, &pnext);
            ptr = pnext;
            if (ret == HEX_TYPE_ADDR_SEG_EX) {
                pageEx = (((unsigned char)(data[1]<<4))+((unsigned char)(data[1]>>4)<<8))+
                    (unsigned char)(data[0]<<4)+(unsigned char)(data[0]>>4);
                continue;
            } else if (ret == HEX_TYPE_END) {
                continue;
            }
        }

        if (CanSend_HexPage(data, pageEx, header.addr/HEX_FLASH_PAGE_SIZE, chk, id) != 0) {
            ret = -1;
            goto ErrorUpdate;
        }
        ParseIHexPageAdjust(data, &header);
    }
    ret = 0;

ErrorUpdate:
    CanSelect_AppRun(id);
    File_BufferFree(buf);
    CanQuit();

    return ret;
}
