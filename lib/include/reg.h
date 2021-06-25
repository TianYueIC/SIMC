#pragma once
#include "type.h"

class CReg
{
public:

    // 构造
    CReg();
    CReg(int data);
    CReg(int data, int ID)
    {
        m_data = data;
        m_ID = ID;
    }

    // 赋值
    void operator = (signed data);      // RDx = 立即数
    void operator = (CReg data);        // RDx = RD0

    // 双目运算
    CReg operator +(signed data);       // RDy = RDx + 立即数
    CReg operator -(signed data);       // RDy = RDx - 立即数

    // 运算后覆盖原值
    void operator += (signed data);
    void operator += (CReg data);
    void operator -= (signed data);
    void operator -= (CReg data);
    void operator &= (signed data);
    void operator &= (CReg data);
    void operator |= (signed data);
    void operator |= (CReg data);
    void operator ^= (signed data);
    void operator ^= (CReg data);

    CReg operator ++ ();
    CReg operator ++ (int);
    CReg operator -- ();
    CReg operator -- (int);


    // 逻辑运算
    bool operator >  (signed data);
    bool operator <  (signed data);
    bool operator ==  (signed data);
    bool operator !=  (signed data);
    bool operator >=  (signed data);


    // 特殊的(用户程序不允许)
    int operator -();                   // -RDx
    int operator & (signed data);       // RDx & 立即数
    CReg operator +(CReg data);         // RDz = RDx + RDy
    CReg operator >>(CReg data);        // RDz = RDx >> RDy
    CReg operator <<(CReg data);        // RDz = RDx << RDy
    int operator >> (signed data);
    int operator << (signed data);
    void operator >>= (signed data);
    void operator <<= (signed data);



public:
    int m_data;
    static const int ID_MEM = 0x40;     // 特殊寄存器

    bitField_union GetBitField()
    {
        bitField_union bf;
        bf.data = m_data;
        return bf;
    }

    void SetByBitField(bitField_union bf)
    {
        m_data = bf.data;
    }

    static bitField_union GetBitFieldRQ()
    {
        bitField_union bf;
        bf.data = m_rq;
        return bf;
    }


    static oper_status GetOperStatus()
    {
        return m_oper_status;
    }

    // 特殊用于ID_MEM寄存器
    void setMemOffset(int offset)
    {
        m_offset = offset;
    }

private:
    static int m_rq;
    int m_offset;
    int m_ID = 0xffffffff;                // ID是RD,RA的身份标识,如RD0~RD15的ID为0~15,RA0~RA7的ID为24~31
    static oper_status m_oper_status;

    static bool isAddOverflow(int x, int y);

    static bool isAddOverflowH(int x, int y);

    bool isMemID()
    {
        return (m_ID == ID_MEM);
    }

    bool isRdID()
    {
        return (m_ID >= 0 && m_ID <= 15);
    }

    bool isRaID()
    {
        return (m_ID >= 24 && m_ID <= 31);
    }
};