#include "fir.h"
#include "fir_base.h"

extern void delay8(CReg addin, CReg addout);
extern void delay1(CReg addin, CReg addout);
extern void delay9(CReg addin, CReg addout);



//////////////////////////////////////////////////////////
////  名称:
////      FIR_PATH3_HPInit_HP1
////  功能:
////      设置fir滤波器系数
////  参数:
////      无
////  返回值:
////      无
//////////////////////////////////////////////////////////
//Sub_AutoField FIR_PATH3_HPInit_HP1
//{
//    RSP -= 20 * MMU_BASE;
//    RA0 = RSP;
//
//    // fir系数
//    #define a0          0x862a
//    #define a1          0x03de
//    #define a2          0x0d2a
//    #define a3          0x80be
//    #define a4          0x9942
//    #define a5          0x8c12
//    #define a6          0x393e
//    #define a7          0x7fff
//    #define a8          0x7fff
//    #define a9          0x393e  
//    #define a10         0x8c12  
//    #define a11         0x9942  
//    #define a12         0x80be  
//    #define a13         0x0d2a  
//    #define a14         0x03de  
//    #define a15         0x862a  
//    #define c0          0x0000   //占位
//
//
//    //以下填入滤波器系数
//    RD0 = a0;
//    M[RA0++] = RD0;
//    RD0 = a1;
//    M[RA0++] = RD0;
//    RD0 = a2;
//    M[RA0++] = RD0;
//    RD0 = a3;
//    M[RA0++] = RD0;
//    RD0 = a4;
//    M[RA0++] = RD0;
//    RD0 = a5;
//    M[RA0++] = RD0;
//    RD0 = a6;
//    M[RA0++] = RD0;
//    RD0 = a7;
//    M[RA0++] = RD0;
//    RD0 = a8;
//    M[RA0++] = RD0;
//    RD0 = a9;
//    M[RA0++] = RD0;
//    RD0 = a10;
//    M[RA0++] = RD0;
//    RD0 = a11;
//    M[RA0++] = RD0;
//    RD0 = a12;
//    M[RA0++] = RD0;
//    RD0 = a13;
//    M[RA0++] = RD0;
//    RD0 = a14;
//    M[RA0++] = RD0;
//    RD0 = a15;
//    M[RA0++] = RD0;
//    RD0 = c0;
//    M[RA0++] = RD0;
//    M[RA0++] = RD0;
//    M[RA0++] = RD0;
//    M[RA0++] = RD0;
//
//    RA0 = RSP;
//    RD0 = 0;                        
//    call_AutoField FIR1_SetPara;
//
//
//
//    RSP += 20 * MMU_BASE;
//    Return_AutoField(0 * MMU_BASE);
//}

////////////////////////////////////////////////////////
//  名称:
//      FIR_PATH3_HPInit_HP1
//  功能:
//      设置fir滤波器系数
//  参数:
//      无
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField FIR_PATH3_HPInit_HP1
{
    RSP -= 20 * MMU_BASE;
    RA0 = RSP;

    // fir系数
    #define a0          102
    #define a1          32945
    #define a2          396
    #define a3          33568
    #define a4          1438
    #define a5          35186
    #define a6          4011
    #define a7          40075
    #define a8          22993
    #define a9          18268  
    #define a10         38132  
    #define a11         2471  
    #define a12         33961  
    #define a13         541  
    #define a14         32997  
    #define a15         0  
    #define c0          0x0000  //占位


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
    RD0 = c0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;

    RA0 = RSP;
    RD0 = 0;
    call_AutoField FIR1_SetPara;
 
    RA0 = RSP;
    RD0 = 1;
    call_AutoField FIR1_SetPara;



    RSP += 20 * MMU_BASE;
    Return_AutoField(0 * MMU_BASE);
}

////////////////////////////////////////////////////////
//  函数名称:
//      FIR1_SetPara
//  函数功能:
//      FIR1配置系数，模拟设置fir系数操作
//  形参:
//      1.RA0:系数序列
//      2.RD0:配置第几个bank,取值范围0~3
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField FIR1_SetPara
{
    int bank_id = RD0.m_data;

    short para[20];

    for (int i = 0; i < 20; i++)
    {
        RD0 = M[RA0++];
        para[i] = RD0.m_data;
    }
    fir1.setPara(bank_id, para);

    Return_AutoField(0 * MMU_BASE);
}

////////////////////////////////////////////////////////
//  名称:
//      _FIR1
//  功能:
//      使用FIR执行滤波
//  参数:
//      1.RA0:输入序列指针，16bit紧凑格式序列
//      2.RA1:输出序列指针，16bit紧凑格式序列
//      3.RD0:序列DWORD个数
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField FIR1
{

    RD1 = RD0;
    RD0 = 0;                        // bank_id,取值范围[0,3]
    call_AutoField FIR1_Filter;

    Return_AutoField(0 * MMU_BASE);
}

