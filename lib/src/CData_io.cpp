#include "CData_io.h"
#include <iostream>
#include "memory.h"

int CData_io::* inBuf = NULL;

CData_io::CData_io(char fileIn[], char fileOut[])
{
    inBufIndex = 0;

    strcpy(fileInPath, fileIn);
    strcpy(fileOutPath, fileOut);
    readFile(fileInPath);
    createFile(fileOutPath);
}

// 从inBuf取一帧数据(如32)，如果取完或失败返回0，正确返回取到的数据个数
// 数据紧密排列[n+1|n]，将占据RAx指向的frameLen/2个DWORD
int CData_io::getData_1Frame(CReg RAx, int frameLen)
{
    int dwLen = frameLen / 2;

    int* frame = new int[dwLen];
    if (frame == NULL)
    {
        return 0;
    }

    if (inBufNum < (inBufIndex + dwLen))
    {
        delete[] frame;
        return 0;
    }

    for (int i = 0; i < dwLen; i++)
    {
        frame[i] = inBuf[inBufIndex + i];
    }
    inBufIndex += dwLen;

    // frame -> RAx
    for (int i = 0; i < dwLen; i++)
    {
        SET_M(RAx + i * MMU_BASE, frame[i]);
    }


    delete[] frame;

    loop++;
    return frameLen;
}


// 从RAx取一帧数据，写入到输出文件中
// RAx的数据为紧凑排列[n+1|n]，占据frameLen/2个DWORD，每个DWORD两个个采样数据
void CData_io::outData_1Frame(CReg RAx, int frameLen)
{
    int dwLen = frameLen / 2;

    int* frame = new int[dwLen];
    if (frame == NULL)
    {
        return;
    }    

    // frame <- RAx
    for (int i = 0; i < dwLen; i++)
    {
        CReg tempRD = GET_M(RAx + i * MMU_BASE);
        frame[i] = tempRD.m_data;
    }

    // 写入文件
    writeFile(fileOutPath, frame, dwLen);

    delete[] frame;
    return;
}

// 从RAx取bLen字节数据，顺序写入到输出文件中
void CData_io::outData_Data32(CReg RAx, int bLen)
{
    int dwLen = bLen / 4;

    int* frame = new int[dwLen];
    if (frame == NULL)
    {
        return;
    }

    // frame <- RAx
    for (int i = 0; i < dwLen; i++)
    {
        CReg tempRD = GET_M(RAx + i * MMU_BASE);
        frame[i] = tempRD.m_data;
    }

    // 写入文件
    writeFileBin_data32(frame, dwLen);

    delete[] frame;
    return;
}

// 从RAx取bLen字节数据（硬件紧凑格式[n+1|n]），改为顺序紧凑[n|n+1]格式写入到输出文件中
void CData_io::outData_Steam(CReg RAx, int bLen)
{
    outData_1Frame(RAx, bLen / 2);
}



// 读取文件的数据内容到inBuf
int CData_io::readFile(char file[])
{
    if (strlen(file) <= 1 )
    {
        return 0;
    }


    // 读取文件后缀, .bin .txt .wav等
    char* pTail = strrchr(file, '.');
    if (strcmp(pTail, ".bin") == 0)
    {
        return readFileBin2(file);
    }
    else
    {
        return 0;
    }


    

    return 0;
}

// 读取.bin文件, 数据格式为音频数据紧密排列，每个采样点为2字节，大端格式
// 读取出来的内容是松散排列，即一个DWORD一个点
// 返回读取的
int CData_io::readFileBin(char file[])
{
    int len = 0;

    // 取文件大小
    struct _stat info;
    _stat(file, &info);
    int size = info.st_size;

    // 打开文件
    FILE* fp = fopen(file, "rb");
    if (fp == NULL)
    {
        return len;
    }
    
    // inBuf申请空间
    int num = size / 2;
    inBuf = new int[num];
    if (inBuf == NULL)
    {
        return len;
    }
    inBufNum = num;

    // 读文件内容到inbuf
    unsigned char byte2[2];
    while (!feof(fp))
    {
        fread(byte2, 1, 2, fp);        
        unsigned char tempByte = byte2[0];
        byte2[0] = byte2[1];
        byte2[1] = tempByte;
        short tempShort = *(short*)byte2;
        inBuf[len++] = (int)tempShort;
    }
    fclose(fp);


    return len;
}


