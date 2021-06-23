#include "iir_base.h"
#include <math.h>
#include <string.h>

typedef long long int64;
#define getBit(x,n) ((x>>n)&1)
#define getML8(x)	((x&0xFF00)>>8)
#define setBit(x,n)	x = (x | ((unsigned int)1<<n))
#define shiftR(x,n) x = (x>=0)?(x>>n):((x>>n) | ((1<<n-1)<<(32-n)))
#define shiftR64(x,n) x = (x>=0)?(x>>n):((x>>n) | ((1<<n-1)<<(64-n)))

CIir_base iir1;


// 根据CFG_IIR计算滤波器组输出前需要乘的增益值gain
void CIir_base::calGain(WORD CFG_IIR, double &gain, bool &bB3A3X2)
{
	STC_CFG_IIR stcCfg;
	convert(CFG_IIR, stcCfg);
	bB3A3X2 = stcCfg.bB3A3X2;

	double negtive_gain;
	double positive_gain;

	if (stcCfg.b2048 && stcCfg.b1024)
	{
		negtive_gain = 2048;
	}
	else if (!stcCfg.b2048 && stcCfg.b1024)
	{
		negtive_gain = 1024;
	}
	else
	{
		negtive_gain = 256;
	}

	positive_gain = 1;

	// 必须的基础正增益
	positive_gain *= stcCfg.Xbase;

	// 加法的正增益
	positive_gain *= ((stcCfg.bX025Add ? 0.25 : 0) + (stcCfg.bX05Add ? 0.5 : 0) + (stcCfg.bX1Add ? 1 : 0) + (stcCfg.bX2Add ? 2 : 0));

	gain = positive_gain / negtive_gain;
}

// CFG_IIR转结构体STC_CFG_IIR
void CIir_base::convert(WORD CFG_IIR, STC_CFG_IIR &stcCfg)
{
	
	stcCfg.b2048 = ((CFG_IIR & 0x800) > 0) ? true : false;
	switch ((CFG_IIR & 0x600) >> 9)
	{
	case 0:
		stcCfg.segNum = 4;
		break;
	case 1:
		stcCfg.segNum = 3;
		break;
	case 2:
		stcCfg.segNum = 2;
		break;
	case 3:
		stcCfg.segNum = 1;
		break;
	default:
		break;
	}

	stcCfg.bB3A3X2 = ((CFG_IIR & 0x100) > 0) ? true : false;
	stcCfg.b1024 = ((CFG_IIR & 0x80) > 0) ? true : false;
	stcCfg.bX2Add = ((CFG_IIR & 0x40) > 0) ? true : false;
	stcCfg.bX1Add = ((CFG_IIR & 0x20) > 0) ? true : false;
	stcCfg.bX05Add = ((CFG_IIR & 0x10) > 0) ? true : false;
	stcCfg.bX025Add = ((CFG_IIR & 0x08) > 0) ? true : false;

	switch (CFG_IIR & 0x7)
	{
	case 1:
		stcCfg.Xbase = 1;
		break;
	case 2:
		stcCfg.Xbase = 2;
		break;
	case 4:
		stcCfg.Xbase = 4;
		break;
	default:
		stcCfg.Xbase = 0;
		break;
	}


}

// 将硬件格式的系数转化为内部计算所用的系数a和b,并存入相应的bank里
// bank_id:使用哪一组bank参与运算,取值范围[0, BANK_NUM)
int CIir_base::setPara(int bank_id, short* paraArray)
{
	double gain;
	bool bB3A3x2 = false;
	calGain((WORD)paraArray[ORDER_ADD1 * 2 - 1], gain, bB3A3x2);
	gain_bank[bank_id] = gain;

	for (int i = 0; i < LEVEL; i++)
	{
		for (int j = 0; j < ORDER_ADD1; j++)
		{
			short b_hard = paraArray[i * ORDER_ADD1 * 2 + j];
			short a_hard = paraArray[i * ORDER_ADD1 * 2 + ORDER_ADD1 + j - 1];
			double b_val = convertPara_hard2normal(b_hard, true);
			double a_val = convertPara_hard2normal(a_hard, false);

			if (j == 2)
			{
				b_bank[bank_id][i * ORDER_ADD1 + j] = b_val * (bB3A3x2 ? 2 : 1);
			}
			else
			{
				b_bank[bank_id][i * ORDER_ADD1 + j] = b_val;
			}
			
			if (j >= 1)
			{
				if (j == 2)
				{
					a_bank[bank_id][i * ORDER_ADD1 + j] = a_val * (bB3A3x2?2:1);
				}
				else
				{
					a_bank[bank_id][i * ORDER_ADD1 + j] = a_val;
				}
				
			}
			
		}
	}
	return 0;
}

// 
int CIir_base::iirFilterFrame(int bank, int* dataBuf, int dataLen)
{
	iirFilterLevels(dataBuf, dataLen, bank, LEVEL);
	return 0;
}

