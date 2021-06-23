#include "alu.h"
#include "memory.h"
#include <math.h>

////////////////////////////////////////////////////////
//  名称:
//      dma_trans_ad
//  功能:
//      从ADC缓冲区读取一帧采样点
//  参数:
//      1.RD0:源指针<--未使用-->
//      2.RD1:目标指针(out)
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField DMA_Trans_AD
{
		Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      Ram_Clr
//  功能:
//      清除指定的GRAM块
//  参数:
//      1.RA1:指定的GRAM块地址(out)
//		2.RA0:借用的地址，必须指向另一个Group
//      3.RD1:Dword长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Ram_Clr
{
	int len = RD1.m_data;
	for (int i = 0; i < len; i++)
	{
		SET_M(RA1 + i * MMU_BASE, 0);

	}
	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  名称:
//      Ram_Set
//  功能:
//      给指定的GRAM块设置固定值(by CPU)
//  参数:
//      1.RA0:指定的GRAM块地址(in&out)
//      2.RD0:操作DWORD个数
//      3.RD1:const值
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Ram_Set
{

	for (int i = 0; i < RD0.m_data; i++)
	{
		SET_M(RA0 + i * MMU_BASE, RD1.m_data);

	}
	Return_AutoField(0);

}




////////////////////////////////////////////////////////
//  名称:
//      Dual_Ser_Add32
//  功能:
//      双序列加法运算，32bit运算
//  参数:
//      1.RA0:输入序列1指针，32bit格式序列(out)
//      2.RA1:输入序列2指针，32bit格式序列
//      3.RD0:序列Dword长度
//  返回值:
//
//  注意事项:
//   
////////////////////////////////////////////////////////
Sub_AutoField Dual_Ser_Add32
{
	int len = RD0.m_data;
	for (int i = 0; i < len; i++)
	{
		RD0 = M[RA0 + i * MMU_BASE];
		RD1 = M[RA1++];
		RD0 += RD1;
		M[RA0 + i * MMU_BASE] = RD0;
	}

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      Dual_Ser_Sub32
//  功能:
//      双序列减法运算，32bit运算
//  参数:
//      1.RA0:输入序列1指针，32bit格式序列
//      2.RA1:输入序列2指针，32bit格式序列
//      3.RD1:输出序列指针，32bit格式序列(out)
//      4.RD0:序列Dword长度
//  返回值:
//      无
//  注意事项:
//      耗时13.0us;
////////////////////////////////////////////////////////
Sub_AutoField Dual_Ser_Sub32
{
	push(RA2);
	RA2 = RD1;

	int len = RD0.m_data;
	for (int i = 0; i < len; i++)
	{
		RD0 = M[RA0++];
		RD1 = M[RA1++];
		RD0 -= RD1;
		M[RA2++] = RD0;
	}

	pop(RA2);
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      Cal_Single_Shift
//  功能:
//      单序列移位运算，配置可选
//  参数:
//      1.RD0:处理位宽+移位处理指令 (例:Op32bit+Rf_SftR1)详见DMA_ALU.h
//      2.RD1:序列长度
//      3.RA0:输入序列指针(out相同)
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Cal_Single_Shift
{
	int a;
	if (RD0 == 0b10010000)		//译码，详见DMA_ALU.h
	a = -8;
	else if (RD0 == 0b10010001)
		a = -4;
	else if (RD0 == 0b10010010)
		a = -2;
	else if (RD0 == 0b10010011)
		a = -1;
	else if (RD0 == 0b10010100)
		a = 8;
	else if (RD0 == 0b10010101)
		a = 4;
	else if (RD0 == 0b10010110)
		a = 2;
	else if (RD0 == 0b10010111)
		a = 1;
	else if (RD0 == 0b00011000)
		a = 18;
	else if (RD0 == 0b00011001)
		a = 14;
	else if (RD0 == 0b00011010)
		a = 12;
	else if (RD0 == 0b00011011)
		a = 11;
	else
		a = 0;

	unsigned int x;
	int z;
	for (int i = 0; i < RD1.m_data; i++)
	{
		if (a > 10)
		{
			int y = a - 10;
			RD0 = M[RA0 + i * MMU_BASE];
			z = RD0.m_data;
			z = z >> y;
			M[RA0 + i * MMU_BASE] = x;
		}
		else if (a >= 0)
		{
			RD0 = GET_M(RA0 + i * MMU_BASE);
			x = RD0.m_data;
			x = x << a;
			SET_M(RA0 + i * MMU_BASE, x);
		}
		else
		{
			int y = -a;
			RD0 = GET_M(RA0 + i * MMU_BASE);
			x = RD0.m_data;
			x = x >> y;
			SET_M(RA0 + i * MMU_BASE, x);
		}
	}
	Return_AutoField(0);


}

////////////////////////////////////////////////////////
//  名称:
//      Cal_Single_ShiftR1
//  功能:
//      单序列逻辑右移1位运算，32bit运算
//  参数:
//      1.RD0:输入序列指针，32bit格式序列(out)
//      2.RD1:输入序列Dword长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Cal_Single_ShiftR1
{

	RA0 = RD0;
	for (int i = 0; i < RD1.m_data; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RF_ShiftR1(RD0);
		SET_M(RA0 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      Add_DMA_Wola
//  功能:
//      双序列加法运算，32bit运算，128DWORD
//  参数:
//      1.RA0:输入序列1指针，32bit格式序列(out)
//      2.RA1:输入序列2指针，32bit格式序列
//  返回值:
//      1.RD0:无
////////////////////////////////////////////////////////
Sub_AutoField Add_DMA_Wola
{
	for (int i = 0; i < 128; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		RD0 += RD1;
		SET_M(RA0 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  名称:
//      Add_DMA_Wola2
//  功能:
//      双序列加法运算，32bit运算，DWORD数由参数指定
//  参数:
//      1.RA0:输入序列1指针，32bit格式序列(out)
//      2.RA1:输入序列2指针，32bit格式序列
//      3.RD0:运算的DWORD数
//  返回值:
//      1.RD0:无
////////////////////////////////////////////////////////
Sub_AutoField Add_DMA_Wola2
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		RD0 += RD1;
		SET_M(RA0 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  名称:
//      Add_LMT
//  功能:
//      双序列加法运算（限幅至16bit），16bit运算
//      写入两个32位数，DATA0和DATA1，DATA0高16位是数据a，低16位是数据b，DATA1高16位是数据c，低16位是数据d
//      数据a + 数据c，限幅后做为输出的高16位，数据b + 数据d，限幅后做为输出的低16位
//  参数:
//      1.RA0:输入序列1指针，32bit格式序列
//      2.RA1:输入序列2指针，32bit格式序列
//      3.RD1:输出序列指针，32bit格式序列(out)
//      4.RD0:序列Dword长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Add_LMT
{
	push(RA2);
	RA2 = RD1;

	int a, b, c, d;
	int len = RD0.m_data;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		a = RD0.m_data >> 16;
		b = *(short*)(&RD0.m_data);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		c = RD1.m_data >> 16;
		d = *(short*)(&RD1.m_data);
		a += c;
		b += d;
		if (a > 32767)                  //限幅
			a = 32767;
		else if (a < -32767)
			a = -32767;
		if (b > 32767)
			b = 32767;
		else if (b < -32767)
			b = -32767;
		RD0 = (a << 16) + (b & 0xffff);  //int为有符号数，ab相加前b清除高16位
		SET_M(RA2 + i * MMU_BASE, RD0);
	}
	pop(RA2);
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      Sub_LMT
//  功能:
//      双序列减法运算（限幅至16bit），16bit运算
//  参数:
//      1.RA0:输入序列1指针，32bit格式序列(out)
//      2.RA1:输入序列2指针，32bit格式序列
//      3.RD0:序列Dword长度
//  返回值:
//      1.RA0:输出序列指针，32bit格式序列
////////////////////////////////////////////////////////
Sub_AutoField Sub_LMT
{
	int len = RD0.m_data;
	int a, b, c, d;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		a = RD0.m_data >> 16;
		b = *(short*)(&RD0.m_data);
		RD1 = GET_M(RA1 + i * MMU_BASE);
		c = RD1.m_data >> 16;
		d = *(short*)(&RD1.m_data);
		a -= c;
		b -= d;
		if (a > 32767)                  //限幅
			a = 32767;
		else if (a < -32767)
			a = -32767;
		if (b > 32767)
			b = 32767;
		else if (b < -32767)
			b = -32767;
		RD0 = (a << 16) + (b & 0xffff);  //int为有符号数，ab相加前b清除高16位
		SET_M(RA0 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      Cal_Single_Add_Const
//  功能:
//      单序列加常量，高低16bit都有效
//  参数:
//      1.RD1:Const
//      2.RA0:源地址
//      3.RA1:目标地址(out)
//      4.RD0:数据长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Cal_Single_Add_Const
{

	int a, b, c, d;
	c = RD1.m_data >> 16;
	d = *(short*)(&RD1.m_data);
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		a = RD0.m_data >> 16;
		b = *(short*)(&RD0.m_data);
		a += c;
		b += d;
		RD0 = (a << 16) + (b & 0xffff);  //int为有符号数，ab相加前b清除高16位
		SET_M(RA1 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      Cal_Single_Add_Const32
//  功能:
//      单序列加常量，一个数据为32bit
//  参数:
//      1.RD1:Const
//      2.RA0:源地址
//      3.RA1:目标地址(out)
//      4.RD0:数据长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Cal_Single_Add_Const32
{

	int len = RD0.m_data;
	RD2 = RD1;

	for (int i = 0; i < len; i++)
	{
		RD0 = M[RA0++];
		RD0 += RD2;
		M[RA1++] = RD0;
	}
	Return_AutoField(0);

}



////////////////////////////////////////////////////////
//  名称:
//      LMT32To16
//  功能:
//      32bit序列转16bit序列
//  参数:
//      1.RA0:输入序列指针，32bit格式序列
//      2.RA1:输出序列指针，16bit紧凑格式序列(out)
//      3.RD0:输出序列Dword长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField LMT32To16
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + 2 * i * MMU_BASE);
		if (RD0 > 32767)                  //限幅
			RD0 = 32767;
		else if (RD0 < -32767)
			RD0 = -32767;

		RD1 = GET_M(RA0 + (2 * i + 1) * MMU_BASE);
		if (RD1 > 32767)
			RD1 = 32767;
		else if (RD1 < -32767)
			RD1 = -32767;
		RD0 = (RD1 << 16) + (RD0 & 0xffff);  //int为有符号数，相加前清除高16位
		SET_M(RA1 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      DMA_Trans
//  功能:
//      DMA传输数据
//  参数:
//      1.RA0:源地址
//      2.RA1:目标地址(out)
//      3.RD0:数据Dword长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField DMA_Trans
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		SET_M(RA1 + i * MMU_BASE, RD0);
	}
	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  名称:
//      DMA_TransDimin_PATH1
//  功能:
//      通过PATH1的DMA传输数据，目标地址为递减模式（倒序）
//  参数:
//      1.RA0:源指针
//      2.RA1:目标指针(out)
//      3.RD0:Dword长度
//  返回值:
//      无
/////////////////////////////////////////////////////
Sub_AutoField DMA_TransDimin_PATH1
{
	int len = RD0.m_data;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		SET_M(RA1 + (0 - i) * MMU_BASE, RD0);

	}
	Return_AutoField(0);

}

