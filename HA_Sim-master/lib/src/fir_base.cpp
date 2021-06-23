#include "fir_base.h"
#include <math.h>
#include "memory.h"
#include <string.h>
#include <stdio.h>


CFir_base fir1;



////////////////////////////////////////////////////////
//  名称:
//      Multi24_16x24
//  功能:
//      乘法器，通过c语言模拟硬件24位乘法器特性。凡涉及到MAC硬件的乘法都需使用本函数替代。
//  参数:
//      1.Multi24_0:乘数，对应MAC硬件内的RA0序列，16bit格式
//      2.Multi24_1:乘数，对应MAC硬件内的RA1序列，24bit格式
//  返回值值:
//      Multi24_0
////////////////////////////////////////////////////////
int CFir_base::Multi24_16x24(int Multi24_0, int Multi24_1)
{
	long long x;
	x = Multi24_1;
	if (Multi24_1 & 0x8000000)
	{
		x += 1;
	}
	if (Multi24_0 == 0xffff8000)
	{
		Multi24_0 += 1;
	}
	x = x * Multi24_0;
	x = x >> 8;
	Multi24_0 = *(int*)(&x);					//低32bit 
	return Multi24_0;

}


// 将硬件格式的系数转化为内部计算所用的系数a,并存入相应的bank里
// bank_id:使用哪一组bank参与运算,取值范围[0, BANK_NUM)
void CFir_base::setPara(int bank_id, short* paraArray)
{
	for (int i = 0; i < LEVEL; i++)
	{
		short a_hard = paraArray[i];
		double a_val = convertPara_hard2normal(a_hard);
		a_bank[bank_id][i] = a_val;

	}
}

//fir核心计算
void CFir_base::FirFilterFrame(int bank, int* dataBuf, int dataLen)
{
	for (int i = 0; i < dataLen; i++)
	{
		long long s = 0;//乘累加值
		for (int j = 0; j < LEVEL; j++)
		{
			xBuf_bank[bank][LEVEL - j - 1] = xBuf_bank[bank][LEVEL - j - 2];
		}
		xBuf_bank[bank][0] = dataBuf[i];
		//printf("0x%x\n", dataBuf[i]);
		for (int j = 0; j < LEVEL; j++)
		{
			s += Multi24_16x24(a_bank[bank][j], xBuf_bank[bank][j]);
			//int a = s >> 15;
			//printf("0x%x\n", a);
		}
		//printf("0x11223344\n");

		s = s >> 7;
		//限幅
		if (s > 8388607)
			s = 8388607;
		else if (s < -8388608)
			s = -8388608;
		s = s & 0xffffff;
		dataBuf[i] = s;

	}
}
//  硬件格式转为普通数据值
//  硬件格式：{符号位，绝对值}
double CFir_base::convertPara_hard2normal(short input)
{
	bool sign = input >= 0 ? true : false;		// false:负 true:非负
	short value = input & 0x7fff;


	if (!sign)
	{
		value = -value;
	}


	return value;
}





