#include "2D_Conv.h"
#include "2D_Conv_base.h"


////////////////////////////////////////////////////////
//  名称:
//      Multi_Array_8x8(根据硬件同步，暂不提供8*8，如有需求请使用16*16)
//  功能:
//      两个8X8矩阵相乘,数据格式为Q15
//  参数:
//      1.M[RSP+2*MMU_BASE]：输入矩阵A的首地址
//      2.M[RSP+1*MMU_BASE]：输入矩阵B的首地址
//      3.M[RSP+0*MMU_BASE]：输出矩阵的首地址
//  返回值:
//      无
////////////////////////////////////////////////////////
//Sub_AutoField Multi_Array_8x8
//{
//	RA0 = M[RSP + 2 * MMU_BASE];	//输入矩阵A的地址	
//	RA1 = M[RSP + 1 * MMU_BASE];	//输入矩阵B的地址
//	RA2 = M[RSP + 0 * MMU_BASE];	//输出地址
//
//	double A[64], B[64];
//	double C[64] = { 0 };
//	for (int i = 0; i < 64; i++)
//	{
//		RD0 = M[RA0++];
//		A[i] = RD0.m_data;
//		A[i] = A[i] / 32768;
//	}
//	for (int i = 0; i < 64; i++)
//	{
//		RD0 = M[RA1++];
//		B[i] = RD0.m_data;
//		B[i] = B[i] / 32768;
//	}
//	//算法实现
//	for (int i = 0; i < 8; i++)
//	{
//		for (int k = 0; k < 8; k++)
//		{
//			for (int m = 0; m < 8; m++)
//			{
//				C[8 * i + k] += A[8 * i + m] * B[8 * m + k];
//			}
//			C[8 * i + k] = C[8 * i + k] * 32768;
//		}
//	}
//
//	for (int i = 0; i < 64; i++)
//	{
//		RD0 = C[i];
//		M[RA2++] = RD0;
//	}
//
//	Return_AutoField(3 * MMU_BASE);		//压栈了三次，函数结尾处需推栈三次
//}


////////////////////////////////////////////////////////
//  名称:
//      Multi_Array_16X16
//  功能:
//      两个16X16矩阵相乘,数据格式为Q15
//  参数:
//      1.M[RSP+2*MMU_BASE]：输入矩阵A的首地址，q15
//      2.M[RSP+1*MMU_BASE]：输入矩阵B的首地址，q15
//      3.M[RSP+0*MMU_BASE]：输出矩阵的首地址，q16已限幅
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Multi_Array_16x16
{
	RA0 = M[RSP + 2 * MMU_BASE];	//输入矩阵A的地址	
	RA1 = M[RSP + 1 * MMU_BASE];	//输入矩阵B的地址
	RA2 = M[RSP + 0 * MMU_BASE];	//输出地址
	short A[256], B[256], T[256];
	int C[256] = { 0 };
	int data[16];
	for (int i = 0; i < 256; i++)
	{
		RD0 = M[RA0++];
		A[i] = RD0.m_data;
	}
	for (int i = 0; i < 256; i++)
	{
		RD0 = M[RA1++];
		B[i] = RD0.m_data;
	}
	conv.Multi_Array(A, B, C);

	//RA1 = RN_GRAM0;
	for (int i = 0; i < 256; i++)
	{
		RD0 = C[i];
		M[RA2++] = RD0;
		//RD0 = T[i];
		//M[RA1++] = RD0;

	}
	Return_AutoField(3 * MMU_BASE);		
}

////////////////////////////////////////////////////////
//  名称:
//      Conv_2D_3X3
//  功能:
//      卷积核大小为3X3的二维卷积计算（卷积层高128*宽64）
//  参数:
//      1.M[RSP+2*MMU_BASE]：data地址,数据格式为Q15
//      2.M[RSP+1*MMU_BASE]：卷积滤波器系数存放地址,数据格式为Q15
//      3.M[RSP+0*MMU_BASE]：卷积结果地址,数据格式为Q7
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Conv_2D_3X3
{
	RA0 = M[RSP + 2 * MMU_BASE];	//data地址
	RA1 = M[RSP + 1 * MMU_BASE];	//卷积滤波器系数存放地址
	RA2 = M[RSP + 0 * MMU_BASE];	//卷积结果地址

	int data[8976],key[25];
	for (int i = 0; i < 8192; i++)
	{
		RD0 = M[RA0++];
		data[i] = RD0.m_data;
	}
	for (int i = 0; i < 25; i++)
	{
		RD0 = M[RA1++];
		key[i] = RD0.m_data;
	}

	//3X3扩充为5X5
	//卷积核扩充
	conv.Conv_3X3to5X5(key);
	//扩充图像，扩充后为（128+4）*（64+4）
	conv.Extend_2D_Conv_5x5(data);
	//卷积乘
	conv.Conv_5x5(data, key);


	//结果放到输出指针
	for (int i = 0; i < 8192; i++)
	{
		RD0 = data[i];
		M[RA2++] = RD0;
	}

	Return_AutoField(3 * MMU_BASE);		//压栈了三次，函数结尾处需推栈三次

}

