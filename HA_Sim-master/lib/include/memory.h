#pragma once
#include "cpu.h"
#include "fifo.h"

#define sub void
#define Sub void
#define Sub_AutoField void
#define call            send_para(RD0);
#define call_AutoField  push_AutoField;
#define Return(x)       RSP += (x+MMU_BASE);\
                        return
#define Return_AutoField(x)       RSP += x;\
                        pop_AutoField;\
                        return


//#define RSP_MINUS(x)    CMemBase::malloc_local(x)
//#define RSP_ADD(x)      CMemBase::release_local(x)
#define GET_M(x)        CMemBase::M_R(x)
#define SET_M(x,y)      CMemBase::M_W(x,y)
#define MEMCPY(x,y,z)   CMemBase::memcpy(x, y, z)
#define push(x)         CMemBase::PUSH(x)
#define pop(x)          CMemBase::POP(x)
#define send_para(x)    push(x)

#define En_GRAM_To_CPU      CMemBase::_En_GRAM_To_CPU()
#define Dis_GRAM_To_CPU     CMemBase::_Dis_GRAM_To_CPU()
#define En_AllRAM_To_CPU    CMemBase::_En_AllRAM_To_CPU()
#define En_AllGRAM_To_CPU   CMemBase::_En_AllGRAM_To_CPU()
#define En_AllXRAM_To_CPU   CMemBase::_En_AllXRAM_To_CPU()

#define BASERAM_SIZE_DWORD      (1 * 1024)
#define GRAM_SIZE_DWORD         (4 * 1024)
#define XRAM_SIZE_DWORD         (2 * 1024)
#define CONV_SIZE_DWORD         (64 * 1024)



class CMemBase :
    public CCpu
{
public:




    // 将BASE_RAM GRAM XRAM 作为一整块连续地址的内存
	static int pTotalRam[BASERAM_SIZE_DWORD + GRAM_SIZE_DWORD + XRAM_SIZE_DWORD + CONV_SIZE_DWORD];

    static void init();
    static void malloc_local(int len);                             // BASE_RAM中局部变量空间申请，每次申请RSP会减小len，相当于RSP-=len
    static void release_local(int len);                            // BASE_RAM中局部变量空间释放，每次释放RSP会增加len，相当于RSP+=len

    static CReg M_R(CReg addr);                                     // 取地址所指的数据，相当于RDx = M[RAx]
    static void M_W(CReg addr, CReg data);                          // 设地址所指的数据，相当于M[RAx] = RDx
    static void PUSH(CReg rdx);                                     // 寄存器压栈，相当于push RDx
    static void POP(CReg &rdx);                                     // 寄存器压栈，相当于push RDx

    static void memcpy(CReg len, CReg src, CReg dst);

    // 形如M[RAx+n*MMU_BASE]
    CReg& operator [](CReg RAx)
    {
        RD_MEM.setMemOffset(RAx.m_data);
        RD_MEM = M_R(RAx);
        return RD_MEM;
    }

    static Sub_AutoField _En_GRAM_To_CPU();
    static Sub_AutoField _Dis_GRAM_To_CPU();
    static Sub_AutoField _En_AllRAM_To_CPU();
    static Sub_AutoField _En_AllGRAM_To_CPU();
    static Sub_AutoField _En_AllXRAM_To_CPU();

};



extern CMemBase M;
