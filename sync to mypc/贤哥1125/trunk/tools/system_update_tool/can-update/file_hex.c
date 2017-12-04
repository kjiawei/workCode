#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "file_hex.h"

unsigned char *File_OpenRead(char *path)
{
    unsigned int len=0;
    FILE *fp=0;
    unsigned char *buf=NULL;

    fp = fopen(path, "rb+");
    if (fp == NULL) {
        return NULL;
    }
    fseek(fp, 0L, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buf = (unsigned char *)malloc(len);
    if (buf == NULL) {
        return NULL;
    }
    memset(buf, 0, len);
    if (fread(buf, 1, len, fp) != len) {
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return buf;
}

int File_BufferFree(unsigned char *buf)
{
    if (buf != NULL) {
        free(buf);
    }
    return 0;
}

int ParseIHexPerLine(const unsigned char *stream,
        struct HexInfoHeader *header,
        unsigned char *data,
        unsigned char **nextstream)
{
    if (stream == NULL) {
        return -1;
    }

    unsigned int nbytes=0,addr=0,type=0,i,val,line_chksum;
    unsigned char cksum;
    char *buf = (char *)stream;

    if(*buf == '\r') buf++;
    if(*buf == '\n') buf++;
    if(*buf == '\r') buf++;
    if(*buf != ':') {
        return -1;
    }
    buf++;

    if(sscanf(buf, "%02x%4x%2X", &nbytes, &addr, &type)!=3) {
        return -1;
    }
    buf += 8;

    if(type == HEX_TYPE_DATA) {
        if(nbytes > HEX_DATA_LEN_MAX) {
            return -1;
        }

        cksum = nbytes+addr+(addr>>8)+type;

        for(i=0; i < nbytes; i++) {
            val=0;
            if(sscanf(buf, "%02X", &val)!=1) {
                return -1;
            }
            buf += 2;
            data[i] = val;
            cksum += val;
        }
        line_chksum = 0;
        if(sscanf(buf, "%02X", &line_chksum) != 1) {
            return -1;
        }
        buf += 2;

        if((cksum+line_chksum)&0xff) {
            return -1;
        }
        header->size = nbytes;
        header->addr = addr;
        header->type = HEX_TYPE_DATA;
        *nextstream = (unsigned char *)buf;
        return 0;
    } else if(type == HEX_TYPE_END) {
        header->size = nbytes;
        header->addr = addr;
        header->type = HEX_TYPE_END;
        *nextstream = NULL;
        return 0;
    } else if(type == HEX_TYPE_ADDR_SEG_EX) {
        if(nbytes > 2) {
            return -1;
        }
        if(sscanf(buf, "%02X", &val)!=1) {
            return -1;
        }
        data[0]=val;
        buf += 2;
        if(sscanf(buf, "%02X", &val)!=1) {
            return -1;
        }
        data[1]=val;
        buf += 4;
        header->size = nbytes;
        header->type = HEX_TYPE_ADDR_SEG_EX;
        *nextstream = (unsigned char *)buf;
        return 0;
    }

    printf("Unknown entry type %d\n", type);
    return -1;
}

int ParseIHexPage(unsigned char *stream,
        struct HexInfoHeader *header,
        unsigned char *buf,
        unsigned char *chk,
        unsigned char **nextstream)
{
    if (buf == NULL) {
        return -1;
    }

    struct HexInfoHeader point;
    unsigned short addr = 0;
    unsigned char *pstr = stream;
    unsigned char *pnext = NULL;
    unsigned char check=0;
    unsigned char data[HEX_DATA_LEN_MAX];
    int len = -1;
    int count =0;
    int i = 0;
    int flag = 0;

    while (count < header->size) {
        addr = header->addr;
        check ^= buf[count++];
        flag = 1;
    }

    while (count < HEX_FLASH_PAGE_SIZE) {
        ParseIHexPerLine(pstr, &point, data, &pnext);

        if (point.type == HEX_TYPE_DATA) {
            if (flag == 0) {
                addr = point.addr&0xFF00;
                header->addr = addr;
                flag = 1;
            }

            if (point.addr-addr >= HEX_FLASH_PAGE_SIZE) {
                while(count < HEX_FLASH_PAGE_SIZE) {
                    check ^= 0xff;
                    buf[count++] = 0xff;
                }
                break;
            }

            pstr = pnext;
            pnext = NULL;
            len = point.addr-count-addr;
            if (len>0) {
                for (i=0; i<len; i++, count++) {
                    check ^= 0xff;
                    buf[count] = 0xff;
                }
            }

            for (i=0; i<point.size; i++, count++) {
                if (count < HEX_FLASH_PAGE_SIZE) {
                    check ^= data[i];
                }
                buf[count] = data[i];
            }
        } else if (point.type == HEX_TYPE_END) {
            if (count == 0) {
                pstr = pnext;
                break;
            }
            for (; count<HEX_FLASH_PAGE_SIZE; count++) {
                check ^= 0xff;
                buf[count] = 0xff;
            }
            point.type = HEX_TYPE_DATA;
            break;
        } else if (point.type == HEX_TYPE_ADDR_SEG_EX) {
            if (count == 0) {
                pstr = pnext;
                buf[0] = data[0];
                buf[1] = data[1];
                break;
            } else {
                for (; count<HEX_FLASH_PAGE_SIZE; count++) {
                    check ^= 0xff;
                    buf[count] = 0xff;
                    point.type = HEX_TYPE_DATA;
                }
            }
        }
    }
    *chk = check;
    *nextstream = pstr;
    header->size = count;
    return point.type;
}

int ParseIHexPageAdjust(unsigned char *buf,
        struct HexInfoHeader *header)
{
    if (header->size > HEX_FLASH_PAGE_SIZE) {
        header->size -= HEX_FLASH_PAGE_SIZE;
        header->addr += HEX_FLASH_PAGE_SIZE;
        memcpy(buf, buf+HEX_FLASH_PAGE_SIZE, header->size);
    } else if (header->size == HEX_FLASH_PAGE_SIZE) {
        header->size = 0;
    }

    return 0;
}