////////////////////////////////////////////////////////
//  名称:
//      Conv_2D_5X5
//  功能:
//      卷积核大小为5X5的二维卷积计算（卷积层高128*宽64）(补原值)
//  参数:
//      1.M[RSP+2*MMU_BASE]：data地址,数据格式为Q15
//      2.M[RSP+1*MMU_BASE]：卷积滤波器系数存放地址,数据格式为Q15
//      3.M[RSP+0*MMU_BASE]：卷积结果地址,数据格式为Q16,已限幅
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Conv_2D_5X5
{
	RA0 = M[RSP + 2 * MMU_BASE];	//data地址
	RA1 = M[RSP + 1 * MMU_BASE];	//卷积滤波器系数存放地址
	RA2 = M[RSP + 0 * MMU_BASE];	//卷积结果地址

	int data[8976],key[25];
	for (int i = 0; i < 8192; i++)
	{
		RD0 = M[RA0++];
		data[i] = RD0.m_data;
	}
	for (int i = 0; i < 25; i++)
	{
		RD0 = M[RA1++];
		key[i] = RD0.m_data;
	}

	//扩充图像
	//扩充后为（128+4）*（64+4）
	conv.Extend_2D_Conv_5x5(data);

	//卷积乘
	conv.Conv_5x5(data, key);




	//结果放到输出指针
	for (int i = 0; i < 8192; i++)
	{
		RD0 = data[i];
		M[RA2++] = RD0;
	}

	Return_AutoField(3 * MMU_BASE);		//压栈了三次，函数结尾处需推栈三次
}

////////////////////////////////////////////////////////
//  名称:
//      Conv_2D_5X5_2(暂不开放)
//  功能:
//      卷积核大小为5X5的二维卷积计算（卷积层宽256*高64）(边上补0)
//  参数:
//      1.M[RSP+2*MMU_BASE]：data地址,数据格式为Q15
//      2.M[RSP+1*MMU_BASE]：卷积滤波器系数存放地址
//      3.M[RSP+0*MMU_BASE]：卷积结果地址,数据格式为Q16,已限幅
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Conv_2D_5X5_2
{
	RA0 = M[RSP + 2 * MMU_BASE];	//data地址
	RA1 = M[RSP + 1 * MMU_BASE];	//卷积滤波器系数存放地址
	RA2 = M[RSP + 0 * MMU_BASE];	//卷积结果地址

	int data[17680],key[25];
	for (int i = 0; i < 16384; i++)
	{
		RD0 = M[RA0++];
		data[i] = RD0.m_data;
	}
	for (int i = 0; i < 25; i++)
	{
		RD0 = M[RA1++];
		key[i] = RD0.m_data;
	}

	//扩充图像
	//扩充后为（256+4）*（64+4）
	conv.Extend_2D_Conv_5x5_2(data);

	//卷积乘
	conv.Conv_5x5_2(data, key);




	//结果放到输出指针
	for (int i = 0; i < 16384; i++)
	{
		RD0 = data[i];
		M[RA2++] = RD0;
	}

	Return_AutoField(3 * MMU_BASE);		//压栈了三次，函数结尾处需推栈三次
}



////////////////////////////////////////////////////////
//  名称:
//      ReLU
//  功能:
//      ReLU激活函数
//  参数:
//      1.M[RSP+2*MMU_BASE]：输入地址
//      2.M[RSP+1*MMU_BASE]：输出地址
//      3.M[RSP+0*MMU_BASE]：数据长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField ReLU
{
	RA0 = M[RSP + 2 * MMU_BASE];	
	RA1 = M[RSP + 1 * MMU_BASE];	
	RD0 = M[RSP + 0 * MMU_BASE];	

	int n = RD0.m_data;
	for (int i = 0; i < n; i++)
	{
		RD0 = M[RA0++];
		if (RD0 < 0)
			RD0 = 0;
		M[RA1++] = RD0;
	}


	Return_AutoField(3 * MMU_BASE);		

}


////////////////////////////////////////////////////////
//  名称:
//      Softmax
//  功能:
//      Softmax函数，q15
//  参数:
//      1.M[RSP+2*MMU_BASE]：输入地址
//      2.M[RSP+1*MMU_BASE]：输出地址
//      3.M[RSP+0*MMU_BASE]：数据长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Softmax
{
	RA0 = M[RSP + 2 * MMU_BASE];
	RA1 = M[RSP + 1 * MMU_BASE];
	RD0 = M[RSP + 0 * MMU_BASE];

	int n = RD0.m_data;
	double data[10];
	double c = 0;
	double r = 0;

	for (int i = 0; i < n; i++)
	{
		RD0 = M[RA0++];
		data[i] = RD0.m_data;
		data[i] = data[i] / 32767;
	}

	//计算exp累加和
	for (int i = 0; i < n; i++)
	{
		c += exp(data[i]);
	}

	for (int i = 0; i < n; i++)
	{
		r = exp(data[i]);
		r = 32767 * r / c;
		RD0 = r;
		M[RA1++] = RD0;
	}


	Return_AutoField(3 * MMU_BASE);

}

