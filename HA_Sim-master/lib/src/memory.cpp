#include "memory.h"

int CMemBase::pTotalRam[BASERAM_SIZE_DWORD + GRAM_SIZE_DWORD + XRAM_SIZE_DWORD+CONV_SIZE_DWORD] = { 0 };
//int CMemBase::pGRam[GRAM_SIZE_DWORD] = { 0 };
//int CMemBase::pXRam[XRAM_SIZE_DWORD] = { 0 };

CMemBase M;


void m_w(int offset, int data)
{
    int* p = (int*)((unsigned int)CMemBase::pTotalRam + offset);
    *p = data;
}


void CMemBase::init()
{
    RSP = BASERAM_SIZE_DWORD*MMU_BASE;

}

// 申请BASE_RAM内存，将改变RSP,RSP减小
void CMemBase::malloc_local(int len)
{
    RSP -= len;
}

// 释放BASE_RAM内存，改变RSP,RSP增大
void CMemBase::release_local(int len)
{
    RSP += len;
}

// 读RAx所指向的内存
CReg CMemBase::M_R(CReg RAx)
{
    int* p = (int*)((unsigned int)pTotalRam + RAx.m_data);
    CReg result = *p;
    return result;
}

// 写RAx所指向的内存
void CMemBase::M_W(CReg RAx, CReg RDx)
{
    m_w(RAx.m_data, RDx.m_data);
}


// 压栈RDx
void CMemBase::PUSH(CReg RDx)
{
    malloc_local(1 * MMU_BASE);
    M_W(RSP, RDx);
}


// 退栈
void CMemBase::POP(CReg& RDx)
{
    RDx = M_R(RSP);
    release_local(1 * MMU_BASE);
}


// BASE_RAM的拷贝,len为字节长度
void CMemBase::memcpy(CReg len, CReg src, CReg dst)
{
    int* psrc = (int*)((unsigned int)pTotalRam + src.m_data);
    int* pdst = (int*)((unsigned int)pTotalRam + dst.m_data);

    for (int i = 0; i < len.m_data / MMU_BASE; i++)
    {
        *(pdst + i) = *(psrc + i);
    }
}


////////////////////////////////////////////////////////
//  名称:
//      En_GRAM_To_CPU
//  功能:
//      将GRAM配置为CPU控制模式
//  参数:
//      1.RD0:需要配置的GRAM地址
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField En_GRAM_To_CPU
{
    Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      Dis_GRAM_To_CPU
//  功能:
//      将GRAM与CPU断开
//  参数:
//      无
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField Dis_GRAM_To_CPU
{

Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      En_AllRAM_To_CPU
//  功能:
//      将所有GRAM和XRAM配置为CPU控制模式
//  参数:
//      无
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField En_AllRAM_To_CPU
{
Return_AutoField(0);
}
////////////////////////////////////////////////////////
//  名称:
//      En_AllGRAM_To_CPU
//  功能:
//      将所有GRAM配置为CPU控制模式
//  参数:
//      无
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField En_AllGRAM_To_CPU
{
Return_AutoField(0);
}


////////////////////////////////////////////////////////
//  名称:
//      En_AllXRAM_To_CPU
//  功能:
//      将所有XRAM配置为CPU控制模式
//  参数:
//      无
//  返回值:
//      无
////////////////////////////////////////////////////////
Sub_AutoField En_AllXRAM_To_CPU
{
Return_AutoField(0);
}

