#pragma once
#include "inc_lib.h"


#define FRAME_LEN_Word  32                                     // 一帧32点
#define FRAME_LEN_DWord 16                                     // 一帧32点,紧凑排列占据16DWORD


// 全局变量
#define RN_GLOBAL_VAR_TOTAL_LEN_B   256       // 全局变量字节长度(用户自行修改)
#define g_0                       M[RA4+0*MMU_BASE]           // 举个例子
#define g_1                       M[RA4+1*MMU_BASE]           // 举个例子


// RN_GRAMx RN_XRAMx类似于全局空间，无需申请，读写必须通过MEMCPY
#define RN_GRAM0                BASERAM_SIZE_DWORD*MMU_BASE                     // 1024Byte		CMemBase::pTotalRam+0x400
#define RN_GRAM1                (RN_GRAM0+1024)                                 // 1024Byte
#define RN_GRAM2                (RN_GRAM1+1024)                                 // 1024Byte
#define RN_GRAM3                (RN_GRAM2+1024)                                 // 1024Byte
#define RN_GRAM4                (RN_GRAM3+1024)                                 // 1024Byte
#define RN_GRAM5                (RN_GRAM4+1024)                                 // 1024Byte
#define RN_GRAM6                (RN_GRAM5+1024)                                 // 1024Byte
#define RN_GRAM7                (RN_GRAM6+1024)                                 // 1024Byte
#define RN_GRAM8                (RN_GRAM7+1024)                                 // 1024Byte
#define RN_GRAM9                (RN_GRAM8+1024)                                 // 1024Byte
#define RN_GRAM10               (RN_GRAM9+1024)                                 // 1024Byte
#define RN_GRAM11               (RN_GRAM10+1024)                                // 1024Byte
#define RN_GRAM12               (RN_GRAM11+1024)                                // 1024Byte
#define RN_GRAM13               (RN_GRAM12+1024)                                // 1024Byte
#define RN_GRAM14               (RN_GRAM13+1024)                                // 1024Byte
#define RN_GRAM15               (RN_GRAM14+1024)                                // 1024Byte
#define RN_XRAM0                (RN_GRAM15+1024)                                // 1024Byte
#define RN_XRAM1                (RN_XRAM0+1024)                                 // 1024Byte
#define RN_XRAM2                (RN_XRAM1+1024)                                 // 1024Byte
#define RN_XRAM3                (RN_XRAM2+1024)                                 // 1024Byte
#define RN_XRAM4                (RN_XRAM3+1024)                                 // 1024Byte
#define RN_XRAM5                (RN_XRAM4+1024)                                 // 1024Byte
#define RN_XRAM6                (RN_XRAM5+1024)                                 // 1024Byte
#define RN_XRAM7                (RN_XRAM6+1024)                                 // 1024Byte

#define RN_RAM_CONV0            ((BASERAM_SIZE_DWORD + GRAM_SIZE_DWORD + XRAM_SIZE_DWORD)*MMU_BASE)		//32k	CMemBase::pTotalRam+0x1C00
#define RN_RAM_CONV1            (RN_RAM_CONV0+131072)													//32k	CMemBase::pTotalRam+0x9C00

// 对GRAM的使用定义如下
// ============================ GRAM0 ============================
#define RN_GRAM_IN              RN_GRAM0                                        // 512Byte
#define RN_GRAM_IN2             (RN_GRAM_IN+32*MMU_BASE)                        // 512Byte
#define RN_GRAM_OUT             RN_GRAM0                                        // 512Byte