#include "mac.h"
#include "memory.h"
#include <math.h>
#include "global.h"

//模拟器内部函数，无需用类硬件写法
extern void Multi24(int Multi24_0, int Multi24_1);
extern void Mac_Sim(int addr_0, int addr_1, int Const_Reg, unsigned int Config_Reg, int addr_out, int len);
extern void Multi24_16x24(int Multi24_0, int Multi24_1);
extern void Mac_Sim32(int addr_0, int addr_1, int Const_Reg, unsigned int Config_Reg, int addr_out, int len);
extern void ComplexMulti(int addr_0, int addr_1, unsigned int Config_Reg, int addr_out, int len);
extern int limit(long long x);
extern int limit16(int x);

//本函数库本质是给定不同参数调取Mac硬件，模拟器中实际表现为调取Mac_Sim函数
//本函数库分两批写成，前面约一半为各自完成功能，后一半使用了Mac_Sim模拟算法

////////////////////////////////////////////////////////
//  名称:
//      Multi24
//  功能:
//      乘法器，通过c语言模拟硬件24位乘法器特性。凡涉及到MAC硬件的乘法都需使用本函数替代。
//  参数:
//      1.Multi24_0:乘数，对应MAC硬件内的RA0序列，16bit格式
//      2.Multi24_1:乘数，对应MAC硬件内的RA1序列，16bit格式进来，计算时扩充为24bit计算
//      3.RD0:乘法结果，32bit格式
//  返回值值:
//      无
////////////////////////////////////////////////////////
void Multi24(int Multi24_0, int Multi24_1)
{
	long long x;
	x = Multi24_1;
	x = x * 256;
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
	RD0 = *(int*)(&x);					//低32bit 


}



////////////////////////////////////////////////////////
//  名称:
//      SingleSerSquare
//  功能:
//      单序列平方运算
//  参数:
//      1.RA0:输入序列指针，紧凑16bit格式，高低位都有数
//      2.RA1:输出序列指针，紧凑16bit格式(out)
//      3.RA0:Dword长度
//  返回值值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField SingleSerSquare
{
	push(RD2);

	int len = RD0.m_data;
	int x;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0.m_data + i * MMU_BASE);
		RD2 = RD0;
		x = RD0.m_data;
		x = *(short*)(&x);					//低16bit 
		Multi24(x, x);
		RD1 = RD0;
		RD1 = RD1 >> 15;

		x = RD2.m_data;
		x = x >> 16;								//高16bit
		Multi24(x, x);
		RD0 = RD0 >> 15;

		RD0 = (RD0 << 16);
		RD0 += RD1;
		SET_M(RA1 + i * MMU_BASE, RD0);
	}

	pop(RD2);
	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  名称:
//      MultiSum
//  功能:
//      双序列乘累加运算
//  参数:
//      RA0:输入序列1指针，紧凑16bit格式序列
//      RA1:输入序列2指针，紧凑16bit格式序列
//      RD1:输出序列指针，紧凑16bit格式序列(out),此为算法验证中间数，有需要时打开注释
//      RD0:序列长度
//  返回值:
//      RD0:乘累加结果
////////////////////////////////////////////////////////
Sub_AutoField MultiSum
{
	push(RA2);
	int len = RD0.m_data;

	RD0 = RD1;
	RA2 = RD0;

	RD0 = GET_M(RA0);   //首数
	int a = *(short*)(&RD0.m_data);		//a为序列1的低位  
	int b = RD0 >> 16;				//b为序列1的高位
	RD1 = GET_M(RA1);
	int c = *(short*)(&RD1.m_data);		//c为序列2的低位  
	int d = RD1 >> 16;				//d为序列2的高位
	Multi24(a, c);
	int j = RD0.m_data;					//第一轮低位乘积(保留32位)
	RD0 = RD0 >> 15;
	RD1 = RD0;						//低位乘积（低16bit有效）
	Multi24(b, d);
	int k = RD0.m_data;					//第一轮高位乘积(保留32位)
	RD0 = RD0 >> 15;					//高位乘积（低16bit有效）
	RD0 = (RD0 << 16);
	RD0 += RD1;
	//SET_M(RA2, RD0);

	for (int i = 1; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		int a = *(short*)(&RD0);		//a为序列1的低位  
		int b = RD0 >> 16;				//b为序列1的高位
		RD1 = GET_M(RA1 + i * MMU_BASE);
		int c = *(short*)(&RD1);		//c为序列2的低位  
		int d = RD1 >> 16;				//d为序列2的高位

		Multi24(a, c);
		j += RD0.m_data;						//本轮低位乘累加和（32bit）
		RD0 = j >> 15;
		RD0 = RD0 & 0XFFFF;				//int为32bit有符号数，相加前清除高16位
		RD1 = RD0;						//本轮低位乘累加和（16bit有效）
		Multi24(b, d);					//本轮低位乘累加和（32bit）
		k += RD0.m_data;
		RD0 = k >> 15;					//本轮低位乘累加和（低16bit有效）
		RD0 = (RD0 << 16);
		RD0 += RD1;
		//SET_M(RA2 + i * MMU_BASE, RD0);

	}
	//此时RD0的值就是最后乘累加的值

	pop(RA2);
	Return_AutoField(0);

}
////////////////////////////////////////////////////////
//  名称:
//      MAC_MultiConst16
//  功能:
//      为单序列乘常量操作配置DMA_Ctrl参数, Z(n)=X[n]*CONST/32767
//  参数:
//      1.M[RSP+3*MMU_BASE]：X(n) 首地址（字节地址）
//      2.M[RSP+2*MMU_BASE]：Const 注意要求高16位与低16位相同
//      3.M[RSP+1*MMU_BASE]：Z(n) 首地址
//      4.M[RSP+0*MMU_BASE]：数据长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField MAC_MultiConst16
{
	RA0 = GET_M(RSP + 3 * MMU_BASE);
	RA1 = GET_M(RSP + 1 * MMU_BASE);
	RD0 = GET_M(RSP + 2 * MMU_BASE);
	RD1 = GET_M(RSP + 0 * MMU_BASE);

	Mac_Sim(RA0.m_data, RA0.m_data, RD0.m_data, 0x0C, RA1.m_data, RD1.m_data);

	Return_AutoField(4 * MMU_BASE);
}