////////////////////////////////////////////////////////
//  名称:
//      FIR2
//  功能:
//      使用FIR执行滤波
//  参数:
//      1.RA0:输入序列指针，16bit紧凑格式序列
//      2.RA1:输出序列指针，16bit紧凑格式序列
//      3.RD0:序列DWORD个数
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField FIR2
{

    RD1 = RD0;
    RD0 = 1;                        // bank_id,取值范围[0,3]
    call_AutoField FIR1_Filter;

    Return_AutoField(0 * MMU_BASE);
}

////////////////////////////////////////////////////////
//  函数名称:
//      FIR1_Filter
//  函数功能:
//      FIR滤波核心
//  形参:
//      1.RA0:输入数据序列，16bit紧凑格式序列
//      2.RA1:输出数据序列，16bit紧凑格式序列
//      3.RD1:DWORD个数
//      4.RD0:配置第几个bank,取值范围0~3
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField FIR1_Filter
{
    int bank_id = RD0.m_data;
    int len = RD1.m_data;

	int* data_in = new int[len * 2];

    for (int i = 0; i < len; i ++)
    {
        RD0 = M[RA0++];
        RD1 = *(short*)(&RD0);
        RD1 = RD1 << 8;
		data_in[i * 2] = RD1.m_data;
        RD1 = RD0 >> 16;
        RD1 = RD1 << 8;
		data_in[i * 2 + 1] = RD1.m_data;
    }
	fir1.FirFilterFrame(bank_id, data_in, len * 2);
	for (int i = 0; i < len; i++)
    {
		RD0 = data_in[i * 2];
        RD0 = RD0 >> 8;
		RD1 = data_in[i * 2 + 1];
        RD1 = RD1 << 8;
        RD1 &= 0XFFFF0000;
        M[RA1++] = RD1 + RD0;

    }

    delete[] data_in;
    Return_AutoField(0 * MMU_BASE);
}





////////////////////////////////////////////////////////
//  函数名称:
//      doublemic
//  函数功能:
//      双麦算法
//  形参:
//      1.RN_GRAM_IN:输入数据序列0，16bit紧凑格式序列
//      2.RN_GRAM_IN2:输入数据序列1，16bit紧凑格式序列
//      3.RN_GRAM_OUT:输出序列
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField doublemic
{
	//第一路
	delay8(RN_GRAM_IN, RN_GRAM1);//序列0delay8
	delay1(RN_GRAM_IN2, RN_GRAM2);//序列1delay1

	RD0 = RN_GRAM2;
	RA0 = RD0;
	RA1 = RD0;
	RD0 = 16;
	call_AutoField FIR1;

	RD0 = RN_GRAM1;
	RA0 = RD0;
	RD0 = RN_GRAM2;
	RA1 = RD0;
	RD0 = 16;
	call_AutoField Sub_LMT;//序列减，值在RA0

	////第二路
	//RD0 = RN_GRAM_IN;
	//RA0 = RD0;
	//RA1 = RD0;
	//RD0 = 16;
	//call_AutoField FIR2;

	//delay9(RN_GRAM_IN2, RN_GRAM_IN2);//序列1delay9

	//RD0 = RN_GRAM_IN2;
	//RA0 = RD0;
	//RD0 = RN_GRAM_IN;
	//RA1 = RD0;
	//RD0 = 16;
	//call_AutoField Sub_LMT;//序列减，值在RA0

	//RD0 = RN_GRAM_IN;
	//RA0 = RD0;
	//RA1 = RD0;
	//RD1 = 0x40004000;//该值可变，默认为0
	//RD0 = 16;
	//call_AutoField MultiConstH16L16;//序列乘常量,结果在RA1

	////两路相减
	//RD0 = RN_GRAM1;
	//RA0 = RD0;
	//RD0 = RN_GRAM_IN;
	//RA1 = RD0;
	//RD0 = 16;
	//call_AutoField Sub_LMT;//序列减，值在RA0

	RA0 = RN_GRAM1;
	RA1 = RN_GRAM_OUT;
	for (int i = 0; i < 16; i++)
	{
		RD0 = M[RA0++];
		M[RA1++] = RD0;
	}

	//////x*1.9，与下面方法二选一
	//RD0 = RN_GRAM1;
	//send_para(RD0);
	//RD0 = 0x00f300f3;
	//send_para(RD0);
	//RD0 = RN_GRAM_OUT;
	//send_para(RD0);
	//RD0 = 16;
	//send_para(RD0);
	//call_AutoField MAC_MultiConst16_Q2207;

	////x*0.9+x
	//RD0 = RN_GRAM1;
	//RA0 = RD0;
	//RD0 = RN_GRAM_IN;
	//RA1 = RD0;
	//RD1 = 0x73327332;//该值可变，默认为0
	//RD0 = 16;
	//call_AutoField MultiConstH16L16;//序列乘常量,结果在RA1
	////两路相加
	//RD0 = RN_GRAM1;
	//RA0 = RD0;
	//RD0 = RN_GRAM_IN;
	//RA1 = RD0;
	//RD0 = 16;
	//RD1 = RN_GRAM_IN;
	//call_AutoField Add_LMT;//序列加，值在RA0


	Return_AutoField(0 * MMU_BASE);

}




