#include "cpu.h"

CCpu _cpu;



// 循环移位n位,当n>0时左移n,当n<0时右移|n|位
// data为int数据，支持正数和负数
void CCpu::RF_RotateX(CReg& data, int n)
{
    data = (data.m_data << n) | ((unsigned int)data.m_data >> (32 - n));
}

// (无符号)算术移位n位,当n>0时左移n,当n<0时右移|n|位
// data为int数据，支持正数和负数
void CCpu::RF_ShiftX(CReg& data, int n)
{
    if (n >= 0)
    {
        data = (unsigned int)data.m_data << n;
    }
    else
    {
        data = (unsigned int)data.m_data >> (-n);
    }
}


// (32位有符号)算术移位n位,当n>0时左移n,当n<0时右移|n|位
// data为int数据，支持正数和负数
void CCpu::RF_ShiftX_Sign32(CReg& data, int n)
{
    if (n >= 0)
    {
        data = data.m_data << n;
    }
    else
    {
        data = data.m_data >> (-n);
    }
}

// (两个16位有符号)算术移位n位,当n>0时左移n,当n<0时右移|n|位
// data的高低16位各为short数据[H16, L16] 同时移位
void CCpu::RF_ShiftX_Sign16(CReg& data, int n)
{
    CReg temp = data;
    RF_ShiftX_Sign32(temp, -16);
    short H16 = (short)temp.m_data;
    temp = data;
    RF_ShiftX_Sign32(temp, 16);
    RF_ShiftX_Sign32(temp, -16);
    short L16 = (short)temp.m_data;

    if (n >= 0)
    {
        H16 = H16 << n;
        L16 = L16 << n;
    }
    else
    {
        H16 = H16 >> (-n);
        L16 = L16 >> (-n);
    }

    data = ((int)H16 << 16) | ((int)L16 & 0xFFFF);
}


void CCpu::RF_Reverse(CReg& data)
{
    bitField_union bf_u;
    bf_u.data = data.m_data;

    bitField_union bf_u_rev;
    bf_u_rev.bf.bit0 = bf_u.bf.bit31;
    bf_u_rev.bf.bit1 = bf_u.bf.bit30;
    bf_u_rev.bf.bit2 = bf_u.bf.bit29;
    bf_u_rev.bf.bit3 = bf_u.bf.bit28;
    bf_u_rev.bf.bit4 = bf_u.bf.bit27;
    bf_u_rev.bf.bit5 = bf_u.bf.bit26;
    bf_u_rev.bf.bit6 = bf_u.bf.bit25;
    bf_u_rev.bf.bit7 = bf_u.bf.bit24;
    bf_u_rev.bf.bit8 = bf_u.bf.bit23;
    bf_u_rev.bf.bit9 = bf_u.bf.bit22;
    bf_u_rev.bf.bit10 = bf_u.bf.bit21;
    bf_u_rev.bf.bit11 = bf_u.bf.bit20;
    bf_u_rev.bf.bit12 = bf_u.bf.bit19;
    bf_u_rev.bf.bit13 = bf_u.bf.bit18;
    bf_u_rev.bf.bit14 = bf_u.bf.bit17;
    bf_u_rev.bf.bit15 = bf_u.bf.bit16;
    bf_u_rev.bf.bit16 = bf_u.bf.bit15;
    bf_u_rev.bf.bit17 = bf_u.bf.bit14;
    bf_u_rev.bf.bit18 = bf_u.bf.bit13;
    bf_u_rev.bf.bit19 = bf_u.bf.bit12;
    bf_u_rev.bf.bit20 = bf_u.bf.bit11;
    bf_u_rev.bf.bit21 = bf_u.bf.bit10;
    bf_u_rev.bf.bit22 = bf_u.bf.bit9;
    bf_u_rev.bf.bit23 = bf_u.bf.bit8;
    bf_u_rev.bf.bit24 = bf_u.bf.bit7;
    bf_u_rev.bf.bit25 = bf_u.bf.bit6;
    bf_u_rev.bf.bit26 = bf_u.bf.bit5;
    bf_u_rev.bf.bit27 = bf_u.bf.bit4;
    bf_u_rev.bf.bit28 = bf_u.bf.bit3;
    bf_u_rev.bf.bit29 = bf_u.bf.bit2;
    bf_u_rev.bf.bit30 = bf_u.bf.bit1;
    bf_u_rev.bf.bit31 = bf_u.bf.bit0;
    data = bf_u_rev.data;

}