////////////////////////////////////////////////////////
//  名称:
//      MultiConstH16L16
//  功能:
//      序列与Const相乘运算,Q15输出;H16L16都有数
//  参数:
//      RA0:输入序列1指针，紧凑16bit格式序列
//      RA1:输出序列指针，紧凑16bit格式序列(out)
//      RD1:Const值
//      RD0:序列长度
//  返回值:
//      none
////////////////////////////////////////////////////////
Sub_AutoField MultiConstH16L16
{

	Mac_Sim(RA0.m_data, RA0.m_data, RD1.m_data, 0x0C, RA1.m_data, RD0.m_data);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  函数名称:
//      MultiVector2
//  函数功能:
//      双矢量乘法，Q7输出
//  入口参数:
//      RA0:*in1
//      RA1:*in2
//      RD1:*out
//      RD0:序列长度
//  出口参数:
//      无
////////////////////////////////////////////////////////
Sub_AutoField MultiVector2
{
	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x81, RD1.m_data, RD0.m_data);

	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  函数名称:
//      Mac_Sim
//  函数功能:
//      模拟Mac硬件数据路径
//  参数:
//      addr_0:data0地址
//      addr_1:data1地址
//      addr_out:out地址
//		Const_Reg：常数值
//		Config_Reg：配置参数，详情见MAC数据路径示意图
//      len:序列长度
//  注意事项:
//      本函数用来模拟MAC流程，并不全面。如需使用乘累加器，请使用MultiSum函数。
////////////////////////////////////////////////////////
void Mac_Sim(int addr_0, int addr_1, int Const_Reg, unsigned int Config_Reg, int addr_out, int len)
{
	push(RA0);
	push(RA1);
	push(RA2);

	RD0 = addr_0;
	RA0 = RD0;
	RD0 = addr_1;
	RA1 = RD0;
	RD0 = addr_out;
	RA2 = RD0;

	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
		int a = *(short*)(&RD0);		//a为序列的低位  
		int b = RD0 >> 16;				//b为序列的高位
		int c, d;

		//提取输入选择参数
		RD1 = (Config_Reg & 0x1c) >> 2;
		if (RD1 == 0)
		{
			RD0 = GET_M(RA1 + i * MMU_BASE);
			c = *(short*)(&RD0);		//c为序列的低位  
			d = RD0 >> 16;				//d为序列的高位
			c = c << 8;					//24bit进乘法器
			d = d << 8;
		}
		else if (RD1 == 1)
		{
			c = a;						//c为序列的低位,求平方时使用，DATA0=DATA1
			d = b;						//d为序列的高位
			c = c << 8;					//24bit进乘法器
			d = d << 8;
		}
		else if (RD1 == 2)
		{
			RD0 = GET_M(RA1 + i * MMU_BASE);
			c = RD0 >> 16;				//c为序列的低位，实部调制时使用，低位=高位  
			d = RD0 >> 16;				//d为序列的高位
			c = c << 8;					//24bit进乘法器
			d = d << 8;
		}
		else if (RD1 == 3)
		{
			RD0 = Const_Reg;
			c = *(short*)(&RD0);		//c为序列的低位，乘常数时使用
			d = RD0 >> 16;				//d为序列的高位
			c = c << 8;					//24bit进乘法器
			d = d << 8;
		}
		else if (RD1 == 7)
		{
			RD0 = Const_Reg;
			d = Const_Reg & 0xffffff;	//常数的低24位，乘常数时使用
			c = 0;						//此时M0关闭工作，c给任意数皆可
		}
		else return;

		//模拟乘法器特性
		Multi24_16x24(a, c);
		int M0 = RD0.m_data;
		Multi24_16x24(b, d);
		int M1 = RD0.m_data;

		//提取输出选择参数
		RD1 = Config_Reg & 0x3;
		if (RD1 == 0)
		{
			M0 = M0 >> 15;
			M0 = M0 & 0xffff;		//低位
			M1 = M1 >> 15;
			M1 = M1 << 16;
			RD0 = M1 + M0;
			SET_M(RA2 + i * MMU_BASE, RD0);
		}
		else if (RD1 == 1)
		{
			M0 = M0 >> 7;
			M1 = M1 >> 7;
			if (Config_Reg & 0x80)
			{	//限幅器打开
				if (M0 > 32767)                  //限幅
					M0 = 32767;
				else if (M0 < -32767)
					M0 = -32767;
				if (M1 > 32767)
					M1 = 32767;
				else if (M1 < -32767)
					M1 = -32767;
			}
			M0 = M0 & 0xffff;		//低位
			M1 = M1 << 16;
			RD0 = M1 + M0;
			SET_M(RA2 + i * MMU_BASE, RD0);
		}
		else if (RD1 == 2)
		{
			RD0 = M1 << 1;
			SET_M(RA2 + i * MMU_BASE, RD0);
		}
		else return;

	}

	pop(RA2);
	pop(RA1);
	pop(RA0);

}

