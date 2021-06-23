#include "2D_Conv_base.h"
#include <bitset>
using std::bitset;

C2D_Conv_base conv;

////////////////////////////////////////////////////////
//  名称:
//      Multi16_Conv（暂定版）
//  功能:
//      乘法器，通过c语言模拟硬件乘法器特性。
//  参数:
//      1.Multi16_0:乘数，16bit格式
//      2.Multi16_1:乘数，16bit格式      
//  返回值值:
//      result:结果，32bit格式，取bit7~bit31
////////////////////////////////////////////////////////
int C2D_Conv_base::Multi16_Conv(short Multi16_0, short Multi16_1)
{
	if (Multi16_0 == 0x8000)
		Multi16_0--;
	if (Multi16_1 == 0x8000)
		Multi16_1--;

	int result;
 	result = Multi16_0;
	result = result * Multi16_1;
	result = result >> 7;

	return result;
}

////////////////////////////////////////////////////////
//  名称:
//      Sum16_Conv（暂定版）
//  功能:
//      加法累加器，通过c语言模拟硬件特性。
//  参数:
//      1.Sum[]，加数
//      2.len，序列长度
//  返回值值:
//      result:结果，32bit格式，取bit2~bit31
////////////////////////////////////////////////////////
int C2D_Conv_base::Sum16_Conv(int Sum[],int len)
{
	int result = 0;
	for (int i = 0; i < len; i++)
	{
		result += Sum[i];
	}
	result = result >> 2;

	return result;
}



////////////////////////////////////////////////////////
//  名称:
//      Extend_2D_Conv_5x5
//  功能:
//      矩阵扩充。根据5*5卷积核扩充高128*宽64矩阵为(128+4)*(64+4)结果矩阵.补边规则为补原值。
//  参数:
//      1.data:数据指针，原地址输出
//  返回值值:
//      无
////////////////////////////////////////////////////////
void C2D_Conv_base::Extend_2D_Conv_5x5(int* data)
{
	int dataResult[8976];
	//列上扩充
	for (int i = 0; i < 128; i++)
	{
		dataResult[i * 68] = data[i * 64];
		dataResult[i * 68 + 1] = data[i * 64];
		dataResult[i * 68 + 66] = data[i * 64 + 63];
		dataResult[i * 68 + 67] = data[i * 64 + 63];

		for (int n = 0; n < 64; n++)
		{
			dataResult[i * 68 + n + 2] = data[i * 64 + n];
		}
	}
	//行上扩充
	for (int i = 8703; i >= 0; i--)
	{
		dataResult[i + 136] = dataResult[i];
	}
	for (int i = 0; i < 68; i++)
	{
		dataResult[i + 8840] = dataResult[i + 8772];
		dataResult[i + 8908] = dataResult[i + 8772];
		dataResult[i + 68] = dataResult[i + 136];
	}
	for (int i = 0; i < 8976; i++)
	{
		data[i] = dataResult[i];
	}
	return;
}


////////////////////////////////////////////////////////
//  名称:
//      Extend_2D_Conv_5x5_2
//  功能:
//      矩阵扩充。根据5*5卷积核扩充宽256*高64矩阵为(256+4)*(64+4)结果矩阵。补边规则为补0
//  参数:
//      1.data:数据指针，原地址输出
//  返回值值:
//      无
////////////////////////////////////////////////////////
void C2D_Conv_base::Extend_2D_Conv_5x5_2(int* data)
{
	int dataResult[17680] = { 0 };
	//列上扩充
	for (int i = 0; i < 64; i++)
	{
		dataResult[i * 260] = 0;
		dataResult[i * 260 + 1] = 0;
		dataResult[i * 260 + 258] = 0;
		dataResult[i * 260 + 259] = 0;

		for (int n = 0; n < 256; n++)
		{
			dataResult[i * 260 + n + 2] = data[i * 256 + n];
		}
	}
	//行上扩充
	for (int i = 16639; i >= 0; i--)
	{
		dataResult[i + 520] = dataResult[i];
	}
	for (int i = 0; i < 520; i++)
	{
		dataResult[i] = 0;
	}
	for (int i = 0; i < 17680; i++)
	{
		data[i] = dataResult[i];
	}
	return;
}

