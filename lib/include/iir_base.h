#pragma once
#include "type.h"


// 最基础的增益是1/256
struct STC_CFG_IIR
{
    bool b2048;             // true:基础增益*1/2,g_receip在1024~2048区间使用  false:不影响基础增益
    int segNum;             // 00:4段 01:3段 10:2段 11:1段 ,每一段可增加4倍增益
    bool bB3A3X2;           // true:计算时b3与a3乘2  false:计算时b3与a3不乘2
    bool b1024;             // true:基础增益*1/4,g_receip在256~1024区间使用    false:不影响基础增益
    bool bX2Add;            // true:加性倍数2倍 false:加性倍数0倍
    bool bX1Add;            // true:加性倍数1倍 false:加性倍数0倍
    bool bX05Add;           // true:加性倍数0.5倍 false:加性倍数0倍
    bool bX025Add;          // true:加性倍数0.25倍 false:加性倍数0倍
    int Xbase;              // 必须的基础增益 1 or 2 or 4
};


class CIir_base
{
public:
    int setPara(int bank_id, short*paraArray);  // paraArray格式：[b11-b15 a12-a15 CFG_IIR, b21-b25 a22-a25 CFG_IIR...],一级10个数
    int iirFilterFrame(int bank, int* dataBuf, int dataLen);

private:
    static const int BANK_NUM = 4;                              // 支持多组系数和数据BANK
    static const int ORDER_ADD1 = 5;                            // 滤波器阶数+1
    static const int LEVEL = 4;                                 // 一组滤波器包含的级数
    double a_bank[BANK_NUM][LEVEL * ORDER_ADD1] = { 0 };        // para a
    double b_bank[BANK_NUM][LEVEL * ORDER_ADD1] = { 0 };        // para b
    double gain_bank[BANK_NUM] = { 0 };                         // 输出前统一调整增益
    double xBuf_bank[BANK_NUM][LEVEL * ORDER_ADD1] = { 0 };        // xdata buffer
    double yBuf_bank[BANK_NUM][LEVEL * ORDER_ADD1] = { 0 };        // ydata buffer

    int MultiHLS(int a24, int b16);
    int multi32_8(int a24, int b16);
    int iirFilterLevels(int* dataBuf, int dataLen, int bank_id, int levelNum);                                        // 多级iir
    int iirFilter1Level(double* a, double* b, double* xBufEx, double* yBufEx, int* dataBuf, int dataLen);     // 1级iir

    void calGain(WORD CFG_IIR, double& gain, bool& bB3A3X2);
    void convert(WORD CFG_IIR, STC_CFG_IIR& stcCfg);
    double convertPara_hard2normal(short input, bool type);
};

extern CIir_base iir1;
