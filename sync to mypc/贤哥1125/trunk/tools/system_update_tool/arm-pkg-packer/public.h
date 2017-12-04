#ifndef __MY_PKG_H__
#define __MY_PKG_H__

#include <stdint.h>

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

#endif