////////////////////////////////////////////////////////
//  名称:
//      Conv_3X3to5X5
//  功能:
//      将3X3卷积核扩充为5X5
//  参数:
//      1.key:原卷积核，3*3
//      2.key:扩充后卷积核，5*5
//  返回值值:
//      无
////////////////////////////////////////////////////////
void C2D_Conv_base::Conv_3X3to5X5(int* key)
{
	int data[25]{ 0 };
	data[6] = key[0];
	data[7] = key[1];
	data[8] = key[2];
	data[11] = key[3];
	data[12] = key[4];
	data[13] = key[5];
	data[16] = key[6];
	data[17] = key[7];
	data[18] = key[8];
	for (int i = 0; i < 25; i++)
	{
		key[i] = data[i];
	}
}




////////////////////////////////////////////////////////
//  名称:
//      Conv_5x5
//  功能:
//      卷积乘。根据扩充后的高(128+4)*宽(64+4)矩阵与5*5卷积核进行卷积后得到128*64结果矩阵
//  参数:
//      1.data:数据指针，原地址输出
//      2.key:卷积核，5*5
//  返回值值:
//      无
////////////////////////////////////////////////////////
void C2D_Conv_base::Conv_5x5(int* data, int* key)
{
	int s[25];

	//此部分用于硬件验证工作，计算乘累加中间值
	//FILE* f0 = fopen("sumout.txt", "w");
	//FILE* f1 = fopen("out.txt", "w");

	//int sum[44880];//乘累加中间值
	//for (int u = 0; u < 11; u++)//每轮取16列，其中后4列下轮重复取，132列共取11轮
	//{
	//	for (int i = 0; i < 68; i++)//68行，逐行扫描
	//	{
	//		for (int n = 0; n < 5; n++)//5*5卷积核,硬件上五个乘法器，共五轮
	//		{
	//			for (int a = 0; a < 12; a++)//一行出12个乘累加结果
	//			{
	//				for (int m = 0; m < 5; m++)
	//				{
	//					s[m] = Multi16_Conv(data[132 * i + 12 * u + a + m], key[m + 5 * n]);
	//				}
	//				sum[4080 * u + 60 * i + 12 * n + a] = Sum16_Conv(s,5);
	//			}
	//		}
	//	}
	//}
	//for (int i = 0; i < 44880; i++)
	//{
	//	fprintf(f0, "%08x\r", sum[i]);
	//}



	//计算二维卷积
	int result[8192];
	int p[5];
	int q[5];
	for (int i = 0; i < 128; i++)
	{
	for (int n = 0; n < 64; n++)
		{
			s[0] = Multi16_Conv(data[68 * i + n], key[0]);
			s[1] = Multi16_Conv(data[68 * i + n + 1] , key[1]);
			s[2] = Multi16_Conv(data[68 * i + n + 2] , key[2]);
			s[3] = Multi16_Conv(data[68 * i + n + 3] , key[3]);
			s[4] = Multi16_Conv(data[68 * i + n + 4] , key[4]);
			s[5] = Multi16_Conv(data[68 * (i + 1) + n], key[5]);
			s[6] = Multi16_Conv(data[68 * (i + 1) + n + 1] , key[6]);
			s[7] = Multi16_Conv(data[68 * (i + 1) + n + 2] ,key[7]);
			s[8] = Multi16_Conv(data[68 * (i + 1) + n + 3] ,key[8]);
			s[9] = Multi16_Conv(data[68 * (i + 1) + n + 4] ,key[9]);
			s[10] = Multi16_Conv(data[68 * (i + 2) + n] ,key[10]);
			s[11] = Multi16_Conv(data[68 * (i + 2) + n + 1] ,key[11]);
			s[12] = Multi16_Conv(data[68 * (i + 2) + n + 2] ,key[12]);
			s[13] = Multi16_Conv(data[68 * (i + 2) + n + 3] ,key[13]);
			s[14] = Multi16_Conv(data[68 * (i + 2) + n + 4] ,key[14]);
			s[15] = Multi16_Conv(data[68 * (i + 3) + n] ,key[15]);
			s[16] = Multi16_Conv(data[68 * (i + 3) + n + 1] ,key[16]);
			s[17] = Multi16_Conv(data[68 * (i + 3) + n + 2] ,key[17]);
			s[18] = Multi16_Conv(data[68 * (i + 3) + n + 3] ,key[18]);
			s[19] = Multi16_Conv(data[68 * (i + 3) + n + 4] ,key[19]);
			s[20] = Multi16_Conv(data[68 * (i + 4) + n], key[20]);
			s[21] = Multi16_Conv(data[68 * (i + 4) + n + 1], key[21]);
			s[22] = Multi16_Conv(data[68 * (i + 4) + n + 2], key[22]);
			s[23] = Multi16_Conv(data[68 * (i + 4) + n + 3], key[23]);
			s[24] = Multi16_Conv(data[68 * (i + 4) + n + 4], key[24]);
			for (int m = 0; m < 5; m++)//按硬件要求 五个一累加
			{
				p[0] = s[5 * m];
				p[1] = s[5 * m + 1];
				p[2] = s[5 * m + 2];
				p[3] = s[5 * m + 3];
				p[4] = s[5 * m + 4];
				q[m] = Sum16_Conv(p, 5);
			}
			result[64 * i + n] = Sum16_Conv(q, 5);
			result[64 * i + n] = result[64 * i + n] >> 5;
			if (result[64 * i + n] > 32767)
				result[64 * i + n] = 32767;
			else if (result[64 * i + n] < -32767)
				result[64 * i + n] = -32767;
		}
	}

	for (int i = 0; i < 8192; i++)
	{
		data[i] = result[i];
		//fprintf(f1, "%08x\r", result[i]);
	}

	//fclose(f0);
	//fclose(f1);

	return;
}

