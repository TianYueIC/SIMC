#include "FMT_F.h"
#include "mac.h"
#include "memory.h"
#include <math.h>

////////////////////////////////////////////////////////
//  名称:
//      Get_Real
//  功能:
//      提取实部（系统默认高位为实部）,addr_in,addr_out可以是同一地址;
//  参数:
//      1.RA0:输入序列指针，格式[Re | Im]
//      2.RA1:输出序列指针，格式[Re(n+1) | Re(n)](out)
//      3.RD0:序列长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Get_Real
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);	//奇数行
		RD0 >>= 16;							//取高位
		RD1 = RD0 & 0xFFFF;

		i++;
		RD0 = GET_M(RA0 + i * MMU_BASE);	//偶数行
		RD0 &= 0xFFFF0000;

		RD0 += RD1;
		SET_M(RA1 + (i-1)/2 * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      Get_Imag
//  功能:
//      提取虚部（系统默认低位为虚部）,addr_in,addr_out可以是同一地址;
//  参数:
//      1.RA0:输入序列指针，格式[Re | Im]
//      2.RA1:输出序列指针，格式[Im(n+1) | Im(n)](out)
//      3.RD0:序列长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Get_Imag
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);	//奇数行						
		RD1 = RD0 & 0xFFFF;					//取低位

		i++;
		RD0 = GET_M(RA0 + i * MMU_BASE);	//偶数行
		RD0 <<= 16;

		RD0 += RD1;
		SET_M(RA1 + (i - 1) / 2 * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  名称:
//      Real_To_Complex2
//  功能:
//      紧凑16bit格式转换为复数格式,虚部置零;
//  参数:
//      1.RA0:输入序列指针，格式[Re(n+1) | Re(n)]
//      2.RA1:输出序列指针，格式[Re | 0](out)
//      3.RD0:序列长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Real_To_Complex2
{
	int len = RD0.m_data;
	int	x[1024];

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);				
		RD1 = RD0 << 16;					//取低位
		x[2 * i] = RD1.m_data;

		RD0 &=0xFFFF0000;					//取高位
		x[2 * i + 1] = RD0.m_data;
	}

	for (int i = 0; i < (2 * len); i++)
	{
		SET_M(RA1 + i * MMU_BASE, x[i]);
	}
	Return_AutoField(0);

}
