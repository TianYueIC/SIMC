#include "debug.h"

CData_io debug_data = CData_io("", "./user/data/debug_data_out.bin");     // 数据IO类初始化
CData_io debug_steam = CData_io("", "./user/data/debug_steam_out.bin");     // 数据IO类初始化
CData_io debug_vector = CData_io("", "./user/data/debug_vector_out.bin");     // 数据IO类初始化


//////////////////////////////////////////////////////////////////////////
//  名称:
//      Export_Sound_16bit
//  功能:
//      按16bit格式导出数字音频
//  参数:
//      1.RD0:数据首地址
//      2.RD1:数据长度(单位：字节，满足4的整倍数)
//  返回值：
//      无
//////////////////////////////////////////////////////////////////////////
Sub_AutoField Export_Sound_16bit
{
    RA0 = RD0;
    debug_steam.outData_Steam(RA0, RD1.m_data);
    Return_AutoField(0);
}


//////////////////////////////////////////////////////////////////////////
//  名称:
//      Export_Data_32bit
//  功能:
//      按32bit格式导出标量调试数据
//  参数:
//      1.RD0:数据首地址
//      2.RD1:数据长度(单位：字节，满足4的整倍数)
//  返回值：
//      无
//////////////////////////////////////////////////////////////////////////
Sub_AutoField Export_Data_32bit
{

    RA0 = RD0;
    debug_data.outData_Data32(RA0, RD1.m_data);

    Return_AutoField(0);
}

//////////////////////////////////////////////////////////////////////////
//  名称:
//      Export_Vector_32bit
//  功能:
//      按32bit格式导出关键数据(向量)，
//  参数:
//      1.RD0:数据首地址
//      2.RD1:数据长度(单位：字节，满足4的整倍数)
//  返回值：
//      无
//////////////////////////////////////////////////////////////////////////
Sub_AutoField Export_Vector_32bit
{
    RA0 = RD0;
    debug_vector.outData_Data32(RA0, RD1.m_data);

    Return_AutoField(0);
}