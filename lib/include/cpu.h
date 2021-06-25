#pragma once
#include "reg.h"




#ifdef MMU_BASE
#undef MMU_BASE
#define MMU_BASE 4
#else
#define MMU_BASE 4
#endif



// 寄存器定义
#define REG_DEF CReg

#define RD0 _cpu.rd0
#define RD1 _cpu.rd1
#define RD2 _cpu.rd2
#define RD3 _cpu.rd3
#define RD4 _cpu.rd4
#define RD5 _cpu.rd5
#define RD6 _cpu.rd6
#define RD7 _cpu.rd7
#define RD8 _cpu.rd8
#define RD9 _cpu.rd8
#define RD10 _cpu.rd10
#define RD11 _cpu.rd11
#define RD12 _cpu.rd12
#define RD13 _cpu.rd13
#define RD14 _cpu.rd14
#define RD15 _cpu.rd15

#define RA0 _cpu.ra0
#define RA1 _cpu.ra1
#define RA2 _cpu.ra2
#define RSP _cpu.rsp
#define RA4 _cpu.ra4
#define RA5 _cpu.ra5
#define RA6 _cpu.ra6
#define RA7 _cpu.ra7

#define RD_MEM _cpu.rd_mem  // 特殊的寄存器，用户禁止使用


// 单操作数
// 单操作数:算术移位
#define RF_ShiftL1(x)       CCpu::RF_ShiftX(x, 1)               // 无符号数据算术移位
#define RF_ShiftL2(x)       CCpu::RF_ShiftX(x, 2)               // 无符号数据算术移位
#define RF_ShiftR1(x)       CCpu::RF_ShiftX(x, -1)              // 无符号数据算术移位
#define RF_ShiftR2(x)       CCpu::RF_ShiftX(x, -2)              // 无符号数据算术移位
#define RF_Sft16SR1(x)      CCpu::RF_ShiftX_Sign16(x, -1)       // 有符号数据算术移位
#define RF_Sft16SR2(x)      CCpu::RF_ShiftX_Sign16(x, -2)       // 有符号数据算术移位
#define RF_Sft16SR4(x)      CCpu::RF_ShiftX_Sign16(x, -4)       // 有符号数据算术移位
#define RF_Sft16SR8(x)      CCpu::RF_ShiftX_Sign16(x, -8)       // 有符号数据算术移位
#define RF_Sft32SR1(x)      CCpu::RF_ShiftX_Sign32(x, -1)       // 有符号数据算术移位
#define RF_Sft32SR2(x)      CCpu::RF_ShiftX_Sign32(x, -2)       // 有符号数据算术移位
#define RF_Sft32SR4(x)      CCpu::RF_ShiftX_Sign32(x, -4)       // 有符号数据算术移位
#define RF_Sft32SR8(x)      CCpu::RF_ShiftX_Sign32(x, -8)       // 有符号数据算术移位
// 单操作数:循环移位
#define RF_RotateL1(x)      CCpu::RF_RotateX(x, 1)              // 循环移位
#define RF_RotateL2(x)      CCpu::RF_RotateX(x, 2)              // 循环移位
#define RF_RotateL8(x)      CCpu::RF_RotateX(x, 8)              // 循环移位
#define RF_RotateL16(x)     CCpu::RF_RotateX(x, 16)             // 循环移位
#define RF_RotateL24(x)     CCpu::RF_RotateX(x, 24)             // 循环移位
#define RF_RotateR1(x)      CCpu::RF_RotateX(x, -1)             // 循环移位
#define RF_RotateR2(x)      CCpu::RF_RotateX(x, -2)             // 循环移位
#define RF_RotateR8(x)      CCpu::RF_RotateX(x, -8)             // 循环移位
#define RF_RotateR16(x)     CCpu::RF_RotateX(x, -16)            // 循环移位
#define RF_RotateR24(x)     CCpu::RF_RotateX(x, -24)            // 循环移位
// 单操作数:取寄存器的8位，16位数
#define RF_GetL8(x)         x &= 0xFF
#define RF_GetML8(x)        x = (x >> 8)&0xFF
#define RF_GetMH8(x)        x = (x >> 16)&0xFF
#define RF_GetH8(x)         x = (x >> 24)&0xFF
#define RF_GetL16(x)        x &= 0xFFFF
#define RF_GetH16(x)        x = (x >> 16)&0xFFFF
// 单操作数:RD0字节操作
#define RD0_SetByteL8       _cpu.rd0_SetByteL8()
#define RD0_SetByteML8      _cpu.rd0_SetByteML8()
#define RD0_SetByteMH8      _cpu.rd0_SetByteMH8()
#define RD0_SetByteL24      _cpu.rd0_SetByteL24()
#define RD0_SetByteH8       _cpu.rd0_SetByteH8()
#define RD0_SetByteH8L8     _cpu.rd0_SetByteH8L8()
#define RD0_SetByteH8L16    _cpu.rd0_SetByteH8L16()
#define RD0_SetByteH16      _cpu.rd0_SetByteH16()
#define RD0_SetByteH16L8    _cpu.rd0_SetByteH16L8()
#define RD0_SetByteH24      _cpu.rd0_SetByteH24()
#define RD0_ClrByteL8       _cpu.rd0_ClrByteL8()
#define RD0_ClrByteML8      _cpu.rd0_ClrByteML8()
#define RD0_ClrByteMH8      _cpu.rd0_ClrByteMH8()
#define RD0_ClrByteMH8L8    _cpu.rd0_ClrByteMH8L8()
#define RD0_ClrByteMH8ML8   _cpu.rd0_ClrByteMH8ML8()
#define RD0_ClrByteL24      _cpu.rd0_ClrByteL24()
#define RD0_ClrByteH8       _cpu.rd0_ClrByteH8()
#define RD0_ClrByteH8_L8    _cpu.rd0_ClrByteH8L8()
#define RD0_ClrByteH8ML8    _cpu.rd0_ClrByteH8ML8()
#define RD0_ClrByteH8L16    _cpu.rd0_ClrByteH8L16()
#define RD0_ClrByteH16      _cpu.rd0_ClrByteH16()
#define RD0_ClrByteH16L8    _cpu.rd0_ClrByteH16L8()
#define RD0_ClrByteH24      _cpu.rd0_ClrByteH24()
// 单操作数:RD0扩展符号位
#define RD0_SignExtL8       _cpu.rd0_SignExtL8()
#define RD0_SignExtL16      _cpu.rd0_SignExtL16()
#define RD0_SignExtL24      _cpu.rd0_SignExtL24()
// 单操作数:其他
#define RF_Reverse(x)       CCpu::RF_Reverse(x)                 // 按位逆序
#define RF_LSB2MSB(x)       CCpu::RF_LSB2MSB(x)                 // 按字节逆序
#define RF_MSB2LSB(x)       RF_LSB2MSB(x)                       // 按字节逆序
#define RF_ExchangeL16(x)   CCpu::RF_ExchangeL16(x)             // 低16位的两个字节交换
//#define RF_Bitextent2           NULL
//#define RF_GetBIT_2N            NULL
//#define RF_Tosddata             NULL
//#define RF_Fromsddata           NULL
#define RF_GetH4(x)         CCpu::RF_GetH4(x)                   // 取高4位
#define RF_Get0(x)          x = 0                               // 置全0
#define RF_Set0(x)          RF_Get0(x)                          // 置全0
#define RF_Get1(x)          x = -1;                             // 置全1
#define RF_Set1(x)          RF_Get_1(x)                         // 置全1
#define RF_Abs(x)           x=x<0?(-x):x;                       // 求绝对值
#define RF_Not(x)           x=(-x)-1;                           // 按位取反
#define RF_Parity(x)        CCpu::RF_Parity(x)                  // 偶校验,偶数个1则值为0,奇数个1则值为1
#define RF_Exp(x)           x=1<<(x&0x1F)                       // 2的n次幂,n取x的低五位
#define RF_Log(x)           CCpu::RF_log(x)                     // 求log2的值
#define RF_Neg(x)           RF_Not(x);                          \
                            x += 1                              // 取反加1



