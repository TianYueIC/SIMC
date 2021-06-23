#include "Math_F.h"
#include "memory.h"
#include <math.h>


////////////////////////////////////////////////////////
//  名称:
//      power_fix
//  功能:
//      定点求Power = 10^(RD0/10)
//  参数:
//      1.RD0: 数据
//  返回值:
//      1.RD0: 结果（32q8）
////////////////////////////////////////////////////////
Sub_AutoField power_fix
{
    push(RD4);
    push(RD5);
    push(RD6);
    push(RD7);

    #define E  RD4
    #define MM RD5
    #define x  RD6
    #define r  RD7

    x = RD0;

    // 1. 计算x = Level*0.33219 = (Level*10885)/Q15
    // x = level * 10885;
    //    RD0 = 10885;
    //    Multi24_X=RD0;
    //    RD0 = x;
    //    Multi24_Y=RD0;
    //    nop;
    //    RD0=Multi24_XY;
    //    RD0_ClrByteH8;
        //x = RD0;

    RD1 = 10885;
    call_AutoField _Rs_Multi;

    //x >>= 15;
    RD1 = 15;
    call_AutoField _Rf_ShiftR_Reg;
    x = RD0;

    // 2. 计算E和r，根据公式 x = (E+1) + (r-1),  E为正整数 ， r为[0,1)的数
    // E = (x >> 8) + 1;               // x是16q8，低8位为小数
    //RD0 = x;
    RD1 = 8;
    call_AutoField _Rf_ShiftR_Reg;
    RD0+=1;
    E = RD0;

    // r = x & 0xFF;
    RD0 = x;
    RF_GetL8(RD0);
    r = RD0;

    //  if ((r & 0x1) == 1)         // 判bit0是否为1
    if (RD0_Bit0 == 0) goto L_power_fix_0;
    //r += 2;
    r += 2;

L_power_fix_0:
    //r = r >> 1;                   // 移剩下的1
    RF_ShiftR1(r);

    // 如果r==N,r-=1
    // if (r == N)
    RD1 = 128;
    RD1 -= r;
    if (RQ_nZero) goto L_power_fix_1;
    // r -= 1;
    r-=1;
L_power_fix_1:

    // 2. 以M为地址读取结果
    //rst = (1 << E) * exp2_table[r];
    RD0 = E;
    RF_Exp(RD0);
    RD2 = RD0;

    RD0 = r;
    RF_ShiftL1(RD0);
    RD1 = RN_Pow2_Table_ADDR;
    RF_GetL16(RD1);
    RF_ShiftL2(RD1);
    RD0 += RD1;
    call ConstROM_Read_Word;
    RD1 = RD2;

    call_AutoField _Rs_Multi;
    //    Multi24_X=RD1;
    //    //RD0 = x;
    //    Multi24_Y=RD0;
    //    nop;
    //    RD0=Multi24_XY;
    //    RD0_ClrByteH8;

    #undef E
    #undef MM
    #undef x
    #undef r

    pop(RD7);
    pop(RD6);
    pop(RD5);
    pop(RD4);

    Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  名称:
//      recip_fix
//  功能:
//      定点求倒数
//  参数:
//      1.RD0: 数据(q0)
//  返回值:
//      1.RD0: 结果（q23）
////////////////////////////////////////////////////////
Sub_AutoField recip_fix
{

    push(RD4);
    push(RD5);
    push(RD6);

    #define E  RD4
    #define MM RD5
    #define x  RD6

    RD2 = 0;
    if (RD0_Bit31 == 0) goto L_recip_fix_2;
    RD2 = 1;
    RF_Not(RD0);
    RD0 += 1;
L_recip_fix_2:
    // 1. 计算E和M，根据公式 x = 2^E * M,  E为正整数 ， M为[0.5~1)的数
    // E = log2(x);                 // 汇编有CPU指令支持
    x = RD0;
    RF_Log(RD0);
    E = RD0;

    // M = x << index;                 // 汇编可优化成移位
    RD0 = x;
    RD1 = 8;
    call_AutoField _Rf_ShiftL_Reg;
    MM = RD0;

    // M = M >> (E);                   // 先移log2(x)-1    目的是四舍五入
    RD0 = MM;
    RD1 = E;
    call_AutoField _Rf_ShiftR_Reg;
    MM = RD0;

    // if ((M & 0x1) == 1)             // 判bit0是否为1
    if (RD0_Bit0 == 0) goto L_recip_fix_0;
    // M += 2;
    MM += 2;

L_recip_fix_0:
    // M = (M >> 1) - 128;                 // 移剩下的1
    RD0 = MM;
    RF_ShiftR1(RD0);
    RD0 -= 128;
    MM = RD0;

    // 2. 以M为地址读取结果
    // 如果M==N,M-=1
    // if (M == N)
    RD1 = 128;
    RD0 = MM;
    RD1 -= RD0;
    if (RQ_nZero) goto L_recip_fix_1;
    // M -= 1;
    MM -= 1;

L_recip_fix_1:
    // rst = recip_table[M] >> (E);
    //RD0 = MM;
    //RD1 = RN_Recip_Table_ADDR;
    //RA0 = RD1;
    //RD0 = M[RA0 + RD0];

    RD0 = MM;
    RD1 = RN_Recip_Table_ADDR;
    RD0 += RD1;
    call ConstROM_Read_DWord;


    RD1 = E;
    call_AutoField _Rf_ShiftR_Reg;

    #undef E
    #undef MM
    #undef x
    RD1 = 1;
    RD1 ^= RD2;
    if (RQ_nZero) goto L_recip_fix_End;
    RF_Not(RD0);
    RD0 += 1;
L_recip_fix_End:

    pop(RD6);
    pop(RD5);
    pop(RD4);

    Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  名称:
//      recip_fix_Q7
//  功能:
//      定点求倒数
//  参数:
//      1.RD0: 数据(q7)
//  返回值:
//      1.RD0: 结果（q7）
////////////////////////////////////////////////////////
Sub_AutoField recip_fix_Q7
{
    call_AutoField recip_fix;
    RF_ShiftR2(RD0);
    RF_ShiftR2(RD0);
    RF_ShiftR2(RD0);
    RF_ShiftR2(RD0);
    RF_ShiftR1(RD0);
    Return_AutoField(0);
}





////////////////////////////////////////////////////////
//  名称:
//      power2_fix 修改过的2指数计算 取值范围有限(x>=0) 需要注意输入输出是否在值域内
//  功能:
//      定点求Power = 2^(RD0)
//  参数:
//      1.RD0: 数据	(16q8)
//  返回值:
//      1.RD0: 结果（32q8）
////////////////////////////////////////////////////////
Sub_AutoField power2_fix
{
	int x = RD0.m_data;

//建表
    int power2[128] = {
    0x0080,0x0080,
0x0081,0x0082,
0x0082,0x0083,
0x0084,0x0084,
0x0085,0x0086,
0x0087,0x0087,
0x0088,0x0089,
0x008A,0x008A,
0x008B,0x008C,
0x008D,0x008D,
0x008E,0x008F,
0x0090,0x0090,
0x0091,0x0092,
0x0093,0x0094,
0x0094,0x0095,
0x0096,0x0097,
0x0098,0x0099,
0x0099,0x009A,
0x009B,0x009C,
0x009D,0x009E,
0x009E,0x009F,
0x00A0,0x00A1,
0x00A2,0x00A3,
0x00A4,0x00A5,
0x00A5,0x00A6,
0x00A7,0x00A8,
0x00A9,0x00AA,
0x00AB,0x00AC,
0x00AD,0x00AE,
0x00AF,0x00B0,
0x00B1,0x00B2,
0x00B3,0x00B4,
0x00B5,0x00B6,
0x00B6,0x00B7,
0x00B8,0x00B9,
0x00BA,0x00BC,
0x00BD,0x00BE,
0x00BF,0x00C0,
0x00C1,0x00C2,
0x00C3,0x00C4,
0x00C5,0x00C6,
0x00C7,0x00C8,
0x00C9,0x00CA,
0x00CB,0x00CD,
0x00CE,0x00CF,
0x00D0,0x00D1,
0x00D2,0x00D3,
0x00D4,0x00D6,
0x00D7,0x00D8,
0x00D9,0x00DA,
0x00DB,0x00DD,
0x00DE,0x00DF,
0x00E0,0x00E2,
0x00E3,0x00E4,
0x00E5,0x00E6,
0x00E8,0x00E9,
0x00EA,0x00EC,
0x00ED,0x00EE,
0x00EF,0x00F1,
0x00F2,0x00F3,
0x00F5,0x00F6,
0x00F7,0x00F9,
0x00FA,0x00FB,
0x00FD,0x00FE };

//x=xh+xl，2^x=(x^xh)*(x^xl)
    x = x + 1;
    int xl = (x & 0xff) >> 1;
    int xh = ((x & 0xff00) >> 8) + 1;
    RD1 = 2 ^ xh;		//x^xh
    RD0 = power2[xl];	//x^xl查表
    long long y = RD1.m_data;
    y = RD0.m_data * y;
    RD0 = y >> 8;
    Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  名称:
//      log2_fix
//  功能:
//      定点求log2
//  参数:
//      1.RD0: 数据 
//  返回值:
//      1.RD0: 结果（q15）
////////////////////////////////////////////////////////
Sub_AutoField log2_fix
{
	int x = RD0.m_data;

//建表
    int Log2[128] = { 
0xFFFF8000,0xFFFF8170,
0xFFFF82DD,0xFFFF8448,
0xFFFF85AF,0xFFFF8714,
0xFFFF8876,0xFFFF89D6,
0xFFFF8B32,0xFFFF8C8D,
0xFFFF8DE5,0xFFFF8F3A,
0xFFFF908D,0xFFFF91DD,
0xFFFF932B,0xFFFF9477,
0xFFFF95C1,0xFFFF9708,
0xFFFF984D,0xFFFF998F,
0xFFFF9AD0,0xFFFF9C0E,
0xFFFF9D4A,0xFFFF9E85,
0xFFFF9FBD,0xFFFFA0F3,
0xFFFFA227,0xFFFFA359,
0xFFFFA489,0xFFFFA5B7,
0xFFFFA6E3,0xFFFFA80D,
0xFFFFA935,0xFFFFAA5C,
0xFFFFAB81,0xFFFFACA4,
0xFFFFADC5,0xFFFFAEE4,
0xFFFFB002,0xFFFFB11E,
0xFFFFB238,0xFFFFB351,
0xFFFFB467,0xFFFFB57D,
0xFFFFB690,0xFFFFB7A2,
0xFFFFB8B3,0xFFFFB9C2,
0xFFFFBACF,0xFFFFBBDB,
0xFFFFBCE5,0xFFFFBDEE,
0xFFFFBEF6,0xFFFFBFFB,
0xFFFFC100,0xFFFFC203,
0xFFFFC305,0xFFFFC405,
0xFFFFC504,0xFFFFC601,
0xFFFFC6FD,0xFFFFC7F8,
0xFFFFC8F2,0xFFFFC9EA,
0xFFFFCAE1,0xFFFFCBD6,
0xFFFFCCCA,0xFFFFCDBD,
0xFFFFCEAF,0xFFFFCFA0,
0xFFFFD08F,0xFFFFD17D,
0xFFFFD26A,0xFFFFD356,
0xFFFFD441,0xFFFFD52A,
0xFFFFD613,0xFFFFD6FA,
0xFFFFD7E0,0xFFFFD8C5,
0xFFFFD9A9,0xFFFFDA8B,
0xFFFFDB6D,0xFFFFDC4D,
0xFFFFDD2D,0xFFFFDE0B,
0xFFFFDEE9,0xFFFFDFC5,
0xFFFFE0A1,0xFFFFE17B,
0xFFFFE254,0xFFFFE32D,
0xFFFFE404,0xFFFFE4DA,
0xFFFFE5B0,0xFFFFE684,
0xFFFFE758,0xFFFFE82A,
0xFFFFE8FC,0xFFFFE9CD,
0xFFFFEA9D,0xFFFFEB6C,
0xFFFFEC3A,0xFFFFED07,
0xFFFFEDD3,0xFFFFEE9E,
0xFFFFEF69,0xFFFFF032,
0xFFFFF0FB,0xFFFFF1C3,
0xFFFFF28A,0xFFFFF350,
0xFFFFF415,0xFFFFF4DA,
0xFFFFF59E,0xFFFFF661,
0xFFFFF723,0xFFFFF7E4,
0xFFFFF8A5,0xFFFFF965,
0xFFFFFA24,0xFFFFFAE2,
0xFFFFFB9F,0xFFFFFC5C,
0xFFFFFD18,0xFFFFFDD3,
0xFFFFFE8E,0xFFFFFF47
	};

    if (x == 0)
    {
        Return_AutoField(0);
    }

	//x=2^e*m
	int e = log2(x) + 1;
	int m;

	if (e > 8)
	{
		//M = (x >> (E-index)) - N
		x = x >> (e - 8);
		m = x - 128;
	}
	else if (e < 8)
	{
		//M = (x << (index - E)) - N;
		x = x << (8 - e);
		m = x - 128;
	}
	else
        m = x - 128;

	//查表
	x = e << 15;
	RD0 = Log2[m];	//查表
	RD0 += x;

    Return_AutoField(0);

}

////////////////////////////////////////////////////////
//  函数名称:
//      Sequ_Recip_Fix_q31_L16Cut
//  函数功能:
//      定点求序列倒数;
//  输入参数:
//      RA0:输入序列指针,高16位有效,低16位为0,必为正数(相当于q16);
//      RD0:序列长度(单位:Dword);
//  输出参数:
//      RA1:输出序列指针，32位有效。(q=-31，调用对应q31输出的a,b表);
//  注意事项:
//      1. 输入值越小绝对误差越大;
//      2. 输入小于16时,输出限幅,输出最大值0x7FFFFFFF;
//      3. 此函数为针对维纳滤波的特殊格式,不可混用;
//      4. a表的基址,地址RN_XRAM5;
//      5. b表的基址,地址RN_XRAM6;
//      6. a,b表和调用初始化表函数总共占码点: 2594 Bytes;
//      7. call Init_Sequ_Recip_Table_q31_L16Cut,初始化赋值求倒数函数中的 a,b 表,一次即可;
////////////////////////////////////////////////////////
Sub_AutoField Sequ_Recip_Fix_q31_L16Cut
{
	int A[256] = {0xFFFFE000,
0xFFFFFE1E,
0xFFFFFF59,
0xFFFFFFAC,
0xFFFFFFCD,
0xFFFFFFDE,
0xFFFFFFE8,
0xFFFFFFEE,
0xFFFFFFF2,
0xFFFFFFF5,
0xFFFFFFF7,
0xFFFFFFF8,
0xFFFFFFF9,
0xFFFFFFFA,
0xFFFFFFFB,
0xFFFFFFFC,
0xFFFFFFFC,
0xFFFFFFFD,
0xFFFFFFFD,
0xFFFFFFFD,
0xFFFFFFFE,
0xFFFFFFFE,
0xFFFFFFFE,
0xFFFFFFFE,
0xFFFFFFFE,
0xFFFFFFFE,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0xFFFFFFFF,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
};
	int B[256] = { 0x04000000,
0x00787878,
0x003EB1A2,
0x002A3A10,
0x001FCD1E,
0x00197E68,
0x00154521,
0x00123EB8,
0x000FF8E7,
0x000E33D7,
0x000CC915,
0x000BA017,
0x000AA87B,
0x0009D6E3,
0x0009232E,
0x00088765,
0x0007FF0F,
0x000786BE,
0x00071BC8,
0x0006BC10,
0x000665EA,
0x000617F5,
0x0005D116,
0x00059060,
0x0005550D,
0x00051E78,
0x0004EC15,
0x0004BD6E,
0x0004921B,
0x000469C5,
0x0004441F,
0x000420E6,
0x0003FFE1,
0x0003E0DC,
0x0003C3AA,
0x0003A823,
0x00038E23,
0x0003758B,
0x00035E3E,
0x00034823,
0x00033323,
0x00031F29,
0x00030C23,
0x0002F9FF,
0x0002E8AE,
0x0002D822,
0x0002C84F,
0x0002B927,
0x0002AAA1,
0x00029CB3,
0x00028F54,
0x0002827B,
0x00027620,
0x00026A3D,
0x00025ECA,
0x000253C2,
0x0002491F,
0x00023EDB,
0x000234F2,
0x00022B5F,
0x0002221D,
0x0002192A,
0x00021080,
0x0002081C,
0x0001FFFC,
0x0001F81C,
0x0001F079,
0x0001E910,
0x0001E1DF,
0x0001DAE3,
0x0001D41A,
0x0001CD83,
0x0001C71A,
0x0001C0DE,
0x0001BACD,
0x0001B4E6,
0x0001AF26,
0x0001A98D,
0x0001A418,
0x00019EC7,
0x00019998,
0x00019489,
0x00018F9A,
0x00018ACA,
0x00018617,
0x00018180,
0x00017D04,
0x000178A3,
0x0001745C,
0x0001702D,
0x00016C15,
0x00016815,
0x0001642B,
0x00016057,
0x00015C97,
0x000158EC,
0x00015554,
0x000151CF,
0x00014E5D,
0x00014AFC,
0x000147AD,
0x0001446F,
0x00014140,
0x00013E22,
0x00013B13,
0x00013813,
0x00013521,
0x0001323D,
0x00012F67,
0x00012C9F,
0x000129E3,
0x00012734,
0x00012492,
0x000121FB,
0x00011F70,
0x00011CF0,
0x00011A7B,
0x00011811,
0x000115B1,
0x0001135C,
0x00011110,
0x00010ECF,
0x00010C97,
0x00010A68,
0x00010842,
0x00010624,
0x00010410,
0x00010204,
0x00010000,
0x0000FE03,
0x0000FC0F,
0x0000FA23,
0x0000F83E,
0x0000F660,
0x0000F489,
0x0000F2B9,
0x0000F0F1,
0x0000EF2E,
0x0000ED73,
0x0000EBBD,
0x0000EA0E,
0x0000E865,
0x0000E6C2,
0x0000E525,
0x0000E38E,
0x0000E1FC,
0x0000E070,
0x0000DEE9,
0x0000DD67,
0x0000DBEB,
0x0000DA74,
0x0000D901,
0x0000D794,
0x0000D62B,
0x0000D4C7,
0x0000D368,
0x0000D20D,
0x0000D0B6,
0x0000CF64,
0x0000CE16,
0x0000CCCD,
0x0000CB87,
0x0000CA45,
0x0000C908,
0x0000C7CE,
0x0000C698,
0x0000C566,
0x0000C437,
0x0000C30C,
0x0000C1E5,
0x0000C0C1,
0x0000BFA0,
0x0000BE83,
0x0000BD69,
0x0000BC52,
0x0000BB3F,
0x0000BA2E,
0x0000B921,
0x0000B817,
0x0000B710,
0x0000B60B,
0x0000B50A,
0x0000B40B,
0x0000B30F,
0x0000B216,
0x0000B120,
0x0000B02C,
0x0000AF3B,
0x0000AE4C,
0x0000AD60,
0x0000AC76,
0x0000AB8F,
0x0000AAAB,
0x0000A9C8,
0x0000A8E8,
0x0000A80A,
0x0000A72F,
0x0000A656,
0x0000A57F,
0x0000A4AA,
0x0000A3D7,
0x0000A306,
0x0000A238,
0x0000A16B,
0x0000A0A1,
0x00009FD8,
0x00009F11,
0x00009E4D,
0x00009D8A,
0x00009CC9,
0x00009C0A,
0x00009B4C,
0x00009A91,
0x000099D7,
0x0000991F,
0x00009869,
0x000097B4,
0x00009701,
0x00009650,
0x000095A0,
0x000094F2,
0x00009445,
0x0000939A,
0x000092F1,
0x00009249,
0x000091A3,
0x000090FE,
0x0000905A,
0x00008FB8,
0x00008F17,
0x00008E78,
0x00008DDA,
0x00008D3E,
0x00008CA3,
0x00008C09,
0x00008B70,
0x00008AD9,
0x00008A43,
0x000089AE,
0x0000891B,
0x00008888,
0x000087F7,
0x00008768,
0x000086D9,
0x0000864B,
0x000085BF,
0x00008534,
0x000084AA,
0x00008421,
0x00008399,
0x00008312,
0x0000828D,
0x00008208,
0x00008184,
0x00008102,
0x00008080,
};

	int len = RD0.m_data;
    int a, b;
    for (int i = 1; i < len; i++)
    {
        RD0 = GET_M(RA0 + i * MMU_BASE);
        RD0 = RD0 >> 24;
        RD0 &= 0xff;
        if (RD0 == 0) goto Sequ_Recip_Fix_q31_L16Cut_00;
        //高8位不为0，结果为ax+b
        //以高8位为地址查表
        a = A[RD0.m_data];
        b = B[RD0.m_data];
        RD0 = GET_M(RA0 + i * MMU_BASE);
        RD0 = RD0 >> 16;
        RD0 &= 0xff;
        RD1 = a;
        call _Rs_Multi;
        RD0 += b;
        SET_M(RA1 + i * MMU_BASE, RD0);
        goto L_Sequ_Recip_Fix_q31_L16Cut_3;

    Sequ_Recip_Fix_q31_L16Cut_00:
        //高8位为0，结果为b
        RD0 = GET_M(RA0 + i * MMU_BASE);
        RD0 = RD0 >> 16;
        RD0 &= 0xff;
        if (RD0 < 17) goto L_Sequ_Recip_Fix_q31_L16Cut_01;
        RD0 = 0x7fffffff;                                                           // 输入小于等于16，位数溢出，输出最大值
        SET_M(RA1 + i * MMU_BASE, RD0);
        goto L_Sequ_Recip_Fix_q31_L16Cut_3;

    L_Sequ_Recip_Fix_q31_L16Cut_01:
        b = B[RD0.m_data];
        RD0 = b << 8;
        SET_M(RA1 + i * MMU_BASE, RD0);

    L_Sequ_Recip_Fix_q31_L16Cut_3:
        a = 0;


    }
    Return_AutoField(0);


}