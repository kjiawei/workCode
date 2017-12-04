#ifndef FILE_HEX_H
#define FILE_HEX_H

#define HEX_TYPE_DATA           0
#define HEX_TYPE_END            1
#define HEX_TYPE_ADDR_SEG_EX    2
#define HEX_TYPE_ADDR_LINE_EX   4

#define HEX_DATA_LEN_MAX        32
#define HEX_FLASH_PAGE_SIZE     256

#include <stdio.h>

struct HexInfoHeader {
    unsigned short size;         /* Hex data size */
    unsigned short addr;        /* Downloader address */
    unsigned char type;         /* Hex data type */
};

struct HexInfo {
    struct HexInfoHeader header;
    unsigned char data[256];    /* Hex data */
};

unsigned char *File_OpenRead(char *path);
int File_BufferFree(unsigned char *buf);

int ParseIHexPerLine(const unsigned char *stream,
        struct HexInfoHeader *header,
        unsigned char *data,
        unsigned char **nextstream);

int ParseIHexPage(unsigned char *stream,
        struct HexInfoHeader *header,
        unsigned char *buf,
        unsigned char *chk,
        unsigned char **nextstream);

int ParseIHexPageAdjust(unsigned char *buf,
        struct HexInfoHeader *header);

#endif