// 条件判断
// 条件判断: RD0的每一位判0或判1
#define RD0_Bit0            RD0.GetBitField().bf.bit0           // if(RD0_Bit0==0) if(RD0_Bit0==1)
#define RD0_Bit1            RD0.GetBitField().bf.bit1           // if(RD0_Bit1==0) if(RD0_Bit1==1)
#define RD0_Bit2            RD0.GetBitField().bf.bit2           // if(RD0_Bit2==0) if(RD0_Bit2==1)
#define RD0_Bit3            RD0.GetBitField().bf.bit3           // if(RD0_Bit3==0) if(RD0_Bit3==1)
#define RD0_Bit4            RD0.GetBitField().bf.bit4           // if(RD0_Bit4==0) if(RD0_Bit4==1)
#define RD0_Bit5            RD0.GetBitField().bf.bit5           // if(RD0_Bit5==0) if(RD0_Bit5==1)
#define RD0_Bit6            RD0.GetBitField().bf.bit6           // if(RD0_Bit6==0) if(RD0_Bit6==1)
#define RD0_Bit7            RD0.GetBitField().bf.bit7           // if(RD0_Bit7==0) if(RD0_Bit7==1)
#define RD0_Bit8            RD0.GetBitField().bf.bit8           // if(RD0_Bit8==0) if(RD0_Bit8==1)
#define RD0_Bit9            RD0.GetBitField().bf.bit9           // if(RD0_Bit9==0) if(RD0_Bit9==1)
#define RD0_Bit10           RD0.GetBitField().bf.bit10          // if(RD0_Bit10==0) if(RD0_Bit10==1)
#define RD0_Bit11           RD0.GetBitField().bf.bit11          // if(RD0_Bit11==0) if(RD0_Bit11==1)
#define RD0_Bit12           RD0.GetBitField().bf.bit12          // if(RD0_Bit12==0) if(RD0_Bit12==1)
#define RD0_Bit13           RD0.GetBitField().bf.bit13          // if(RD0_Bit13==0) if(RD0_Bit13==1)
#define RD0_Bit14           RD0.GetBitField().bf.bit14          // if(RD0_Bit14==0) if(RD0_Bit14==1)
#define RD0_Bit15           RD0.GetBitField().bf.bit15          // if(RD0_Bit15==0) if(RD0_Bit15==1)
#define RD0_Bit16           RD0.GetBitField().bf.bit16          // if(RD0_Bit16==0) if(RD0_Bit16==1)
#define RD0_Bit17           RD0.GetBitField().bf.bit17          // if(RD0_Bit17==0) if(RD0_Bit17==1)
#define RD0_Bit18           RD0.GetBitField().bf.bit18          // if(RD0_Bit18==0) if(RD0_Bit18==1)
#define RD0_Bit19           RD0.GetBitField().bf.bit19          // if(RD0_Bit19==0) if(RD0_Bit19==1)
#define RD0_Bit20           RD0.GetBitField().bf.bit20          // if(RD0_Bit20==0) if(RD0_Bit20==1)
#define RD0_Bit21           RD0.GetBitField().bf.bit21          // if(RD0_Bit21==0) if(RD0_Bit21==1)
#define RD0_Bit22           RD0.GetBitField().bf.bit22          // if(RD0_Bit22==0) if(RD0_Bit22==1)
#define RD0_Bit23           RD0.GetBitField().bf.bit23          // if(RD0_Bit23==0) if(RD0_Bit23==1)
#define RD0_Bit24           RD0.GetBitField().bf.bit24          // if(RD0_Bit24==0) if(RD0_Bit24==1)
#define RD0_Bit25           RD0.GetBitField().bf.bit25          // if(RD0_Bit25==0) if(RD0_Bit25==1)
#define RD0_Bit26           RD0.GetBitField().bf.bit26          // if(RD0_Bit26==0) if(RD0_Bit26==1)
#define RD0_Bit27           RD0.GetBitField().bf.bit27          // if(RD0_Bit27==0) if(RD0_Bit27==1)
#define RD0_Bit28           RD0.GetBitField().bf.bit28          // if(RD0_Bit28==0) if(RD0_Bit28==1)
#define RD0_Bit29           RD0.GetBitField().bf.bit29          // if(RD0_Bit29==0) if(RD0_Bit29==1)
#define RD0_Bit30           RD0.GetBitField().bf.bit30          // if(RD0_Bit30==0) if(RD0_Bit30==1)
#define RD0_Bit31           RD0.GetBitField().bf.bit31          // if(RD0_Bit31==0) if(RD0_Bit31==1)
// 条件判断: RD0的4，8，16，32位判0 或 非0
#define RD0_Zero            RD0==0                              // if(RD0_Zero)     RD0等于0
#define RD0_nZero           RD0!=0                              // if(RD0_nZero)    RD0不等于0
#define RD0_L4              RD0.GetBitField().u4f.L4            // if(RD0_L4==0)  if(RD0_L4!=0)    RD0的低4位
#define RD0_L8              RD0.GetBitField().bytef.L8          // if(RD0_L8==0)  if(RD0_L8!=0)    RD0的低8位
#define RD0_ML8             RD0.GetBitField().bytef.ML8         // if(RD0_ML8==0) if(RD0_ML8!=0)   RD0的次低8位
#define RD0_MH8             RD0.GetBitField().bytef.MH8         // if(RD0_MH8==0) if(RD0_MH8!=0)   RD0的次高8位
#define RD0_H8              RD0.GetBitField().bytef.H8          // if(RD0_H8==0)  if(RD0_H8!=0)    RD0的高8位
#define RD0_L16             RD0.GetBitField().u16f.L16          // if(RD0_L16==0) if(RD0_L16!=0)   RD0的低16位
#define RD0_H16             RD0.GetBitField().u16f.H16          // if(RD0_H16==0) if(RD0_L16!=0)   RD0的高16位
// 条件判断: 缓存寄存器RQ的每一位判0或判1
#define RQ_Bit0             CReg::GetBitFieldRQ().bf.bit0       // if(RQ_Bit0==0) if(RQ_Bit0==1)
#define RQ_Bit1             CReg::GetBitFieldRQ().bf.bit1       // if(RQ_Bit1==0) if(RQ_Bit1==1)
#define RQ_Bit2             CReg::GetBitFieldRQ().bf.bit2       // if(RQ_Bit2==0) if(RQ_Bit2==1)
#define RQ_Bit3             CReg::GetBitFieldRQ().bf.bit3       // if(RQ_Bit3==0) if(RQ_Bit3==1)
#define RQ_Bit4             CReg::GetBitFieldRQ().bf.bit4       // if(RQ_Bit4==0) if(RQ_Bit4==1)
#define RQ_Bit5             CReg::GetBitFieldRQ().bf.bit5       // if(RQ_Bit5==0) if(RQ_Bit5==1)
#define RQ_Bit6             CReg::GetBitFieldRQ().bf.bit6       // if(RQ_Bit6==0) if(RQ_Bit6==1)
#define RQ_Bit7             CReg::GetBitFieldRQ().bf.bit7       // if(RQ_Bit7==0) if(RQ_Bit7==1)
#define RQ_Bit8             CReg::GetBitFieldRQ().bf.bit8       // if(RQ_Bit8==0) if(RQ_Bit8==1)
#define RQ_Bit9             CReg::GetBitFieldRQ().bf.bit9       // if(RQ_Bit9==0) if(RQ_Bit9==1)
#define RQ_Bit10            CReg::GetBitFieldRQ().bf.bit10      // if(RQ_Bit10==0) if(RQ_Bit10==1)
#define RQ_Bit11            CReg::GetBitFieldRQ().bf.bit11      // if(RQ_Bit11==0) if(RQ_Bit11==1)
#define RQ_Bit12            CReg::GetBitFieldRQ().bf.bit12      // if(RQ_Bit12==0) if(RQ_Bit12==1)
#define RQ_Bit13            CReg::GetBitFieldRQ().bf.bit13      // if(RQ_Bit13==0) if(RQ_Bit13==1)
#define RQ_Bit14            CReg::GetBitFieldRQ().bf.bit14      // if(RQ_Bit14==0) if(RQ_Bit14==1)
#define RQ_Bit15            CReg::GetBitFieldRQ().bf.bit15      // if(RQ_Bit15==0) if(RQ_Bit15==1)
#define RQ_Bit16            CReg::GetBitFieldRQ().bf.bit16      // if(RQ_Bit16==0) if(RQ_Bit16==1)
#define RQ_Bit17            CReg::GetBitFieldRQ().bf.bit17      // if(RQ_Bit17==0) if(RQ_Bit17==1)
#define RQ_Bit18            CReg::GetBitFieldRQ().bf.bit18      // if(RQ_Bit18==0) if(RQ_Bit18==1)
#define RQ_Bit19            CReg::GetBitFieldRQ().bf.bit19      // if(RQ_Bit19==0) if(RQ_Bit19==1)
#define RQ_Bit20            CReg::GetBitFieldRQ().bf.bit20      // if(RQ_Bit20==0) if(RQ_Bit20==1)
#define RQ_Bit21            CReg::GetBitFieldRQ().bf.bit21      // if(RQ_Bit21==0) if(RQ_Bit21==1)
#define RQ_Bit22            CReg::GetBitFieldRQ().bf.bit22      // if(RQ_Bit22==0) if(RQ_Bit22==1)
#define RQ_Bit23            CReg::GetBitFieldRQ().bf.bit23      // if(RQ_Bit23==0) if(RQ_Bit23==1)
#define RQ_Bit24            CReg::GetBitFieldRQ().bf.bit24      // if(RQ_Bit24==0) if(RQ_Bit24==1)
#define RQ_Bit25            CReg::GetBitFieldRQ().bf.bit25      // if(RQ_Bit25==0) if(RQ_Bit25==1)
#define RQ_Bit26            CReg::GetBitFieldRQ().bf.bit26      // if(RQ_Bit26==0) if(RQ_Bit26==1)
#define RQ_Bit27            CReg::GetBitFieldRQ().bf.bit27      // if(RQ_Bit27==0) if(RQ_Bit27==1)
#define RQ_Bit28            CReg::GetBitFieldRQ().bf.bit28      // if(RQ_Bit28==0) if(RQ_Bit28==1)
#define RQ_Bit29            CReg::GetBitFieldRQ().bf.bit29      // if(RQ_Bit29==0) if(RQ_Bit29==1)
#define RQ_Bit30            CReg::GetBitFieldRQ().bf.bit30      // if(RQ_Bit30==0) if(RQ_Bit30==1)
#define RQ_Bit31            CReg::GetBitFieldRQ().bf.bit31      // if(RQ_Bit31==0) if(RQ_Bit31==1) 
// 条件判断: RQ的4，8，16，32位判0 或 非0
#define RQ_Zero             CReg::GetBitFieldRQ().data==0       // if(RQ_Zero)   RQ等于0
#define RQ_nZero            CReg::GetBitFieldRQ().data!=0       // if(RQ_nZero)  RQ不等于0
#define RQ_L4               CReg::GetBitFieldRQ().u4f.L4        // if(RQ_L4==0)  if(RQ_L4!=0)    RQ的低4位
#define RQ_L8               CReg::GetBitFieldRQ().bytef.L8      // if(RQ_L8==0)  if(RQ_L8!=0)    RQ的低8位
#define RQ_ML8              CReg::GetBitFieldRQ().bytef.ML8     // if(RQ_ML8==0) if(RQ_ML8!=0)   RQ的次低8位
#define RQ_MH8              CReg::GetBitFieldRQ().bytef.MH8     // if(RQ_MH8==0) if(RQ_MH8!=0)   RQ的次高8位
#define RQ_H8               CReg::GetBitFieldRQ().bytef.H8      // if(RQ_H8==0)  if(RQ_H8!=0)    RQ的高8位
#define RQ_L16              CReg::GetBitFieldRQ().u16f.L16      // if(RQ_L16==0) if(RQ_L16!=0)   RQ的低16位
#define RQ_H16              CReg::GetBitFieldRQ().u16f.H16      // if(RQ_H16==0) if(RQ_H16!=0)   RQ的高16位
// 条件判断: RQ>0 RQ<=0 RQ>=0 RQ<0 SRQ>=0 SRQ<0 SRQ>0 SRQ<=0
#define RQ                  CReg::GetOperStatus().RQ_ForJudge
#define SRQ                 CReg::GetOperStatus().SRQ_ForJudge
// 条件判断: 
#define RQ_Carry            RQ>=0
#define RQ_nBorrow          RQ_Carry
#define RQ_nCarry           RQ<0
#define RQ_Borrow           RQ_nCarry
#define RQ_OverFlow         CReg::GetOperStatus().SRQ_OverFlow
#define RQ_OverFlow_H       CReg::GetOperStatus().SRQ_OverFlow_H

