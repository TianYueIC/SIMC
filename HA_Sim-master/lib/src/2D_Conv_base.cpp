#include "2D_Conv_base.h"
#include <bitset>
using std::bitset;

C2D_Conv_base conv;

////////////////////////////////////////////////////////
//  ����:
//      Multi16_Conv���ݶ��棩
//  ����:
//      �˷�����ͨ��c����ģ��Ӳ���˷������ԡ�
//  ����:
//      1.Multi16_0:������16bit��ʽ
//      2.Multi16_1:������16bit��ʽ      
//  ����ֵֵ:
//      result:�����32bit��ʽ��ȡbit7~bit31
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
//  ����:
//      Sum16_Conv���ݶ��棩
//  ����:
//      �ӷ��ۼ�����ͨ��c����ģ��Ӳ�����ԡ�
//  ����:
//      1.Sum[]������
//      2.len�����г���
//  ����ֵֵ:
//      result:�����32bit��ʽ��ȡbit2~bit31
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
//  ����:
//      Extend_2D_Conv_5x5
//  ����:
//      �������䡣����5*5����������128*��64����Ϊ(128+4)*(64+4)�������.���߹���Ϊ��ԭֵ��
//  ����:
//      1.data:����ָ�룬ԭ��ַ���
//  ����ֵֵ:
//      ��
////////////////////////////////////////////////////////
void C2D_Conv_base::Extend_2D_Conv_5x5(int* data)
{
	int dataResult[8976];
	//��������
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
	//��������
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
//  ����:
//      Extend_2D_Conv_5x5_2
//  ����:
//      �������䡣����5*5����������256*��64����Ϊ(256+4)*(64+4)������󡣲��߹���Ϊ��0
//  ����:
//      1.data:����ָ�룬ԭ��ַ���
//  ����ֵֵ:
//      ��
////////////////////////////////////////////////////////
void C2D_Conv_base::Extend_2D_Conv_5x5_2(int* data)
{
	int dataResult[17680] = { 0 };
	//��������
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
	//��������
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
//  ����:
//      Conv_3X3to5X5
//  ����:
//      ��3X3���������Ϊ5X5
//  ����:
//      1.key:ԭ����ˣ�3*3
//      2.key:��������ˣ�5*5
//  ����ֵֵ:
//      ��
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
//  ����:
//      Conv_5x5
//  ����:
//      ����ˡ����������ĸ�(128+4)*��(64+4)������5*5����˽��о����õ�128*64�������
//  ����:
//      1.data:����ָ�룬ԭ��ַ���
//      2.key:����ˣ�5*5
//  ����ֵֵ:
//      ��
////////////////////////////////////////////////////////
void C2D_Conv_base::Conv_5x5(int* data, int* key)
{
	int s[25];

	//�˲�������Ӳ����֤������������ۼ��м�ֵ
	//FILE* f0 = fopen("sumout.txt", "w");
	//FILE* f1 = fopen("out.txt", "w");

	//int sum[44880];//���ۼ��м�ֵ
	//for (int u = 0; u < 11; u++)//ÿ��ȡ16�У����к�4�������ظ�ȡ��132�й�ȡ11��
	//{
	//	for (int i = 0; i < 68; i++)//68�У�����ɨ��
	//	{
	//		for (int n = 0; n < 5; n++)//5*5�����,Ӳ��������˷�����������
	//		{
	//			for (int a = 0; a < 12; a++)//һ�г�12�����ۼӽ��
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



	//�����ά���
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
			for (int m = 0; m < 5; m++)//��Ӳ��Ҫ�� ���һ�ۼ�
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
//  ����:
//      Conv_5x5_2
//  ����:
//      ����ˡ���������(256+4)*(64+4)������5*5����˽��о����õ�256*64�������
//  ����:
//      1.data:����ָ�룬ԭ��ַ���
//      2.key:����ˣ�5*5
//  ����ֵֵ:
//      ��
////////////////////////////////////////////////////////
void C2D_Conv_base::Conv_5x5_2(int* data, int* key)
{
	int s[25];

	////�˲�������Ӳ����֤������������ۼ��м�ֵ
	//FILE* f0 = fopen("SumOut.txt", "w");
	//FILE* f1 = fopen("Out.txt", "w");

	//int sum[89760];//���ۼ��м�ֵ
	//for (int u = 0; u < 22; u++)//ÿ��ȡ16�У����к�4�������ظ�ȡ��260�й�ȡ22��
	//{
	//	for (int i = 0; i < 68; i++)//68�У�����ɨ��
	//	{
	//		for (int n = 0; n < 5; n++)//5*5�����,Ӳ��������˷�����������
	//		{
	//			for (int a = 0; a < 12; a++)//һ�г�12�����ۼӽ��
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

	//�����ά���
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
//  ����:
//      Multi_Array
//  ����:
//      16*16�����
//  ����:
//      1.A:����ָ��A��ԭ��ַ���
//      2.B:����ָ��B
//  ����ֵֵ:
//      ��
////////////////////////////////////////////////////////
void C2D_Conv_base::Multi_Array(short* A, short* B, int* C)
{
	int data[16];
	int T[256];

	//�����
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
			if (C[16 * i + k] > 32767)//�޷�
				C[16 * i + k] = 32767;
			else if (C[16 * i + k] < -32767)
				C[16 * i + k] = -32767;
			//TΪӲ���м�ֵ��֤����
			if (i > k)
				T[16 * i + k] = B[16 * i + 16 + k - i];
			else
				T[16 * i + k] = B[16 * i + k - i];
		}
	}

	return;

}