void CCpu::RF_LSB2MSB(CReg& data)
{
    bitField_union bf_u;
    bf_u.data = data.m_data;

    bitField_union bf_u_rev;
    bf_u_rev.bytef.H8 = bf_u.bytef.L8;
    bf_u_rev.bytef.MH8 = bf_u.bytef.ML8;
    bf_u_rev.bytef.ML8 = bf_u.bytef.MH8;
    bf_u_rev.bytef.L8 = bf_u.bytef.H8;

    data = bf_u_rev.data;
}

void CCpu::RF_GetH4(CReg& data)
{
    bitField_union bf_u;
    bf_u.data = data.m_data;
    data = bf_u.u4f.H4;
}

void CCpu::RF_ExchangeL16(CReg& data)
{
    bitField_union bf_u;
    bf_u.data = data.m_data;

    bitField_union bf_u_rev;
    bf_u_rev.bytef.H8 = bf_u.bytef.H8;
    bf_u_rev.bytef.MH8 = bf_u.bytef.MH8;
    bf_u_rev.bytef.ML8 = bf_u.bytef.L8;
    bf_u_rev.bytef.L8 = bf_u.bytef.ML8;
    data = bf_u_rev.data;
}

void CCpu::RF_Parity(CReg& data)
{
    bitField_union bf_u;
    bf_u.data = data.m_data;

    int rst = 0;
    rst += parity_even_byte(bf_u.bytef.H8);
    rst += parity_even_byte(bf_u.bytef.MH8);
    rst += parity_even_byte(bf_u.bytef.ML8);
    rst += parity_even_byte(bf_u.bytef.L8);
    data = (rst % 2);
}

// 计算一个DWORD低8位的偶校验
int CCpu::parity_even_byte(DWORD data)
{
    bitField_union bf_u;
    bf_u.data_dw = data;

    int rst = 0;
    rst += bf_u.bf.bit0;
    rst += bf_u.bf.bit1;
    rst += bf_u.bf.bit2;
    rst += bf_u.bf.bit3;
    rst += bf_u.bf.bit4;
    rst += bf_u.bf.bit5;
    rst += bf_u.bf.bit6;
    rst += bf_u.bf.bit7;

    return (rst%2);
}


void CCpu::RF_log(CReg& data)
{
    bitField_union bf_u;
    bf_u.data_dw = data.m_data;
    int rst = 0;

    if (log2_byte(bf_u.bytef.H8, rst))
    {
        data = rst + 24;
    }
    else if (log2_byte(bf_u.bytef.MH8, rst))
    {
        data = rst + 16;
    }
    else if (log2_byte(bf_u.bytef.ML8, rst))
    {
        data = rst + 8;
    }
    else if (log2_byte(bf_u.bytef.L8, rst))
    {
        data = rst;
    }
    else
    {
        data = 0;
    }


}

void CCpu::rd0_SetByteL8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.L8 = 0xFF;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_SetByteML8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.ML8 = 0xFF;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_SetByteMH8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.MH8 = 0xFF;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_SetByteL24()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.L8 = 0xFF;
    bf_u.bytef.ML8 = 0xFF;
    bf_u.bytef.MH8 = 0xFF;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_SetByteH8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.H8 = 0xFF;
    rd0.SetByBitField(bf_u);

}