class CCpu
{
public:
    REG_DEF rd0 = REG_DEF(0, 0);
    REG_DEF rd1 = REG_DEF(0, 1);
    REG_DEF rd2 = REG_DEF(0, 2);
    REG_DEF rd3 = REG_DEF(0, 3);
    REG_DEF rd4 = REG_DEF(0, 4);
    REG_DEF rd5 = REG_DEF(0, 5);
    REG_DEF rd6 = REG_DEF(0, 6);
    REG_DEF rd7 = REG_DEF(0, 7);
    REG_DEF rd8 = REG_DEF(0, 8);
    REG_DEF rd9 = REG_DEF(0, 9);
    REG_DEF rd10 = REG_DEF(0, 10);
    REG_DEF rd11 = REG_DEF(0, 11);
    REG_DEF rd12 = REG_DEF(0, 12);
    REG_DEF rd13 = REG_DEF(0, 13);
    REG_DEF rd14 = REG_DEF(0, 14);
    REG_DEF rd15 = REG_DEF(0, 15);

    REG_DEF ra0 = REG_DEF(0, 24);
    REG_DEF ra1 = REG_DEF(0, 25);
    REG_DEF ra2 = REG_DEF(0, 27);
    REG_DEF rsp = REG_DEF(0, 27);
    REG_DEF ra4 = REG_DEF(0, 28);
    REG_DEF ra5 = REG_DEF(0, 29);
    REG_DEF ra6 = REG_DEF(0, 30);
    REG_DEF ra7 = REG_DEF(0, 31);