// 读取.bin文件, 数据格式为音频数据紧密排列，每个采样点为2字节，大端格式
// 读取出来的内容是紧密排列，即一个DWORD两个个点[n+1 | n]
// 返回读取的
int CData_io::readFileBin2(char file[])
{
    int len = 0;

    // 取文件大小
    struct _stat info;
    _stat(file, &info);
    int size = info.st_size;

    // 打开文件
    FILE* fp = fopen(file, "rb");
    if (fp == NULL)
    {
        return len;
    }

    // inBuf申请空间
    int num = size / 4;
    inBuf = new int[num];
    if (inBuf == NULL)
    {
        return len;
    }
    inBufNum = num;

    // 读文件内容到inbuf
    unsigned char byte4[4];
    while (!feof(fp))
    {
        fread(byte4, 1, 4, fp);
        BYTE* pTemp = (BYTE*)&inBuf[len++];
        pTemp[3] = byte4[2];
        pTemp[2] = byte4[3];
        pTemp[1] = byte4[0];
        pTemp[0] = byte4[1];
    }
    fclose(fp);


    return len;
}

// 将buf写入文件，写入方式是append
int CData_io::writeFile(char file[], int *buf, int len)
{
    // 读取文件后缀, .bin .txt .wav等
    char* pTail = strrchr(file, '.');
    if (strcmp(pTail, ".bin") == 0)
    {
        return writeFileBin2(file, buf, len);
    }
    else
    {
        return 0;
    }

    return 0;
}




// 将buf写入文件，写入方式是append, 数据格式为音频数据紧密排列，每个采样点为2字节，大端格式
int CData_io::writeFileBin(char file[], int* buf, int len)
{
    int retlen = 0;
    short temp;
    unsigned char tempByte[2];

    // 打开文件
    FILE* fp = fopen(file, "ab");
    if (fp == NULL)
    {
        return retlen;
    }

    for (int i = 0; i < len; i++)
    {
        temp = limit(buf[i]);
        tempByte[0] = *((unsigned char*)(&temp) + 1);
        tempByte[1] = *((unsigned char*)(&temp) + 0);

        fwrite(tempByte, 1, 2, fp);
        retlen += 2;
    }
    

    fclose(fp);

    return retlen;
}

// buf的格式为硬件紧凑格式[n+1|n]，每个采样点为2字节，大端格式
// 将buf写入文件，写入方式是append, 数据格式为音频数据紧密排列[n|n+1]，每个采样点为2字节，大端格式
// 返回写入文件的采样点数
int CData_io::writeFileBin2(char file[], int* buf, int len)
{
    int retlen = 0;
    unsigned char tempByte[4];

    // 打开文件
    FILE* fp = fp_out;
    if (fp == NULL)
    {
        return retlen;
    }

    for (int i = 0; i < len; i++)
    {
        BYTE* pTemp = (BYTE*)&buf[i];
        tempByte[0] = pTemp[1];
        tempByte[1] = pTemp[0];
        tempByte[2] = pTemp[3];
        tempByte[3] = pTemp[2];

        fwrite(tempByte, 1, 4, fp);
        retlen += 2;
    }


    //fclose(fp);

    return retlen;
}


// 将buf内容顺序写入文件，每个数占四字节大端格式，写入方式是append,
// 返回写入文件的字节数
int CData_io::writeFileBin_data32(int* buf, int len)
{
    int retlen = 0;
    unsigned char tempByte[4];

    // 打开文件
    FILE* fp = fp_out;
    if (fp == NULL)
    {
        return retlen;
    }

    for (int i = 0; i < len; i++)
    {
        BYTE* pTemp = (BYTE*)&buf[i];
        tempByte[0] = pTemp[3];
        tempByte[1] = pTemp[2];
        tempByte[2] = pTemp[1];
        tempByte[3] = pTemp[0];

        fwrite(tempByte, 1, 4, fp);
        retlen += 4;
    }

    return retlen;
}


// int转short，并且将数值范围限制在 -32767到32767
short CData_io::limit(int data)
{
    short rst;

    if (data > 32767)
    {
        rst = 32767;
    }
    else if (data < -32767)
    {
        rst = -32767;
    }
    else
    {
        rst = (short)data;
    }
    return rst;
}

void CData_io::createFile(char file[])
{
    fp_out = NULL;

    if (strlen(file) <= 0)
    {
        return;
    }

    FILE* fp = fopen(file, "wb");
    if (fp == NULL)
    {
        return;
    }
    
    fp_out = fp;

}