////////////////////////////////////////////////////////
//  名称:
//      Logistic
//  功能:
//      Logistic函数，q15
//  参数:
//      1.M[RSP+2*MMU_BASE]：输入地址
//      2.M[RSP+1*MMU_BASE]：输出地址
//      3.M[RSP+0*MMU_BASE]：数据长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Logistic
{
	RA0 = M[RSP + 2 * MMU_BASE];
	RA1 = M[RSP + 1 * MMU_BASE];
	RD0 = M[RSP + 0 * MMU_BASE];

	int len = RD0.m_data;
	double x;
	for (int i = 0; i < len; i++)
	{
		RD0 = M[RA0++];
		x = RD0.m_data;
		x = x / 32767;
		x = exp(-x);
		x = 1 / (1 + x);
		x = x * 32767;
		RD0 = x;
		M[RA1++] = RD0;
	}

	Return_AutoField(3 * MMU_BASE);
}

////////////////////////////////////////////////////////
//  名称:
//      Tanh
//  功能:
//      Tanh函数，q15
//  参数:
//      1.M[RSP+2*MMU_BASE]：输入地址
//      2.M[RSP+1*MMU_BASE]：输出地址
//      3.M[RSP+0*MMU_BASE]：数据长度
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Tanh
{
	RA0 = M[RSP + 2 * MMU_BASE];
	RA1 = M[RSP + 1 * MMU_BASE];
	RD0 = M[RSP + 0 * MMU_BASE];

	int len = RD0.m_data;
	double x,y;
	for (int i = 0; i < len; i++)
	{
		RD0 = M[RA0++];
		x = RD0.m_data;
		x = x / 32767;
		y = exp(-x);
		x = exp(x);
		x = (x - y) / (x + y);
		x = x * 32767;
		RD0 = x;
		M[RA1++] = RD0;
	}

	Return_AutoField(3 * MMU_BASE);
}

////////////////////////////////////////////////////////
//  名称:
//      Mac_48_1024
//  功能:
//      1024长度的48位乘累加器
//  参数:
//      1.M[RSP+2*MMU_BASE]：输入地址0，低16bit有效
//      2.M[RSP+1*MMU_BASE]：输入地址1，低16bit有效
//      3.RD0：输出结果的低32位
//		4.RD1：输出结果的高16位,低16位有效
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Mac_48_1024
{
	RA0 = M[RSP + 1 * MMU_BASE];
	RA1 = M[RSP + 0 * MMU_BASE];
	int len = 1024;
	long long sum = 0;
	int x,y;
	for (int i = 0; i < len; i++)
	{
		RD0 = M[RA0++];
		RD1 = M[RA1++];
		x = *(short*)(&RD0.m_data);					//低16bit 
		y = *(short*)(&RD1.m_data);					//低16bit 
		x = x * y;
		sum += x;
	}
	x = sum >> 32;
	y = sum;
	RD0 = y;
	RD1 = x;

	Return_AutoField(2 * MMU_BASE);

}


////////////////////////////////////////////////////////
//  名称:
//      Rolling_Multi
//  功能:
//      高128*宽64的矩阵与16*16矩阵滚动矩阵乘
//  参数:
//      1.M[RSP+2*MMU_BASE]：高128*宽64的矩阵data首地址,数据格式为Q15
//      2.M[RSP+1*MMU_BASE]：16*16矩阵存放首地址,数据格式为Q15
//      3.M[RSP+0*MMU_BASE]：计算结果地址,数据格式为Q16,已限幅
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Rolling_Multi
{
	RA0 = M[RSP + 2 * MMU_BASE];	//128*64矩阵首地址
	RA1 = M[RSP + 1 * MMU_BASE];	//16*16矩阵首地址
	RA2 = M[RSP + 0 * MMU_BASE];	//结果地址

	short D[8192], A[256], B[256];
	int C[256] = { 0 };
	int data[16];
	for (int i = 0; i < 8192; i++)
	{
		RD0 = M[RA0++];
		D[i] = RD0.m_data;
	}
	for (int i = 0; i < 256; i++)
	{
		RD0 = M[RA1++];
		B[i] = RD0.m_data;
	}

	for (int i = 0; i < 8; i++)
	{
		for (int n = 0; n < 4; n++)
		{
			for (int j = 0; j < 16; j++)
			{
				for (int k = 0; k < 16; k++)
				{
					A[j * 16 + k] = D[1024 * i + 64 * j + k + 16 * n];
				}
			}
			conv.Multi_Array(A, B, C);
			for (int j = 0; j < 16; j++)
			{
				for (int k = 0; k < 16; k++)
				{
					D[1024 * i + 64 * j + k + 16 * n] = C[j * 16 + k];
				}
			}

		}
	}

	for (int i = 0; i < 8192; i++)
	{
		RD0 = D[i];
		M[RA2++] = RD0;
		//RD0 = T[i];
		//M[RA1++] = RD0;

	}
	Return_AutoField(3 * MMU_BASE);



}