    REG_DEF rd_mem = REG_DEF(0, REG_DEF::ID_MEM); // 专用于M[]操作

    // 单操作数
    static void RF_RotateX(CReg& data, int n);
    static void RF_ShiftX(CReg& data, int n);
    static void RF_ShiftX_Sign32(CReg& data, int n);
    static void RF_ShiftX_Sign16(CReg& data, int n);
    static void RF_Reverse(CReg& data);
    static void RF_LSB2MSB(CReg& data);
    static void RF_GetH4(CReg& data);
    static void RF_ExchangeL16(CReg& data);
    static void RF_Parity(CReg& data);
    static void RF_log(CReg& data);
    void rd0_SetByteL8();
    void rd0_SetByteML8();
    void rd0_SetByteMH8();
    void rd0_SetByteL24();
    void rd0_SetByteH8();
    void rd0_SetByteH8L8();
    void rd0_SetByteH8L16();
    void rd0_SetByteH16();
    void rd0_SetByteH16L8();
    void rd0_SetByteH24();
    void rd0_ClrByteL8();
    void rd0_ClrByteML8();
    void rd0_ClrByteMH8();
    void rd0_ClrByteMH8L8();
    void rd0_ClrByteMH8ML8();
    void rd0_ClrByteL24();
    void rd0_ClrByteH8();
    void rd0_ClrByteH8L8();
    void rd0_ClrByteH8ML8();
    void rd0_ClrByteH8L16();
    void rd0_ClrByteH16();
    void rd0_ClrByteH16L8();
    void rd0_ClrByteH24();
    void rd0_SignExtL8();
    void rd0_SignExtL16();
    void rd0_SignExtL24();
    // 条件判断
    bool RD0_Bit0_0()
    {
        return (rd0.GetBitField().bf.bit0 == 0);
    }

private:
    static int parity_even_byte(DWORD data);
    static bool log2_byte(DWORD data, int& rst);

private:
    static bitField_union rd0_bit;
};


extern CCpu _cpu;
