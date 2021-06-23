#pragma once


typedef unsigned char   byte;
typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned int    DWORD;

struct _bitField
{
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int bit3 : 1;
    unsigned int bit4 : 1;
    unsigned int bit5 : 1;
    unsigned int bit6 : 1;
    unsigned int bit7 : 1;
    unsigned int bit8 : 1;
    unsigned int bit9 : 1;
    unsigned int bit10 : 1;
    unsigned int bit11 : 1;
    unsigned int bit12 : 1;
    unsigned int bit13 : 1;
    unsigned int bit14 : 1;
    unsigned int bit15 : 1;
    unsigned int bit16 : 1;
    unsigned int bit17 : 1;
    unsigned int bit18 : 1;
    unsigned int bit19 : 1;
    unsigned int bit20 : 1;
    unsigned int bit21 : 1;
    unsigned int bit22 : 1;
    unsigned int bit23 : 1;
    unsigned int bit24 : 1;
    unsigned int bit25 : 1;
    unsigned int bit26 : 1;
    unsigned int bit27 : 1;
    unsigned int bit28 : 1;
    unsigned int bit29 : 1;
    unsigned int bit30 : 1;
    unsigned int bit31 : 1;
};
typedef struct _bitField bitField;

struct _u16Field
{
    unsigned int L16 : 16;
    unsigned int H16 : 16;
};
typedef struct _u16Field u16Field;


struct _byteField
{
    unsigned int L8 : 8;
    unsigned int ML8 : 8;
    unsigned int MH8 : 8;
    unsigned int H8 : 8;
};
typedef struct _byteField byteField;

struct _u4Field
{
    unsigned int L4 : 4;
    unsigned int elseFiled : 24;
    unsigned int H4 : 4;
};
typedef struct _u4Field u4Field;


union _bitField_union
{
    bitField bf;
    u16Field u16f;
    byteField bytef;
    u4Field u4f;
    int data;
    DWORD data_dw;
};
typedef union _bitField_union bitField_union;


typedef struct oper_status
{
    int RQ_ForJudge;                    // 无符号减的结果，仅能用于判正负
    int SRQ_ForJudge;                   // 有符号减的结果，仅能用于判正负
    bool SRQ_OverFlow;                  // 有符号加\减的溢出标志，双向
    bool SRQ_OverFlow_H;                // 有符号加\减的溢出标志，正向
};