// 华莱士乘
int CIir_base::MultiHLS(int a24, int b16)
{
	unsigned int q0, q2, q4, q6, bh8, cout, sout;
	bool co, xc, ct, sign, cout1b, sout1b, carry;
	int result = 0;

	cout = sout = 0;

	// 记录乘法结果的符号位
	sign = !(((a24 < 0) & (b16 > 0)) | ((a24 > 0) & (b16 < 0)));

	// 计算bh8=bh8*a24
	a24 = abs(a24);
	b16 = abs(b16);
	bh8 = getML8(b16);
	bh8 *= a24;

	// 计算bl8*a24
	q0 = (a24 * (b16 & 0x03));
	q2 = (a24 * (b16 & 0x0c));
	q4 = (a24 * (b16 & 0x30));
	q6 = (a24 * (b16 & 0xc0));

	unsigned char temp;
	for (int i = 0; i < 32; i++)
	{
		if (i == 0) xc = 0;
		else xc = co;

		temp = getBit(q0, i);
		temp += getBit(q2, i);
		temp += getBit(q4, i);
		co = temp & 0x02;

		temp = getBit(q0, i);
		temp ^= getBit(q2, i);
		temp ^= getBit(q4, i);
		ct = temp & 0x01;

		// cout
		if (i <= 30)
		{
			temp = getBit(q6, i);
			temp += ct;
			temp += xc;
			cout1b = temp & 0x02;
			if (cout1b)
			{
				cout1b = cout1b;
				setBit(cout, i + 1);
			}
		}

		// Sout[n] = Ct[n] ^ XC[n] ^ Q6[n]
		temp = ct;
		temp ^= xc;
		temp ^= getBit(q6, i);
		sout1b = temp & 0x01;
		if (sout1b)
		{
			sout1b = sout1b;
			setBit(sout, i);
		}

	}


	bh8 += (cout >> 8);
	bh8 += (sout >> 8);
	carry = getBit(cout, 7);
	bh8 += carry;
	carry = getBit(sout, 7);
	bh8 += carry;

	if (!sign) result = (-1) * bh8;
	else result = bh8;

	return result;
}



// 用64位乘，结果缩8位(四舍五入)，返回32位
int CIir_base::multi32_8(int a24, int b16)
{
	return MultiHLS(a24, b16);
}

// bank_id:使用哪一组bank参与运算,取值范围[0, BANK_NUM)
// levelNum:使用多少级iir参与运算,取值范围[1, LEVEL]
int CIir_base::iirFilterLevels(int* dataBuf, int dataLen, int bank_id, int levelNum)
{
	for (int i = 0; i < levelNum; i++)
	{
		iirFilter1Level(a_bank[bank_id] + i* ORDER_ADD1, b_bank[bank_id] + i * ORDER_ADD1, xBuf_bank[bank_id]+i* ORDER_ADD1, yBuf_bank[bank_id] + i * ORDER_ADD1, dataBuf, dataLen);
	}

	// 输出前再调整一次增益
	for (int i = 0; i < dataLen; i++)
	{
		dataBuf[i] = (int)(dataBuf[i] * gain_bank[bank_id]);
	}
	return 0;
}


int CIir_base::iirFilter1Level(double* a, double* b, double* xBufEx, double* yBufEx, int* dataBuf, int dataLen)
{
	int bufLen = ORDER_ADD1;
	double* xBuf = new double[bufLen];
	double* yBuf = new double[bufLen];
	double tempInt = 0;

	// 缓存清0
	memset(xBuf, 0, sizeof(double) * bufLen);
	memset(yBuf, 0, sizeof(double) * bufLen);

	// xBufEx->xBuf, yBufEx->yBuf
	for (int i = 0; i < bufLen; i++)
	{
		xBuf[i] = xBufEx[i];
		yBuf[i] = yBufEx[i];
	}

	for (int i = 0; i < dataLen; i++)
	{
		// 更新x缓存
		for (int j = bufLen - 1; j > 0; j--) xBuf[j] = xBuf[j - 1];
		xBuf[0] = dataBuf[i];

		// 乘累加
		tempInt = 0;
		for (int j = 0; j < bufLen; j++)
		{
			tempInt += (xBuf[j]*b[j] / 256.0);
		}
		for (int j = 1; j < bufLen; j++)
		{
			tempInt -= (yBuf[j]*a[j] / 256.0);
		}

		// 除以32
		tempInt = tempInt / 32.0;

		// 限制器2^23
		int limit = 0x7fffff;
		if (tempInt >= limit)
		{
			tempInt = limit;
		}
		else if (tempInt <= (-limit))
		{
			tempInt = -limit + 1;
		}

		dataBuf[i] = (int)tempInt;

		// 更新y缓存
		for (int j = bufLen - 1; j > 0; j--) yBuf[j] = yBuf[j - 1];
		yBuf[1] = tempInt;

	}

	// xBufEx<-xBuf, yBufEx<-yBuf
	for (int i = 0; i < bufLen; i++)
	{
		xBufEx[i] = xBuf[i];
		yBufEx[i] = yBuf[i];
	}

	delete[] xBuf;
	delete[] yBuf;

	return 0;
}

//  b,a硬件格式转为普通数据值
//	b系数：{符号位，绝对值}
//  a系数：{符号位反码，绝对值}
// type: true-b系数  false-a系数
double CIir_base::convertPara_hard2normal(short input, bool type)
{
	bool sign = input >= 0 ? true : false;		// false:负 true:非负
	short value = input & 0x7fff;

	if (!type)
	{
		sign = !sign;
	}

	if (!sign)
	{
		value = -value;
	}


	return value;
}