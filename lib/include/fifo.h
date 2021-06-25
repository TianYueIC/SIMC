#pragma once
#include "cpu.h"




#define push_AutoField  fifo_AutoField.push_Field()
#define pop_AutoField   fifo_AutoField.pop_Field()


class CFifo
{
public:

    CFifo(CCpu* pcpu)
    {
        m_cpu = pcpu;
    }

    void push_Field()
    {
        push_1(m_cpu->rd2);
        push_1(m_cpu->rd3);
        push_1(m_cpu->ra0);
        push_1(m_cpu->ra1);
    }
    void pop_Field()
    {
        pop_1(m_cpu->ra1);
        pop_1(m_cpu->ra0);
        pop_1(m_cpu->rd3);
        pop_1(m_cpu->rd2);
    }

private:
    const static int layerNum = 8;      // 嵌套最深的深度
    const static int regNum = 4;        // RD2 RD3 RA0 RA1
    int curIndex = 0;                   // 当前深度的位置，初值为0，超过buffer即为溢出
    int buffer[layerNum * regNum] = { 0 };
    CCpu* m_cpu = 0;

    void push_1(CReg x)
    {
        *(buffer + curIndex) = x.m_data;
        curIndex++;
    }

    void pop_1(CReg& x)
    {
        curIndex--;
        x.m_data = *(buffer + curIndex);
    }

};


extern CFifo fifo_AutoField;
