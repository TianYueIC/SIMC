#pragma once
#include "inc_lib.h"


#define ConstROM_Read_Word  constROM_Read_Word()
#define ConstROM_Read_DWord  constROM_Read_DWord()

extern Sub ConstROM_Read_Word;
extern Sub ConstROM_Read_DWord;


#define RN_Const_StartAddr      0x00000000

// ���°���DWORD��ַ
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
//#define RN_TPI_ADDR             (RN_B_16_ADDR+128)                              // WDRC�ָ��            ����    9*MMU_BASE
//#define RN_tableMod_ADDR        (RN_TPI_ADDR+9)                                 // 0~45��ԭ��            ���� 1024*MMU_BASE
//#define RN_FFT_COFF_ADDR        (RN_tableMod_ADDR+1024)                         // FFTϵ��               ���� 1024*MMU_BASE
//#define RN_ADC_TABLE_ADDR       (RN_FFT_COFF_ADDR+1024)                         // ADCУ����Ĭ��ֵ       ����   35*MMU_BASE
//#define RN_Log2_Table_ADDR      (RN_ADC_TABLE_ADDR+35)                          // �������Log2���ñ�    ����   64*MMU_BASE


#define RN_Log2_Table_ADDR      (RN_Const_StartAddr+0)                          // �������Log2���ñ�    ����   64*MMU_BASE
#define RN_Pow2_Table_ADDR      (RN_Log2_Table_ADDR+64)                         // �������Pow2���ñ�    ����   64*MMU_BASE
#define RN_Recip_Table_ADDR     (RN_Pow2_Table_ADDR+64)                         // �������Recip���ñ�   ����  128*MMU_BASE
#define RN_Sqrt_Table_ADDR      (RN_Recip_Table_ADDR+128)                       // �������Sqrt���ñ�    ����   64*MMU_BASE
//#define RN_Addr_Para_Import     (RN_Sqrt_Table_ADDR+64)                         // Demo�㷨����          ����  169*MMU_BASE
//#define RN_VERIFY_SUM_16_REG_ADDR   (RN_Addr_Para_Import+169)                   // У�麯��ָ����        ����   17*MMU_BASE
//#define RN_Multi_Sin_ADDR       (RN_VERIFY_SUM_16_REG_ADDR+17)                  // Multi_Sin��Ƶ��       ����   32*MMU_BASE
//#define RN_SPLIT_8CH_4SEG_ADDR  (RN_Multi_Sin_ADDR+32)                          // 8ch_4seg���Ӵ�ϵ��    ����  160*MMU_BASE
//#define RN_SPLIT_RST_ADDR       (RN_SPLIT_8CH_4SEG_ADDR+160)                    // 8ch_4seg���Ӵ����    ����  256*MMU_BASE
//#define RN_FMT_RST_ADDR         (RN_SPLIT_RST_ADDR+256)                         // FMT���Խ��           ����  160*MMU_BASE
//
//#define RN_BaseROM_Verify_ADDR  (RN_Const_StartAddr+(RN_CONSTROM_SIZE/4)-1)     // BaseROMУ��ֵ         ����    1*MMU_BASE
