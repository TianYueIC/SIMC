#include "iir.h"
#include "iir_base.h"

////////////////////////////////////////////////////////
//  名称:
//      IIR_PATH3_HPInit_HP2
//  功能:
//      设置高通滤波器系数
//  参数:
//      无？？？
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField IIR_PATH3_HPInit_HP2
{
    RSP -= 40 * MMU_BASE;
    RA0 = RSP;

    // hp2_cheb1_20_800_0.2_40
    #define b11        0x2000
    #define b12        0xc000
    #define b13        0x2000
    #define b14        0x0000
    #define b15        0x0000
    #define a12        0x3789
    #define a13        0x98bb
    #define a14        0x0000
    #define a15        0x0000
    #define c0         0x0000  

    //增加一段做增益调节，增益*4
    RD0 = 0x7999;
    M[RA0++] = RD0;
    RD0 = 0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;

    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    RD0 = 0x0079;
    M[RA0++] = RD0;

    //增加一段做增益调节，增益*4
    RD0 = 0x7999;
    M[RA0++] = RD0;
    RD0 = 0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;

    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;

    //增加一段做增益调节，增益*4
    RD0 = 0x7999;
    M[RA0++] = RD0;
    RD0 = 0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;

    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;

    //以下填入滤波器系数
    RD0 = b11;
    M[RA0++] = RD0;
    RD0 = b12;
    M[RA0++] = RD0;
    RD0 = b13;
    M[RA0++] = RD0;
    RD0 = b14;
    M[RA0++] = RD0;
    RD0 = b15;
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

    RA0 = RSP;
    RD0 = 0;                        // bank_id,取值范围[0,3]
    call_AutoField IIR1_SetPara;



    RSP += 40 * MMU_BASE;
    Return_AutoField(0 * MMU_BASE);
}

////////////////////////////////////////////////////////
//  名称:
//      IIR_PATH3_HPInit_HP1
//  功能:
//      设置高通滤波器系数
//  参数:
//      无？？？
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField IIR_PATH3_HPInit_HP1
{
    RSP -= 40 * MMU_BASE;
    RA0 = RSP;

    // hp1_butt_120_1500_0.2_35
    #define b11         0x2000
    #define b12         0xe000
    #define b13         0x6000
    #define b14         0xa000
    #define b15         0x0000
    #define a12         0x547e
    #define a13         0xcaf5
    #define a14         0x1650
    #define a15         0x0000
    #define c0          0x0000

    //增加一段做增益调节，增益*4
    RD0 = 0x7fff;
    M[RA0++] = RD0;
    RD0 = 0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;

    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    RD0 = 0x0079;
    M[RA0++] = RD0;

    //增加一段做增益调节，增益*4
    RD0 = 0x7fff;
    M[RA0++] = RD0;
    RD0 = 0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;

    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;

    //增加一段做增益调节，增益*4
    RD0 = 0x7fff;
    M[RA0++] = RD0;
    RD0 = 0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;

    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;
    M[RA0++] = RD0;

    //以下填入滤波器系数
    RD0 = b11;
    M[RA0++] = RD0;
    RD0 = b12;
    M[RA0++] = RD0;
    RD0 = b13;
    M[RA0++] = RD0;
    RD0 = b14;
    M[RA0++] = RD0;
    RD0 = b15;
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

    RA0 = RSP;
    RD0 = 0;                        // bank_id,取值范围[0,3]
    call_AutoField IIR1_SetPara;



    RSP += 40 * MMU_BASE;
    Return_AutoField(0 * MMU_BASE);
}


////////////////////////////////////////////////////////
//  名称:
//      _IIR_PATH3_HP
//  功能:
//      使用IIR2_3执行高通滤波，高通起始频率170Hz，Para0, Data00
//  参数:
//      1.RA0:输入序列指针，16bit紧凑格式序列
//      2.RA1:输出序列指针，16bit紧凑格式序列(out)
//      3.RD0:序列DWORD个数
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField _IIR_PATH3_HP
{

    RD1 = RD0;
    RD0 = 0;                        // bank_id,取值范围[0,3]
    call_AutoField IIR1_Filter;

    //    IIR_PATH3_Disable;
    Return_AutoField(0 * MMU_BASE);
}

////////////////////////////////////////////////////////
//  函数名称:
//      IIR1_SetPara
//  函数功能:
//      IIR1配置系数，模拟IIR_PATH3_HD = RD0的设置iir系数操作
//  形参:
//      1.RA0:系数序列
//      2.RD0:配置第几个bank,取值范围0~3
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField IIR1_SetPara
{
    int bank_id = RD0.m_data;
    
    short para[40];

    for (int i = 0; i < 40; i++)
    {
        RD0 = M[RA0++];
        para[i] = RD0.m_data;
    }
    iir1.setPara(bank_id, para);

    Return_AutoField(0 * MMU_BASE);
}

////////////////////////////////////////////////////////
//  函数名称:
//      IIR1_Filter
//  函数功能:
//      IIR1滤波，模拟_IIR_PATH3_HP这类函数的执行滤波操作
//  形参:
//      1.RA0:输入数据序列，16bit紧凑格式序列
//      2.RA1:输出数据序列，16bit紧凑格式序列
//      3.RD1:DWORD个数
//      4.RD0:配置第几个bank,取值范围0~3
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField IIR1_Filter
{
    int bank_id = RD0.m_data;
    int len = RD1.m_data * 2;

    int* data_in = new int[len];

    for (int i = 0; i < len; i+=2)
    {
        RD0 = M[RA0++];
        RD1 = RD0;
        RF_GetL16(RD0);
        RD0_SignExtL16;
        data_in[i] = RD0.m_data;
        RD0 = RD1;
        RF_GetH16(RD0);
        RD0_SignExtL16;
        data_in[i+1] = RD0.m_data;
    }
    iir1.iirFilterFrame(bank_id, data_in, len);
    for (int i = 0; i < len; i += 2)
    {
        RD1.m_data = data_in[i];
        RF_GetL16(RD1);
        RD0.m_data = data_in[i+1];
        RF_GetL16(RD0);
        RF_RotateL16(RD0);
        RD0 |= RD1;
        M[RA1++] = RD0;
    }

    delete[] data_in;
    Return_AutoField(0 * MMU_BASE);
}