void CCpu::rd0_SetByteH8L8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.L8 = 0xFF;
    bf_u.bytef.H8 = 0xFF;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_SetByteH8L16()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.L8 = 0xFF;
    bf_u.bytef.ML8 = 0xFF;
    bf_u.bytef.H8 = 0xFF;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_SetByteH16()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.MH8 = 0xFF;
    bf_u.bytef.H8 = 0xFF;
    rd0.SetByBitField(bf_u);

}

void CCpu::rd0_SetByteH16L8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.L8 = 0xFF;
    bf_u.bytef.MH8 = 0xFF;
    bf_u.bytef.H8 = 0xFF;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_SetByteH24()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.ML8 = 0xFF;
    bf_u.bytef.MH8 = 0xFF;
    bf_u.bytef.H8 = 0xFF;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_ClrByteL8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.L8 = 0;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_ClrByteML8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.ML8 = 0;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_ClrByteMH8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.MH8 = 0;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_ClrByteMH8L8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.L8 = 0;
    bf_u.bytef.MH8 = 0;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_ClrByteMH8ML8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.ML8 = 0;
    bf_u.bytef.MH8 = 0;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_ClrByteL24()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.L8 = 0;
    bf_u.bytef.ML8 = 0;
    bf_u.bytef.MH8 = 0;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_ClrByteH8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.H8 = 0;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_ClrByteH8L8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.L8 = 0;
    bf_u.bytef.H8 = 0;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_ClrByteH8ML8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.ML8 = 0;
    bf_u.bytef.H8 = 0;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_ClrByteH8L16()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.L8 = 0;
    bf_u.bytef.ML8 = 0;
    bf_u.bytef.H8 = 0;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_ClrByteH16()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.MH8 = 0;
    bf_u.bytef.H8 = 0;
    rd0.SetByBitField(bf_u);

}

void CCpu::rd0_ClrByteH16L8()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.L8 = 0;
    bf_u.bytef.MH8 = 0;
    bf_u.bytef.H8 = 0;
    rd0.SetByBitField(bf_u);
}

void CCpu::rd0_ClrByteH24()
{
    bitField_union bf_u = rd0.GetBitField();
    bf_u.bytef.ML8 = 0;
    bf_u.bytef.MH8 = 0;
    bf_u.bytef.H8 = 0;
    rd0.SetByBitField(bf_u);
}

// 符号位扩展
void CCpu::rd0_SignExtL8()
{
    bitField_union bf_u = rd0.GetBitField();
    if (bf_u.bf.bit7 == 1)
    {
        rd0_SetByteH24();
    }
    else
    {
        rd0_ClrByteH24();
    }
}

void CCpu::rd0_SignExtL16()
{
    bitField_union bf_u = rd0.GetBitField();
    if (bf_u.bf.bit15 == 1)
    {
        rd0_SetByteH16();
    }
    else
    {
        rd0_ClrByteH16();
    }
}

void CCpu::rd0_SignExtL24()
{
    bitField_union bf_u = rd0.GetBitField();
    if (bf_u.bf.bit23 == 1)
    {
        rd0_SetByteH8();
    }
    else
    {
        rd0_ClrByteH8();
    }
}

// 计算一个DWORD低8位的log2,规定0的log2为0
// 如果全0则返回false，有至少一个1返回true
bool CCpu::log2_byte(DWORD data, int& rst)
{
    rst = 0;

    if (data == 0)
    {
        return false;
    }

    bitField_union bf_u;
    bf_u.data_dw = data;

    if (bf_u.bf.bit7 == 1) rst = 7;
    else if (bf_u.bf.bit6 == 1) rst = 6;
    else if (bf_u.bf.bit5 == 1) rst = 5;
    else if (bf_u.bf.bit4 == 1) rst = 4;
    else if (bf_u.bf.bit3 == 1) rst = 3;
    else if (bf_u.bf.bit2 == 1) rst = 2;
    else if (bf_u.bf.bit1 == 1) rst = 1;
    else rst = 0;

    return true;
}