////////////////////////////////////////////////////////
//  名称:
//      Multi24_16x24
//  功能:
//      乘法器，通过c语言模拟硬件24位乘法器特性。凡涉及到MAC硬件的乘法都需使用本函数替代。
//  参数:
//      1.Multi24_0:乘数，对应MAC硬件内的RA0序列，16bit格式
//      2.Multi24_1:乘数，对应MAC硬件内的RA1序列，24bit格式
//      3.RD0:乘法结果，32bit格式
//  返回值值:
//      无
////////////////////////////////////////////////////////
void Multi24_16x24(int Multi24_0, int Multi24_1)
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
	RD0 = *(int*)(&x);					//低32bit 


}


////////////////////////////////////////////////////////
//  函数名称:
//      ModulationToZero
//  函数功能:
//      MAC指令为0x08, Q15输出
//		表示（1）实部调制(M1-IN0,M0-IN1)调制,计算y(n).H16*x(n).H16 以及 y(n).H16*x(n).L16,
//      （2）Q0方式输出,即Q0.H16 = DATA1.H16*DATA0.H16（M1乘法器）, Q0.L16 = DATA1.H16*DATA0.L16（M0乘法器）,
//      参见MAC数据路径示意图（2）文档。
//  输入参数:
//      1.RA0: x(n), [H16|L16]格式的32位操作数序列;
//      2.RA1: y(n), [H16|0]格式的高16位操作数序列;
//      3.RD1: z(n) = x(n).*y(n)的目标地址;
//      4.RD0: 数据长度
//  输出参数:
//      无
//  注意事项:
//      无
////////////////////////////////////////////////////////
Sub_AutoField ModulationToZero
{
	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x08, RD1.m_data, RD0.m_data);

	Return_AutoField(0);

}
////////////////////////////////////////////////////////
//  名称:
//      DualSequMulti_Q0
//  功能:
//      双序列乘,Q0输出;MAC指令为0x00
//  参数:
//      1.RA0:输入序列1指针,紧凑16bit格式
//      2.RA1:输入序列2指针,紧凑16bit格式
//      3.RD0:Dword长度
//  返回值值:
//      1.RD1:输出序列指针,紧凑16bit格式(out)
//  注意事项:
//      实虚部平方只能采用Q0方式输出,不可以Q1带限幅输出;
////////////////////////////////////////////////////////
Sub_AutoField DualSequMulti_Q0
{
	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x00, RD1.m_data, RD0.m_data);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      MultiConst24_DivQ7_LMT
//  功能:
//      序列与Const相乘运算，MAC指令为0x9d
//  参数:
//      RA0:输入序列1指针,高16bit格式序列
//      RA1:输出序列指针,结果为: QM1L[22:7],QM0L[22:7]
//      RD1:Const值,低8位为0
//      RD0:序列长度
//  返回值:
//      无
//  注意事项:
//      耗时12.2us
////////////////////////////////////////////////////////
Sub_AutoField MultiConst24_DivQ7_LMT
{
	Mac_Sim(RA0.m_data, RA0.m_data, RD1.m_data, 0x9d, RA1.m_data, RD0.m_data);

	Return_AutoField(0);


}