////////////////////////////////////////////////////////
//  名称:
//      Conv_5x5_2
//  功能:
//      卷积乘。根据扩充(256+4)*(64+4)矩阵与5*5卷积核进行卷积后得到256*64结果矩阵
//  参数:
//      1.data:数据指针，原地址输出
//      2.key:卷积核，5*5
//  返回值值:
//      无
////////////////////////////////////////////////////////
void C2D_Conv_base::Conv_5x5_2(int* data, int* key)
{
	int s[25];

	////此部分用于硬件验证工作，计算乘累加中间值
	//FILE* f0 = fopen("SumOut.txt", "w");
	//FILE* f1 = fopen("Out.txt", "w");

	//int sum[89760];//乘累加中间值
	//for (int u = 0; u < 22; u++)//每轮取16列，其中后4列下轮重复取，260列共取22轮
	//{
	//	for (int i = 0; i < 68; i++)//68行，逐行扫描
	//	{
	//		for (int n = 0; n < 5; n++)//5*5卷积核,硬件上五个乘法器，共五轮
	//		{
	//			for (int a = 0; a < 12; a++)//一行出12个乘累加结果
	//			{
	//				for (int m = 0; m < 5; m++)
	//				{
	//					s[m] = Multi16_Conv(data[260 * i + 12 * u + a + m], key[m + 5 * n]);
	//				}
	//				sum[4080 * u + 60 * i + 12 * n + a] = Sum16_Conv(s,5);
	//			}
	//		}
	//	}
	//}
	//for (int i = 0; i < 89760; i++)
	//{
	//	fprintf(f0, "%08x\r", sum[i]);
	//}

	//计算二维卷积
	int result[16384];
	int p[5];
	int q[5];
	for (int i = 0; i < 64; i++)
	{
		for (int n = 0; n < 256; n++)
		{
			s[0] = Multi16_Conv(data[260 * i + n], key[0]);
			s[1] = Multi16_Conv(data[260 * i + n + 1], key[1]);
			s[2] = Multi16_Conv(data[260 * i + n + 2], key[2]);
			s[3] = Multi16_Conv(data[260 * i + n + 3], key[3]);
			s[4] = Multi16_Conv(data[260 * i + n + 4], key[4]);
			s[5] = Multi16_Conv(data[260 * (i + 1) + n], key[5]);
			s[6] = Multi16_Conv(data[260 * (i + 1) + n + 1], key[6]);
			s[7] = Multi16_Conv(data[260 * (i + 1) + n + 2], key[7]);
			s[8] = Multi16_Conv(data[260 * (i + 1) + n + 3], key[8]);
			s[9] = Multi16_Conv(data[260 * (i + 1) + n + 4], key[9]);
			s[10] = Multi16_Conv(data[260 * (i + 2) + n], key[10]);
			s[11] = Multi16_Conv(data[260 * (i + 2) + n + 1], key[11]);
			s[12] = Multi16_Conv(data[260 * (i + 2) + n + 2], key[12]);
			s[13] = Multi16_Conv(data[260 * (i + 2) + n + 3], key[13]);
			s[14] = Multi16_Conv(data[260 * (i + 2) + n + 4], key[14]);
			s[15] = Multi16_Conv(data[260 * (i + 3) + n], key[15]);
			s[16] = Multi16_Conv(data[260 * (i + 3) + n + 1], key[16]);
			s[17] = Multi16_Conv(data[260 * (i + 3) + n + 2], key[17]);
			s[18] = Multi16_Conv(data[260 * (i + 3) + n + 3], key[18]);
			s[19] = Multi16_Conv(data[260 * (i + 3) + n + 4], key[19]);
			s[20] = Multi16_Conv(data[260 * (i + 4) + n], key[20]);
			s[21] = Multi16_Conv(data[260 * (i + 4) + n + 1], key[21]);
			s[22] = Multi16_Conv(data[260 * (i + 4) + n + 2], key[22]);
			s[23] = Multi16_Conv(data[260 * (i + 4) + n + 3], key[23]);
			s[24] = Multi16_Conv(data[260 * (i + 4) + n + 4], key[24]);
			for (int m = 0; m < 5; m++)
			{
				p[0] = s[5 * m];
				p[1] = s[5 * m + 1];
				p[2] = s[5 * m + 2];
				p[3] = s[5 * m + 3];
				p[4] = s[5 * m + 4];
				q[m] = Sum16_Conv(p, 5);
			}
			result[256 * i + n] = Sum16_Conv(q, 5);
			result[256 * i + n] = result[256 * i + n] >> 5;
			if (result[256 * i + n] > 32767)
				result[256 * i + n] = 32767;
			else if (result[256 * i + n] < -32767)
				result[256 * i + n] = -32767;
		}
	}

	for (int i = 0; i < 16384; i++)
	{
		data[i] = result[i];
		//fprintf(f1, "%08x\r", result[i]);
	}

	//fclose(f0);
	//fclose(f1);

	return;
}

////////////////////////////////////////////////////////
//  名称:
//      Multi_Array
//  功能:
//      16*16矩阵乘
//  参数:
//      1.A:数据指针A，原地址输出
//      2.B:数据指针B
//  返回值值:
//      无
////////////////////////////////////////////////////////
void C2D_Conv_base::Multi_Array(short* A, short* B, int* C)
{
	int data[16];
	int T[256];

	//矩阵乘
	for (int i = 0; i < 16; i++)
	{
		for (int k = 0; k < 16; k++)
		{
			for (int m = 0; m < 16; m++)
			{
				data[m] = conv.Multi16_Conv(A[16 * i + m], B[16 * m + k]);
			}
			C[16 * i + k] = conv.Sum16_Conv(data, 16);
			C[16 * i + k] = C[16 * i + k] >> 7;
			if (C[16 * i + k] > 32767)//限幅
				C[16 * i + k] = 32767;
			else if (C[16 * i + k] < -32767)
				C[16 * i + k] = -32767;
			//T为硬件中间值验证过程
			if (i > k)
				T[16 * i + k] = B[16 * i + 16 + k - i];
			else
				T[16 * i + k] = B[16 * i + k - i];
		}
	}

	return;

}