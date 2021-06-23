#include "STA_F.h"
#include "memory.h"
#include <math.h>

////////////////////////////////////////////////////////
//  名称:
//      FindMaxMin
//  功能:
//      求序列极值（STA1）
//  参数:
//      1.RD0:数据地址
//      2.RD1:数据长度
//  返回值:
//      1.RD0:最大值
//      2.RD1:最小值
////////////////////////////////////////////////////////
Sub_AutoField FindMaxMin
{
	RA0 = RD0;
	int len = RD1.m_data;

	int max, min;
	RD0 = GET_M(RA0);
	max = *(short*)(&RD0.m_data);					
	min = *(short*)(&RD0.m_data);

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = *(short*)(&RD0.m_data);
		if (max < RD1.m_data)
			max = RD1.m_data;
		if (min > RD1.m_data)
			min = RD1.m_data;
		RD1 = RD0 >> 16;
		if (max < RD1.m_data)
			max = RD1.m_data;
		if (min > RD1.m_data)
			min = RD1.m_data;

	}
	max &= 0xffff;
	min &= 0xffff;
	RD0 = max;
	RD1 = min;
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      AbsSum
//  功能:
//      求序列的绝对值累加和（STA1）
//  参数:
//      1.RD0:数据地址
//      2.RD1:数据长度
//  返回值:
//      1.RD0:绝对值累加和
////////////////////////////////////////////////////////
Sub_AutoField AbsSum
{
	RA0 = RD0;	
	int len = RD1.m_data;

	int sum = 0;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = RD0 >> 16;
		sum = abs(RD1.m_data) + sum;
		RD1 = *(short*)(&RD0.m_data);
		sum = abs(RD1.m_data) + sum;
	}
	RD0 = sum;

	pop(RA0);
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      FindMaxIndex
//  功能:
//      求序列绝对值的最大值的Index（STA2）,index以DWORD为单位,0..L-1;
//  参数:
//      1.RD0:数据地址,数据在H16
//      2.RD1:数据长度
//  返回值:
//      1.RD0:最大值的Index, 0 to L-1;
////////////////////////////////////////////////////////
Sub_AutoField FindMaxIndex
{
	RA0 = RD0;
	int len = RD1.m_data;

	RD0 = GET_M(RA0);
	RD0 = RD0 > 16;
	int max = abs(RD0.m_data);
	int n = 0;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD0 = RD0 > 16;
		RD0 = abs(RD0.m_data);
		if (max < RD0.m_data)
		{
			max = RD0.m_data;
			n = i;
		}
	}
	RD0 = n;
	Return_AutoField(0);

}