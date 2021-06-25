#include "SOC_Common_F.h"

#include <math.h>


////////////////////////////////////////////////////////
//  名称:
//      _Rf_ShiftL_Reg
//  功能:
//      RD0左移RD1次，右补0
//  参数:
//      1.RD0：被移动的数
//      2.RD1：要移的次数
//  返回值:
//      1.RD0: 移位结果
////////////////////////////////////////////////////////
Sub_AutoField _Rf_ShiftL_Reg
{
	CCpu::RF_ShiftX(RD0, RD1.m_data);
	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      _Rf_ShiftR_Reg
//  功能:
//      RD0右移RD1次，左补0
//  参数:
//      1.RD0：被移动的数
//      2.RD1：要移的次数
//  返回值:
//      1.RD0: 移位结果
////////////////////////////////////////////////////////
Sub_AutoField _Rf_ShiftR_Reg
{
	CCpu::RF_ShiftX(RD0, -RD1.m_data);
	Return_AutoField(0);
}

/////////////////////////////////////////////////////////////
//  模块名称: _Ru_Multi;
//  模块功能: 无符号乘法
//    计算[zh,zl]=x*y; x,y,zh,zl均为32比特的整数,zh,zl分别
//    为结果的高部.低部;
//  模块入口:
//    RD0:乘数x;
//    RD1:乘数y;
//  模块出口:
//    RD0:zl-乘积低部;
//    RD1:zh-乘积高部;
//2012/3/20 11:23:40
////////////////////////////////////////////////////////////
Sub_AutoField _Ru_Multi
{
	long long x = RD0.m_data;
	x = x * RD1.m_data;
	RD0 = *(int*)(&x);					//低32bit 
	RD1 = x >> 32;

	Return_AutoField(0);
}


/////////////////////////////////////////////////////////////
//  模块名称: _Rs_Multi;
//  模块功能: 有符号乘法
//    计算[zh,zl]=x*y; x,y,zh,zl均为32比特的整数,zh,zl分别
//    为结果的高部.低部;
//  模块入口:
//    RD0:被乘数x;
//    RD1:乘数y;
//  模块出口:
//    RD0:zl-乘积低部;
//    RD1:zh-乘积高部;
//2012/3/20 11:23:59
////////////////////////////////////////////////////////////
Sub_AutoField _Rs_Multi
{
	int Multi24_0 = RD0.m_data;
	int Multi24_1 = RD1.m_data;

	long long x = Multi24_0;
	x = x * Multi24_1;
	RD0 = *(int*)(&x);					//低32bit 
	RD1 = x >> 32;

	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  函数名称:
//      _Rf_ShiftL_Sequ_32
//  函数功能:
//      32位序列左移RD1次，右补0;
//  输入参数:
//      1. RA0: 输入序列指针,32位有效;
//      2. RD0: 序列长度,DWORD数;
//      3. RD1: 移位次数;
//  输出参数:
//      RA0:移位结果序列指针,32位有效;
//  注意事项:
//       当RD1=64时,耗时32.5us;
////////////////////////////////////////////////////////
Sub_AutoField _Rf_ShiftL_Sequ_32
{
	RD3 = RD0;                                                                  // RD3: 序列长度,DWORD数;
	RD2 = 0x1F;
	RD2 &= RD1;                                                                 // 限制RD2=0-31,一位次数

L_Rf_ShiftL_Sequ_32_1:
	RD0 = M[RA0];                                                               // RD0 = x(n), n = 1 to 64
	RD1 = RD2;
	call_AutoField _Rf_ShiftL_Reg;
	M[RA0++] = RD0;                                                             // x(n) = x(n) << RD1
	RD3 -= 1;
	if (RQ_nZero) goto L_Rf_ShiftL_Sequ_32_1;
	Return_AutoField(0);
}