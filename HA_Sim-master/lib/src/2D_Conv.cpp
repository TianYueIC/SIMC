#include "2D_Conv.h"
#include "2D_Conv_base.h"


////////////////////////////////////////////////////////
//  ����:
//      Multi_Array_8x8(����Ӳ��ͬ�����ݲ��ṩ8*8������������ʹ��16*16)
//  ����:
//      ����8X8�������,���ݸ�ʽΪQ15
//  ����:
//      1.M[RSP+2*MMU_BASE]���������A���׵�ַ
//      2.M[RSP+1*MMU_BASE]���������B���׵�ַ
//      3.M[RSP+0*MMU_BASE]�����������׵�ַ
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
//Sub_AutoField Multi_Array_8x8
//{
//	RA0 = M[RSP + 2 * MMU_BASE];	//�������A�ĵ�ַ	
//	RA1 = M[RSP + 1 * MMU_BASE];	//�������B�ĵ�ַ
//	RA2 = M[RSP + 0 * MMU_BASE];	//�����ַ
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
//	//�㷨ʵ��
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
//	Return_AutoField(3 * MMU_BASE);		//ѹջ�����Σ�������β������ջ����
//}


////////////////////////////////////////////////////////
//  ����:
//      Multi_Array_16X16
//  ����:
//      ����16X16�������,���ݸ�ʽΪQ15
//  ����:
//      1.M[RSP+2*MMU_BASE]���������A���׵�ַ��q15
//      2.M[RSP+1*MMU_BASE]���������B���׵�ַ��q15
//      3.M[RSP+0*MMU_BASE]�����������׵�ַ��q16���޷�
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Multi_Array_16x16
{
	RA0 = M[RSP + 2 * MMU_BASE];	//�������A�ĵ�ַ	
	RA1 = M[RSP + 1 * MMU_BASE];	//�������B�ĵ�ַ
	RA2 = M[RSP + 0 * MMU_BASE];	//�����ַ
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
//  ����:
//      Conv_2D_3X3
//  ����:
//      ����˴�СΪ3X3�Ķ�ά������㣨������128*��64��
//  ����:
//      1.M[RSP+2*MMU_BASE]��data��ַ,���ݸ�ʽΪQ15
//      2.M[RSP+1*MMU_BASE]������˲���ϵ����ŵ�ַ,���ݸ�ʽΪQ15
//      3.M[RSP+0*MMU_BASE]����������ַ,���ݸ�ʽΪQ7
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Conv_2D_3X3
{
	RA0 = M[RSP + 2 * MMU_BASE];	//data��ַ
	RA1 = M[RSP + 1 * MMU_BASE];	//����˲���ϵ����ŵ�ַ
	RA2 = M[RSP + 0 * MMU_BASE];	//��������ַ

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

	//3X3����Ϊ5X5
	//���������
	conv.Conv_3X3to5X5(key);
	//����ͼ�������Ϊ��128+4��*��64+4��
	conv.Extend_2D_Conv_5x5(data);
	//�����
	conv.Conv_5x5(data, key);


	//����ŵ����ָ��
	for (int i = 0; i < 8192; i++)
	{
		RD0 = data[i];
		M[RA2++] = RD0;
	}

	Return_AutoField(3 * MMU_BASE);		//ѹջ�����Σ�������β������ջ����

}

////////////////////////////////////////////////////////
//  ����:
//      Conv_2D_5X5
//  ����:
//      ����˴�СΪ5X5�Ķ�ά������㣨������128*��64��(��ԭֵ)
//  ����:
//      1.M[RSP+2*MMU_BASE]��data��ַ,���ݸ�ʽΪQ15
//      2.M[RSP+1*MMU_BASE]������˲���ϵ����ŵ�ַ,���ݸ�ʽΪQ15
//      3.M[RSP+0*MMU_BASE]����������ַ,���ݸ�ʽΪQ16,���޷�
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Conv_2D_5X5
{
	RA0 = M[RSP + 2 * MMU_BASE];	//data��ַ
	RA1 = M[RSP + 1 * MMU_BASE];	//����˲���ϵ����ŵ�ַ
	RA2 = M[RSP + 0 * MMU_BASE];	//��������ַ

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

	//����ͼ��
	//�����Ϊ��128+4��*��64+4��
	conv.Extend_2D_Conv_5x5(data);

	//�����
	conv.Conv_5x5(data, key);




	//����ŵ����ָ��
	for (int i = 0; i < 8192; i++)
	{
		RD0 = data[i];
		M[RA2++] = RD0;
	}

	Return_AutoField(3 * MMU_BASE);		//ѹջ�����Σ�������β������ջ����
}

////////////////////////////////////////////////////////
//  ����:
//      Conv_2D_5X5_2(�ݲ�����)
//  ����:
//      ����˴�СΪ5X5�Ķ�ά������㣨������256*��64��(���ϲ�0)
//  ����:
//      1.M[RSP+2*MMU_BASE]��data��ַ,���ݸ�ʽΪQ15
//      2.M[RSP+1*MMU_BASE]������˲���ϵ����ŵ�ַ
//      3.M[RSP+0*MMU_BASE]����������ַ,���ݸ�ʽΪQ16,���޷�
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Conv_2D_5X5_2
{
	RA0 = M[RSP + 2 * MMU_BASE];	//data��ַ
	RA1 = M[RSP + 1 * MMU_BASE];	//����˲���ϵ����ŵ�ַ
	RA2 = M[RSP + 0 * MMU_BASE];	//��������ַ

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

	//����ͼ��
	//�����Ϊ��256+4��*��64+4��
	conv.Extend_2D_Conv_5x5_2(data);

	//�����
	conv.Conv_5x5_2(data, key);




	//����ŵ����ָ��
	for (int i = 0; i < 16384; i++)
	{
		RD0 = data[i];
		M[RA2++] = RD0;
	}

	Return_AutoField(3 * MMU_BASE);		//ѹջ�����Σ�������β������ջ����
}



////////////////////////////////////////////////////////
//  ����:
//      ReLU
//  ����:
//      ReLU�����
//  ����:
//      1.M[RSP+2*MMU_BASE]�������ַ
//      2.M[RSP+1*MMU_BASE]�������ַ
//      3.M[RSP+0*MMU_BASE]�����ݳ���
//  ����ֵ:
//      ��
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
//  ����:
//      Softmax
//  ����:
//      Softmax������q15
//  ����:
//      1.M[RSP+2*MMU_BASE]�������ַ
//      2.M[RSP+1*MMU_BASE]�������ַ
//      3.M[RSP+0*MMU_BASE]�����ݳ���
//  ����ֵ:
//      ��
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

	//����exp�ۼӺ�
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
//  ����:
//      Logistic
//  ����:
//      Logistic������q15
//  ����:
//      1.M[RSP+2*MMU_BASE]�������ַ
//      2.M[RSP+1*MMU_BASE]�������ַ
//      3.M[RSP+0*MMU_BASE]�����ݳ���
//  ����ֵ:
//      ��
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
//  ����:
//      Tanh
//  ����:
//      Tanh������q15
//  ����:
//      1.M[RSP+2*MMU_BASE]�������ַ
//      2.M[RSP+1*MMU_BASE]�������ַ
//      3.M[RSP+0*MMU_BASE]�����ݳ���
//  ����ֵ:
//      ��
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
//  ����:
//      Mac_48_1024
//  ����:
//      1024���ȵ�48λ���ۼ���
//  ����:
//      1.M[RSP+2*MMU_BASE]�������ַ0����16bit��Ч
//      2.M[RSP+1*MMU_BASE]�������ַ1����16bit��Ч
//      3.RD0���������ĵ�32λ
//		4.RD1���������ĸ�16λ,��16λ��Ч
//  ����ֵ:
//      ��
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
		x = *(short*)(&RD0.m_data);					//��16bit 
		y = *(short*)(&RD1.m_data);					//��16bit 
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
//  ����:
//      Rolling_Multi
//  ����:
//      ��128*��64�ľ�����16*16������������
//  ����:
//      1.M[RSP+2*MMU_BASE]����128*��64�ľ���data�׵�ַ,���ݸ�ʽΪQ15
//      2.M[RSP+1*MMU_BASE]��16*16�������׵�ַ,���ݸ�ʽΪQ15
//      3.M[RSP+0*MMU_BASE]����������ַ,���ݸ�ʽΪQ16,���޷�
//  ����ֵ:
//      ��
////////////////////////////////////////////////////////
Sub_AutoField Rolling_Multi
{
	RA0 = M[RSP + 2 * MMU_BASE];	//128*64�����׵�ַ
	RA1 = M[RSP + 1 * MMU_BASE];	//16*16�����׵�ַ
	RA2 = M[RSP + 0 * MMU_BASE];	//�����ַ

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