//时延8个点
void delay8(CReg addin, CReg addout)
{
#define DelayCache0             (RN_GRAM_IN2+32*MMU_BASE)     	
#define DelayCache1             (DelayCache0+4*MMU_BASE)    


	RA0 = addin + 12 * MMU_BASE;
	RA1 = DelayCache1;
	for (int i = 0; i < 4; i++)
	{
		RD0 = M[RA0++];
		M[RA1++] = RD0;
	}
	RA0 = addin;
	RA1 = addout;
	for (int i = 0; i < 12; i++)
	{
		RD0 = M[RA0 + (11 - i) * MMU_BASE];
		M[RA1 + (15 - i) * MMU_BASE] = RD0;
	}
	RA0 = DelayCache0;
	RA1 = addout;
	for (int i = 0; i < 4; i++)
	{
		RD0 = M[RA0++];
		M[RA1++] = RD0;
	}
	RA0 = DelayCache1;
	RA1 = DelayCache0;
	for (int i = 0; i < 4; i++)
	{
		RD0 = M[RA0++];
		M[RA1++] = RD0;
	}

}

//时延1个点
void delay1(CReg addin, CReg addout)
{
#define DelayCache0             (RN_GRAM_IN2+50*MMU_BASE)     	
#define DelayCache1             (DelayCache0+MMU_BASE)     	


	RA0 = DelayCache1;
	RD0 = M[addin + 15 * MMU_BASE];
	RD0 = RD0 >> 16;
	M[RA0] = RD0 & 0xffff;
	RA0 = addin;
	for (int i = 0; i < 15; i++)
	{
		RD0 = M[RA0 + (14 - i) * MMU_BASE];
		RD0 = RD0 >> 16;
		RD0 &= 0xffff;
		RD1 = M[RA0 + (15 - i) * MMU_BASE];
		RD1 = RD1 << 16;
		RD0 += RD1;
		M[addout + (15 - i) * MMU_BASE] = RD0;
	}
	RD0 = M[RA0];
	RD1 = M[DelayCache0];
	RD1 += RD0 << 16;
	M[addout] = RD1;

	RA0 = DelayCache1;
	RA1 = DelayCache0;
	RD0 = M[RA0++];
	M[RA1++] = RD0;

}

//时延9个点
void delay9(CReg addin, CReg addout)
{
#define DelayCache0             (RN_GRAM_IN2+60*MMU_BASE)     	
#define DelayCache1             (DelayCache0+MMU_BASE)     	


	RA0 = DelayCache1;
	RD0 = M[addin + 15 * MMU_BASE];
	RD0 = RD0 >> 16;
	M[RA0] = RD0 & 0xffff;
	RA0 = addin;
	for (int i = 0; i < 15; i++)
	{
		RD0 = M[RA0 + (14 - i) * MMU_BASE];
		RD0 = RD0 >> 16;
		RD0 &= 0xffff;
		RD1 = M[RA0 + (15 - i) * MMU_BASE];
		RD1 = RD1 << 16;
		RD0 += RD1;
		M[addout + (15 - i) * MMU_BASE] = RD0;
	}
	RD0 = M[RA0];
	RD1 = M[DelayCache0];
	RD1 += RD0 << 16;
	M[addout] = RD1;

	RA0 = DelayCache1;
	RA1 = DelayCache0;
	RD0 = M[RA0++];
	M[RA1++] = RD0;
#define DelayCache2             (RN_GRAM_IN2+70*MMU_BASE)     	
#define DelayCache3             (DelayCache2+4*MMU_BASE)    


	RA0 = addout + 12 * MMU_BASE;
	RA1 = DelayCache3;
	for (int i = 0; i < 4; i++)
	{
		RD0 = M[RA0++];
		M[RA1++] = RD0;
	}
	RA0 = addout;
	RA1 = addout;
	for (int i = 0; i < 12; i++)
	{
		RD0 = M[RA0 + (11 - i) * MMU_BASE];
		M[RA1 + (15 - i) * MMU_BASE] = RD0;
	}
	RA0 = DelayCache2;
	RA1 = addout;
	for (int i = 0; i < 4; i++)
	{
		RD0 = M[RA0++];
		M[RA1++] = RD0;
	}
	RA0 = DelayCache3;
	RA1 = DelayCache2;
	for (int i = 0; i < 4; i++)
	{
		RD0 = M[RA0++];
		M[RA1++] = RD0;
	}

}