////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_DivQ7
//  函数功能:
//      双序列乘运算，MAC指令为0x81
//  输入参数:
//      RA0:输入序列1指针,高16bit格式序列
//      RA1:输入序列2指针,高16bit格式序列
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为；QM1L[22:7],QM0L[22:7]
//  注意事项:
//      耗时12.7us;
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_DivQ7
{
	int len = RD0.m_data;
	for (int i = 0; i < len; i++)
	{
		RD0 = GET_M(RA0.m_data + i * MMU_BASE);
		RD0 = RD0 & 0xffff0000;
		SET_M(RA0.m_data + i * MMU_BASE, RD0);
		RD0 = GET_M(RA1.m_data + i * MMU_BASE);
		RD0 = RD0 & 0xffff0000;
		SET_M(RA1.m_data + i * MMU_BASE, RD0);
	}

	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x81, RD1.m_data, len);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      ModulationToZero2
//  功能:
//      调制
//		MAC指令为0x09, Q7输出
//  参数:
//      1.RA0:表地址
//      2.RA1:操作数地址
//      3.RD1:目标地址
//      4.RD0:数据长度对应的TimerNum值(Dword长度*3+3)
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField ModulationToZero2
{
	Mac_Sim(RA0.m_data, RA1.m_data, 0, 0x09, RD1.m_data, RD0.m_data);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      SingleSerSquareL
//  功能:
//      单序列平方运算
//  参数:
//      1.RA0:输入序列指针,紧凑16bit格式
//      2.RA1:输出序列指针,紧凑16bit格式(out)
//      3.RD0:序列长度
//  返回值值:
//      无
//  注意事项:
//      实虚部平方只能采用Q0方式输出,不可以Q1带限幅输出;
////////////////////////////////////////////////////////
Sub_AutoField SingleSerSquareL
{
	Mac_Sim(RA0.m_data, RA0.m_data, 0, 0x00, RA1.m_data, RD0.m_data);

	Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      SingleSerSquare32
//  功能:
//      单序列平方运算，输出结果为输入序列高16位的平方，保留32位结果
//  参数:
//      1.RA0:输入序列指针,紧凑16bit格式
//      2.RA1:输出序列指针,紧凑16bit格式(out)
//      3.RD0:序列长度
//  返回值值:
//      无
//  注意事项:
//      舍弃低16位平方结果
////////////////////////////////////////////////////////
Sub_AutoField SingleSerSquare32
{
	Mac_Sim(RA0.m_data, RA0.m_data, 0, 0x46, RA1.m_data, RD0.m_data);

	Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  名称:
//      MAC_MultiConst16_Q2207
//  功能:
//      为单序列乘常量操作配置DMA_Ctrl参数
//  参数:
//      1.M[RSP+3*MMU_BASE]：X(n) 首地址（字节地址）
//      2.M[RSP+2*MMU_BASE]：Const 注意要求高16位与低16位相同
//      3.M[RSP+1*MMU_BASE]：Z(n) 首地址
//      4.M[RSP+0*MMU_BASE]：数据长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField MAC_MultiConst16_Q2207
{
	RA0 = GET_M(RSP + 3 * MMU_BASE);
	RA1 = GET_M(RSP + 1 * MMU_BASE);
	RD0 = GET_M(RSP + 2 * MMU_BASE);
	RD1 = GET_M(RSP + 0 * MMU_BASE);

	Mac_Sim(RA0.m_data, RA0.m_data, RD0.m_data, 0x8D, RA1.m_data, RD1.m_data);

	Return_AutoField(4 * MMU_BASE);

}


////////////////////////////////////////////////////////
//  名称:
//      FIR_MAC
//  功能:
//      利用MAC乘累加器进行FIR计算
//  参数:
//      1.RA0:输入序列指针，16bit紧凑格式序列
//      2.RA1:输出序列指针，16bit紧凑格式序列
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField FIR_MAC
{
	push(RA2);
	push(RD2);
	push(RD3);
	push(RD4);

	#define FIRCache             (RN_GRAM0+100*MMU_BASE)

	RD0 = RA1;
	RA2 = RD0;
	RD0 = FIRCache;
	RA1 = RD0;


	for (int i = 0; i < 16; i++)//每帧16
	{
		for (int j = 0; j < 15; j++)
		{
			RD0 = M[RA1 + (14 - j) * MMU_BASE];
			M[RA1 + (15 - j) * MMU_BASE] = RD0;
		}
		RD0 = M[RA0 + i * MMU_BASE];
		RD0 = *(short*)(&RD0);
		M[RA1] = RD0;

		RD0 = RA0;
		RD2 = RD0;
		RD0 = RA1;
		RD3 = RD0;		//暂存地址数据

		RD0 = RN_GRAM2;//系数地址
		RA0 = RD0;
		RD0 = FIRCache;
		RA1 = RD0;
		RD0 = 16;
		call_AutoField MultiSum;//乘累加，RD0为计算结果
		RD0 &= 0xffff;
		RD4 = RD0;

		RD0 = RD2;
		RA0 = RD0;
		RD0 = RD3;
		RA1 = RD0;		//释放地址数据

		for (int j = 0; j < 15; j++)
		{
			RD0 = M[RA1 + (14 - j) * MMU_BASE];
			M[RA1 + (15 - j) * MMU_BASE] = RD0;
		}
		RD0 = M[RA0 + i * MMU_BASE];
		RD0 = RD0 >> 16;
		M[RA1] = RD0;

		RD0 = RA0;
		RD2 = RD0;
		RD0 = RA1;
		RD3 = RD0;		//暂存地址数据

		RD0 = RN_GRAM2;//系数地址
		RA0 = RD0;
		RD0 = FIRCache;
		RA1 = RD0;
		RD0 = 16;
		call_AutoField MultiSum;//乘累加，RD0为计算结果
		RD0 = RD0 << 16;
		RD0 += RD4;
		M[RA2 + i * MMU_BASE] = RD0;

		RD0 = RD2;
		RA0 = RD0;
		RD0 = RD3;
		RA1 = RD0;		//释放地址数据




	}

	pop(RD4);
	pop(RD3);
	pop(RD2);
	pop(RA2);

	Return_AutoField(0);

}



////////////////////////////////////////////////////////
//  名称:
//      FIR_DATA
//  功能:
//      设置fir滤波器系数,便于FIR_MAC函数使用
//  参数:
//      无
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField FIR_DATA
{
	RA0 = RN_GRAM8;

// fir系数
#define a0          102 
#define a1          -177
#define a2          396
#define a3          -800
#define a4          1439
#define a5          -2418
#define a6          4011
#define a7          -7307
#define a8          22993
#define a9          18268  
#define a10         -5364  
#define a11         2471  
#define a12         -1193  
#define a13         541  
#define a14         -229  
#define a15         0  

//以下填入滤波器系数
RD0 = a0;
M[RA0++] = RD0;
RD0 = a1;
M[RA0++] = RD0;
RD0 = a2;
M[RA0++] = RD0;
RD0 = a3;
M[RA0++] = RD0;
RD0 = a4;
M[RA0++] = RD0;
RD0 = a5;
M[RA0++] = RD0;
RD0 = a6;
M[RA0++] = RD0;
RD0 = a7;
M[RA0++] = RD0;
RD0 = a8;
M[RA0++] = RD0;
RD0 = a9;
M[RA0++] = RD0;
RD0 = a10;
M[RA0++] = RD0;
RD0 = a11;
M[RA0++] = RD0;
RD0 = a12;
M[RA0++] = RD0;
RD0 = a13;
M[RA0++] = RD0;
RD0 = a14;
M[RA0++] = RD0;
RD0 = a15;
M[RA0++] = RD0;


Return_AutoField(0 * MMU_BASE);
}


////////////////////////////////////////////////////////
//  名称:
//      ComplexMulti 
//  功能:
//      双序列复数乘法运算底层函数
//  参数:
//      1.addr_0：输入序列0地址，紧凑16bit格式
//      2.addr_1：输入序列1地址，紧凑16bit格式
//      3.addr_out：输出序列地址，紧凑16bit格式
//      4.len：数据长度
//		5.Config_Reg:输出格式指令，2bit，00：低16bit，01：中16bit，10：高16bit
//  返回值值:
//      无
//  注意事项:
//		无;
////////////////////////////////////////////////////////
void ComplexMulti(int addr_0, int addr_1, unsigned int Config_Reg, int addr_out, int len)
{
	push(RA2);
	RA2 = addr_out;
	RA0 = addr_0;
	RA1 = addr_1;
	int A, B, C, D, AC, BD, AD, BC, H, L;
	long long x, y, z;
	y = 0x7fffffff;			//限幅数据
	z = 0xffffffff80000000;

	for (int i = 0; i < len; i++)
	{
		//根据公式：(A+Bi)*(C+Di)=(AC-BD)+(AD+BC)i
		RD0 = M[RA0++];
		A = RD0 >> 16;
		B = *(short*)(&RD0);
		RD1 = M[RA1++];
		C = RD1 >> 16;
		D = *(short*)(&RD1);
		if (C == 0xffff8000)//硬件编码需求
			C = 0xffff8001;
		if (D == 0xffff8000)//硬件编码需求
			D = 0xffff8001;
		AC = A * C;
		BD = B * D;
		AD = A * D;
		BC = B * C;

		x = AC;
		x -= BD;
		if (x > y)	//限幅
			x = y;
		else if (x < z)
			x = z;
		H = x;

		x = AD;
		x += BC;
		if (x > y)	//限幅
			x = y;
		else if (x < z)
			x = z;
		L = x;

		if (Config_Reg == 0)
		{
			H = limit16(H);
			L = limit16(L);
			RD0 = (H << 16) + L;
			M[RA2 + i * MMU_BASE] = RD0;
		}
		else if (Config_Reg == 1)
		{
			H = H >> 8;
			L = L >> 8;
			H = limit16(H);
			L = limit16(L);
			RD0 = (H << 16) + L;
			M[RA2 + i * MMU_BASE] = RD0;
		}
		else if (Config_Reg == 2)
		{
			H = 0xFFFF0000 & H;
			L = (L >> 16) & 0xFFFF;
			RD0 = H + L;
			M[RA2 + i * MMU_BASE] = RD0;
		}
		else
		{
			pop(RA2);
			return;
		}
	}

	pop(RA2);

}

////////////////////////////////////////////////////////
//  函数名称:
//      ComplexMulti_Q3116
//  函数功能:
//      双序列复数乘法运算，(A+Bi)*(C+Di)=(AC-BD)+(AD+BC)i
//  输入参数:
//      RA0:输入序列1指针,16bit紧凑格式序列
//      RA1:输入序列2指针,16bit紧凑格式序列
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,16bit紧凑格式序列,结果为32位中的高16位[b31:b16]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField ComplexMulti_Q3116
{
	ComplexMulti(RA0.m_data, RA1.m_data, 0x2, RD1.m_data, RD0.m_data);//高32

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      ComplexMulti_Q2308
//  函数功能:
//      双序列复数乘法运算，(A+Bi)*(C+Di)=(AC-BD)+(AD+BC)i
//  输入参数:
//      RA0:输入序列1指针,16bit紧凑格式序列
//      RA1:输入序列2指针,16bit紧凑格式序列
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,16bit紧凑格式序列,结果为32位中的中16位[b23:b8]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField ComplexMulti_Q2308
{
	ComplexMulti(RA0.m_data, RA1.m_data, 0x1, RD1.m_data, RD0.m_data);//高32

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      ComplexMulti_Q1500
//  函数功能:
//      双序列复数乘法运算，(A+Bi)*(C+Di)=(AC-BD)+(AD+BC)i
//  输入参数:
//      RA0:输入序列1指针,16bit紧凑格式序列
//      RA1:输入序列2指针,16bit紧凑格式序列
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,16bit紧凑格式序列,结果为32位中的低16位[b15:b0]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField ComplexMulti_Q1500
{
	ComplexMulti(RA0.m_data, RA1.m_data, 0x0, RD1.m_data, RD0.m_data);//高32

	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  函数名称:
//      Mac_Sim32
//  函数功能:
//      模拟32位宽Mac硬件数据路径
//  参数:
//      addr_0:data0地址
//      addr_1:data1地址
//      addr_out:out地址
//		Const_Reg：常数值
//		Config_Reg：配置参数
//      len:序列长度
//  注意事项:
//      参数编码(暂定):
//		bit8		是否关掉X*C0+Y*C1形式下的Y*C1乘法,0-不关;1-关掉;
//		bit5,6		表示是否打开X*Y=Z形式下Y的部分取值(即变成32X16的乘法),00-关闭;01-Y取高16bit;11-Y取低16bit
//		bit4		决定功能0:X*C0+Y*C1(32bit双序列乘常量);1:X*Y=Z(32bit乘法)
//		bit0,1,2	决定取值，000/001/010/011/100对应以下结果取值起点，分别为：000-b0;001-b7;010-b15;011-b23;100-b31;
////////////////////////////////////////////////////////
void Mac_Sim32(int addr_0, int addr_1, int Const_Reg, unsigned int Config_Reg, int addr_out, int len)
{
	push(RA0);
	push(RA1);
	push(RA2);

	RD0 = addr_0;
	RA0 = RD0;
	RD0 = addr_1;
	RA1 = RD0;
	RD0 = addr_out;
	RA2 = RD0;

	for (int i = 0; i < len; i++)
	{
		RD0 = M[RA0++];
		long long X = RD0.m_data;
		RD0 = M[RA1++];
		long long Y = RD0.m_data;
		long long Z;
		////提取配置选择参数
		RD0 = (Config_Reg & 0x7);
		RD1 = (Config_Reg & 0x8) >> 3;
		if (RD1 == 0)//X*C0+Y*C1
		{
			int C0 = Const_Reg >> 16;
			int C1 = *(short*)(&Const_Reg);
			X = X * C0;

			int Config_Reg_b8 = (Config_Reg & 0x100) >> 8;
			if (Config_Reg_b8 == 0)
			{
				Y = Y * C1;
			}
			else
			{
				Y = 0;	// 不算第二部分
			}

			X += Y;
			if (RD0 == 0)
			{
				X = limit(X);
			}
			else if (RD0 == 1)
			{
				X = (X >> 7);
				X = limit(X);
			}
			else if (RD0 == 2)
			{
				X = (X >> 15);
				X = limit(X);
			}

			else return;
		}
		else if (RD1 == 1)
		{
			int Config_Reg_b56 = (Config_Reg & 0x30) >> 4;
			if (Config_Reg_b56 == 1)
				Y = Y >> 16;
			else if (Config_Reg_b56 == 3)
				Y = *(short*)(&Y);
			else
				Y &= 0xfffffffffffffffe;

			X = X * Y;
			if (RD0 == 0)
			{
				X = limit(X);
			}
			else if (RD0 == 1)
			{
				X = X >> 7;
				X = limit(X);
			}
			else if (RD0 == 2)
			{
				X = X >> 15;
				X = limit(X);
			}
			else if (RD0 == 3)
			{
				X = X >> 23;
				X = limit(X);
			}
			else if (RD0 == 4)
			{
				X = X >> 31;
				X = limit(X);
			}
			else return;
		}
		else return;

		RD0 = X;
		M[RA2++] = RD0;

	}
	pop(RA2);
	pop(RA1);
	pop(RA0);

}

////////////////////////////////////////////////////////
//  函数名称:
//      MultiConst32_Dual_Q4615
//  函数功能:
//      32bit双序列乘常数再相加运算，X*C0+Y*C1
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,32bit格式序列
//      RD0:序列长度
//      RD1:常数，紧凑16bit格式
//  输出参数:
//      RA2:输出序列指针,结果为48位中的高32位[b46:b15]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Dual_Q4615
{
	Mac_Sim32(RA0.m_data, RA1.m_data, RD1.m_data, 0x2, RA2.m_data, RD0.m_data);//高32
	//Mac_Sim32(RA0.m_data, RA1.m_data, RD1.m_data, 0x1, RA2.m_data, RD0.m_data);//中32
	//Mac_Sim32(RA0.m_data, RA1.m_data, RD1.m_data, 0x0, RA2.m_data, RD0.m_data);//低32

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      MultiConst32_Dual_Q3807
//  函数功能:
//      32bit双序列乘常数再相加运算，X*C0+Y*C1
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,32bit格式序列
//      RD0:序列长度
//      RD1:常数，紧凑16bit格式
//  输出参数:
//      RA2:输出序列指针,结果为48位中的[b38:b7]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Dual_Q3807
{
	Mac_Sim32(RA0.m_data, RA1.m_data, RD1.m_data, 0x1, RA2.m_data, RD0.m_data);//中32

	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  函数名称:
//      MultiConst32_Dual_Q3100
//  函数功能:
//      32bit双序列乘常数再相加运算，X*C0+Y*C1
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,32bit格式序列
//      RD0:序列长度
//      RD1:常数，紧凑16bit格式
//  输出参数:
//      RA2:输出序列指针,结果为48位中的[b31:b0]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Dual_Q3100
{
	Mac_Sim32(RA0.m_data, RA1.m_data, RD1.m_data, 0x0, RA2.m_data, RD0.m_data);//低32

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      MultiConst32_Single_Q4615
//  函数功能:
//      32bit单序列乘常数，X*C0
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RD0:序列长度
//      RD1:常数，高16bit有效
//  输出参数:
//      RA1:输出序列指针,结果为48位中的高32位[b46:b15]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Single_Q4615
{
	Mac_Sim32(RA0.m_data, RA0.m_data, RD1.m_data, 0x102, RA1.m_data, RD0.m_data);//高32

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  函数名称:
//      MultiConst32_Single_Q3807
//  函数功能:
//      32bit单序列乘常数，X*C0
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RD0:序列长度
//      RD1:常数，高16bit有效
//  输出参数:
//      RA1:输出序列指针,结果为48位中的中32位[b38:b07]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Single_Q3807
{
	Mac_Sim32(RA0.m_data, RA0.m_data, RD1.m_data, 0x101, RA1.m_data, RD0.m_data);//中32

	Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  函数名称:
//      MultiConst32_Single_Q3100
//  函数功能:
//      32bit单序列乘常数，X*C0
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RD0:序列长度
//      RD1:常数，高16bit有效
//  输出参数:
//      RA1:输出序列指针,结果为48位中的低32位[b31:b00]
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField MultiConst32_Single_Q3100
{
	Mac_Sim32(RA0.m_data, RA0.m_data, RD1.m_data, 0x100, RA1.m_data, RD0.m_data);//低32

	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_32X32_Q6231
//  函数功能:
//      双32bit序列乘运算
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,32bit格式序列
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为64位中62-31；
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32X32_Q6231
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0xC, RD1.m_data, RD0.m_data);//高32bit

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_32X32_Q5423
//  函数功能:
//      双32bit序列乘运算
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,32bit格式序列
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为64位中54-23；
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32X32_Q5423
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0xB, RD1.m_data, RD0.m_data);//高32bit

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_32X32_Q4615
//  函数功能:
//      双32bit序列乘运算
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,32bit格式序列
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为64位中46-15；
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32X32_Q4615
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0xA, RD1.m_data, RD0.m_data);//高32bit

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_32X32_Q3807
//  函数功能:
//      双32bit序列乘运算
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,32bit格式序列
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为64位中38-7；
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32X32_Q3807
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x9, RD1.m_data, RD0.m_data);//高32bit

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_32X32_Q3100
//  函数功能:
//      双32bit序列乘运算
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,32bit格式序列
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为64位中31-0；
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32X32_Q3100
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x8, RD1.m_data, RD0.m_data);//高32bit

	Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_32X16H_Q4615
//  函数功能:
//      32bit序列乘16bit序列运算
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,16bit格式序列,高16bit有效
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为48位中46-15；
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32X16H_Q4615
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x1A, RD1.m_data, RD0.m_data);//高32bit

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_32X16H_Q3807
//  函数功能:
//      32bit序列乘16bit序列运算
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,16bit格式序列,高16bit有效
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为48位中38-7；
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32X16H_Q3807
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x19, RD1.m_data, RD0.m_data);//高32bit

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_32X16H_Q3100
//  函数功能:
//      32bit序列乘16bit序列运算
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,16bit格式序列,高16bit有效
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为48位中31-0；
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32X16H_Q3100
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x18, RD1.m_data, RD0.m_data);//高32bit

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_32X16L_Q4615
//  函数功能:
//      32bit序列乘16bit序列运算
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,16bit格式序列,低16bit有效
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为48位中46-15；
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32X16L_Q4615
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x3A, RD1.m_data, RD0.m_data);//高32bit

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_32X16L_Q3807
//  函数功能:
//      32bit序列乘16bit序列运算
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,16bit格式序列,低16bit有效
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为48位中38-7；
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32X16L_Q3807
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x39, RD1.m_data, RD0.m_data);//高32bit

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      SeqMulti_32X16L_Q3100
//  函数功能:
//      32bit序列乘16bit序列运算
//  输入参数:
//      RA0:输入序列1指针,32bit格式序列
//      RA1:输入序列2指针,16bit格式序列,低16bit有效
//      RD0:序列长度
//  输出参数:
//      RD1:输出序列指针,结果为48位中31-0；
//  注意事项:
//      
////////////////////////////////////////////////////////
Sub_AutoField SeqMulti_32X16L_Q3100
{
	Mac_Sim32(RA0.m_data, RA1.m_data, 0, 0x38, RD1.m_data, RD0.m_data);//高32bit

	Return_AutoField(0);
}

////////////////////////////////////////////////////////
//  函数名称:
//      limit
//  函数功能:
//      32位限幅，大于0x7fffffff限为0x7fffffff;小于0x80000001限为0x80000001
//  输入参数:
//      x：限幅前数据
//  输出参数:
//      y：限幅后数据；
//  注意事项:
//      
////////////////////////////////////////////////////////
int limit(long long x)
{
	int b63 = (x & 0x8000000000000000) >> 63;
	int y;

	if (b63 == 0)
	{
		if (x & 0xFFFFFFFF80000000)
			y = 0x7fffffff;
		else
			y = x;
	}
	else
	{
		x = ~x;
		if (x & 0xFFFFFFFF80000000)
			y = 0x80000001;
		else
			y = ~x;
	}

	return y;
}

////////////////////////////////////////////////////////
//  函数名称:
//      limit16
//  函数功能:
//      16位限幅，大于0x7fff限为0x7fff;小于0x8001限为0x8001
//  输入参数:
//      x：限幅前数据
//  输出参数:
//      y：限幅后数据；
//  注意事项:
//      
////////////////////////////////////////////////////////
int limit16(int x)
{
	int b31 = (x & 0x80000000) >> 31;

	if (b31 == 0)
	{
		if (x & 0xFFFF8000)
			x = 0x7fff;
	}
	else
	{
		x = ~x;
		if (x & 0xFFFF8000)
			x = 0x8001;
		else
			x = ~x;
	}
	x &= 0xffff;

	return x;
}