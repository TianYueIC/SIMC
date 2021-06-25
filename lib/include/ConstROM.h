#pragma once
#include "inc_lib.h"


#define ConstROM_Read_Word  constROM_Read_Word()
#define ConstROM_Read_DWord  constROM_Read_DWord()

extern Sub ConstROM_Read_Word;
extern Sub ConstROM_Read_DWord;


#define RN_Const_StartAddr      0x00000000

// 以下按照DWORD编址
//#define RN_K0_1_8_ADDR          (RN_Const_StartAddr+244)
//#define RN_K0_2_8_ADDR          (RN_K0_1_8_ADDR+8)
//#define RN_K0_3_8_ADDR          (RN_K0_2_8_ADDR+8)
//#define RN_K1_1_8_ADDR          (RN_K0_3_8_ADDR+8)
//#define RN_K1_2_8_ADDR          (RN_K1_1_8_ADDR+8)
//#define RN_K1_3_8_ADDR          (RN_K1_2_8_ADDR+8)
//#define RN_B0_1_8_ADDR          (RN_K1_3_8_ADDR+8)
//#define RN_B0_2_8_ADDR          (RN_B0_1_8_ADDR+8)
//#define RN_B0_3_8_ADDR          (RN_B0_2_8_ADDR+8)
//#define RN_B1_1_8_ADDR          (RN_B0_3_8_ADDR+8)
//#define RN_B1_2_8_ADDR          (RN_B1_1_8_ADDR+8)
//#define RN_B1_3_8_ADDR          (RN_B1_2_8_ADDR+8)
//
//#define RN_K_16_ADDR            (RN_B1_3_8_ADDR+8)
//#define RN_B_16_ADDR            (RN_K_16_ADDR+128)
//#define RN_TPI_ADDR             (RN_B_16_ADDR+128)                              // WDRC分割点            长度    9*MMU_BASE
//#define RN_tableMod_ADDR        (RN_TPI_ADDR+9)                                 // 0~45度原表            长度 1024*MMU_BASE
//#define RN_FFT_COFF_ADDR        (RN_tableMod_ADDR+1024)                         // FFT系数               长度 1024*MMU_BASE
//#define RN_ADC_TABLE_ADDR       (RN_FFT_COFF_ADDR+1024)                         // ADC校正表默认值       长度   35*MMU_BASE
//#define RN_Log2_Table_ADDR      (RN_ADC_TABLE_ADDR+35)                          // 定点计算Log2所用表    长度   64*MMU_BASE


#define RN_Log2_Table_ADDR      (RN_Const_StartAddr+0)                          // 定点计算Log2所用表    长度   64*MMU_BASE
#define RN_Pow2_Table_ADDR      (RN_Log2_Table_ADDR+64)                         // 定点计算Pow2所用表    长度   64*MMU_BASE
#define RN_Recip_Table_ADDR     (RN_Pow2_Table_ADDR+64)                         // 定点计算Recip所用表   长度  128*MMU_BASE
#define RN_Sqrt_Table_ADDR      (RN_Recip_Table_ADDR+128)                       // 定点计算Sqrt所用表    长度   64*MMU_BASE
//#define RN_Addr_Para_Import     (RN_Sqrt_Table_ADDR+64)                         // Demo算法参数          长度  169*MMU_BASE
//#define RN_VERIFY_SUM_16_REG_ADDR   (RN_Addr_Para_Import+169)                   // 校验函数指令码        长度   17*MMU_BASE
//#define RN_Multi_Sin_ADDR       (RN_VERIFY_SUM_16_REG_ADDR+17)                  // Multi_Sin音频流       长度   32*MMU_BASE
//#define RN_SPLIT_8CH_4SEG_ADDR  (RN_Multi_Sin_ADDR+32)                          // 8ch_4seg分子带系数    长度  160*MMU_BASE
//#define RN_SPLIT_RST_ADDR       (RN_SPLIT_8CH_4SEG_ADDR+160)                    // 8ch_4seg分子带结果    长度  256*MMU_BASE
//#define RN_FMT_RST_ADDR         (RN_SPLIT_RST_ADDR+256)                         // FMT测试结果           长度  160*MMU_BASE
//
//#define RN_BaseROM_Verify_ADDR  (RN_Const_StartAddr+(RN_CONSTROM_SIZE/4)-1)     // BaseROM校验值         长度    1*MMU_BASE
