#include "FFT_F.h"
#include "memory.h"
#include "mac.h"
#include <math.h>
#include <stdio.h>

#define _CRT_FUNCTIONS_REQUIRED
#define LOWER_FFT_ACCURACY

extern void fft(double pr[], double pi[], int n, double fr[], double fi[]);

////////////////////////////////////////////////////////
//  ��������:
//      FFT_fix64
//  ��������:
//      FFT����
//  �β�:
//      1.RD0:��������ָ�룬������ʽ
//      2.RD1:�������ָ�룬������ʽ
//  ����ֵ:
//      RD0:����ϵ��
////////////////////////////////////////////////////////
Sub_AutoField FFT_fix64
{
	double pr[64], pi[64], fr[64], fi[64], x;

	RA0 = RD0;
	RA1 = RD1;

	for (int i = 0; i <= 63; i++)
	{
		RD0 = GET_M(RA0 + i * MMU_BASE);
        x = floor(RD0.m_data / 65536);
        x = x / 32768;
        pr[i] = x;
        x = *(short*)(&RD0.m_data);					//��16bit 
        x = x / 32768;
        pi[i] = x;
    }
	fft(pr, pi, 64, fr, fi);    //fft

   //�޷������
    int k = RD0.m_data;
    double max0 = 0;
    double max1 = 0;
    for (int i = 0; i < 64; i++)
    {
        if (fabs(fr[i]) > fabs(max0))
            max0 = fabs(fr[i]);
        if (fabs(fi[i]) > fabs(max1))
            max1 = fabs(fi[i]);
    }
    if (fabs(max0) < fabs(max1))
        max0 = max1;
    if (max0 < 1)
    {
        k = 0;
    }
    else
    {
        k = log2(max0) + 1;
    }


    int l = 1 << k;
    for (int i = 0; i < 64; i++)
    {
        fr[i] = fr[i] / l;
        fi[i] = fi[i] / l;
    }


    for (int i = 0; i <= 63; i++)
    {
        x = fr[i];
		x = x * 32768;
        RD0 = x;
        RD0 <<= 16;
        x = fi[i];
        x = x * 32768;
        RD1 = x;
        RD1 &= 0xffff;
        RD0 += RD1;
        SET_M(RA1 + i * MMU_BASE, RD0);

    }
    RD0 = k;
    Return_AutoField(0);

}

//////////////////////////////////////////////////////////
////  ��������:
////      FFT_32X128
////  ��������:
////      128���λFFT����
////  �β�:
////      1.RD0:��������ָ�룬32bit��ʽ��˳��ΪR0;I0;R1;I1...
////  ����ֵ:
////      RD0:����ϵ��
//////////////////////////////////////////////////////////
//Sub_AutoField FFT_Fast128
//{
//    double pr[128], pi[128], fr[128], fi[128], x;
//
//    RA0 = RD0;
//
//    for (int i = 0; i < 128; i++)
//    {
//        RD0 = M[RA0++];
//        x = RD0.m_data;
//        x = x / 32768;
//        pr[i] = x;
//        RD0 = M[RA0++];
//        x = RD0.m_data;
//        x = x / 32768;
//        pi[i] = x;
//    }
//    fft(pr, pi, 128, fr, fi);    //fft
//
//    //�޷������
//    int k = 0;
//    double max0 = 0;
//    double max1 = 0;
//    for (int i = 0; i < 128; i++)
//    {
//        if (fabs(fr[i]) > fabs(max0))
//            max0 = fabs(fr[i]);
//        if (fabs(fi[i]) > fabs(max1))
//            max1 = fabs(fi[i]);
//    }
//    if (fabs(max0) < fabs(max1))
//        max0 = max1;
//    if (max0<65536)
//    {
//        k = 0;
//    }
//    else
//    {
//    k = log2(max0/65536) + 1;
//    }
//
//    
//    int l = 1 << k;
//    for (int i = 0; i < 128; i++)
//    {
//        fr[i] = fr[i] / l;
//        fi[i] = fi[i] / l;
//    }
//
//
//    for (int i = 0; i < 128; i++)
//    {
//        x = fr[i];
//        x = x * 32768;
//        RD0 = x;
//        M[RA1++] = RD0;
//
//        x = fi[i];
//        x = x * 32768;
//        RD0 = x;
//        M[RA1++] = RD0;
//    }
//    RD0 = k;
//    Return_AutoField(0);
//
//}


////////////////////////////////////////////////////////
//  ��������:
//      FFT_Fast128
//  ��������:
//      128��FFT���㣬����128��ר�ü�����
//  �β�:
//      1.RD0:��������ָ�룬������ʽ
//      2.RD1:�������ָ�룬������ʽ
//  ����ֵ:
//      RD0:����ϵ��
////////////////////////////////////////////////////////
Sub_AutoField FFT_Fast128
{
    double pr[128], pi[128], fr[128], fi[128], x;

    RA0 = RD0;
    RA1 = RD1;

    for (int i = 0; i < 128; i++)
    {
        RD0 = GET_M(RA0 + i * MMU_BASE);
		x = floor(RD0.m_data / 65536);
        x = floor(x / 2);
        x = x / 32768;
        pr[i] = x;
        x = *(short*)(&RD0.m_data);					//��16bit 
        x = floor(x / 2);
        x = x / 32768;
        pi[i] = x;
    }
    fft(pr, pi, 128, fr, fi);    //fft

    //�޷������
    int k = RD0.m_data;
    double max0 = 0;
    double max1 = 0;
    for (int i = 0; i < 128; i++)
    {
        if (fabs(fr[i]) > fabs(max0))
            max0 = fabs(fr[i]);
        if (fabs(fi[i]) > fabs(max1))
            max1 = fabs(fi[i]);
    }
    if (fabs(max0) < fabs(max1))
        max0 = max1;
    if (max0<1)
    {
        k = 0;
    }
    else
    {
    k = log2(max0) + 1;
    }

    
    int l = 1 << k;
    for (int i = 0; i < 128; i++)
    {
        fr[i] = fr[i] / l;
        fi[i] = fi[i] / l;
    }


    for (int i = 0; i < 128; i++)
    {
        x = fr[i];
        x = x * 32768;
        RD0 = x;
        RD0 <<= 16;
        x = fi[i];
        x = x * 32768;
        RD1 = x;
        RD1 &= 0xffff;
        RD0 += RD1;
        SET_M(RA1 + i * MMU_BASE, RD0);

    }
    RD0 = k;
    Return_AutoField(0);

}
/////////////////////////////////////////////////////////////////////////////
//  ����:
//      fft
//  ����:
//      fft����
//  ����:
//      double pr[n]	���n�����������ʵ����������ɢ����Ҷ�任����
//      double pi[n]	���n������������鲿
//      double fr[n]	������ɢ����Ҷ�任��n��ʵ��
//      double fi[n]	������ɢ����Ҷ�任��n���鲿
//      int n	        ��������
//      int k	        ����n = 2^k 
//  ����ֵֵ:
//      ��
//////////////////////////////////////////////////////////////////////////////
void fft(double pr[], double pi[], int n, double fr[], double fi[])
{
    RD0 = 1;
	int k = log2(n);
    int it, m, is, i, j, nv, l0;
    double p, q, s, vr, vi, poddr, poddi;

    //FILE* fo = fopen("Out.txt", "w");//��ӡ�м���

    for (it = 0; it <= n - 1; it++)  //��pr[0]��pi[0]ѭ����ֵ��fr[]��fi[]
    {
        m = it;
        is = 0;
        for (i = 0; i <= k - 1; i++)
        {
            j = m / 2;
            is = 2 * is + (m - 2 * j);
            m = j;
        }
        fr[it] = pr[is];
        fi[it] = pi[is];
    }

  //  fprintf(fo, "//������\r");
  //  for (int qq = 0; qq < 128; qq++)
  //  {
  //      int x = fr[qq] * 32768;
  //      int y = fi[qq] * 32768;
		//x = (x << 16) + (y & 0xffff);
  //      fprintf(fo, "%08x\r", x);
  //  }


    pr[0] = 1.0;
    pi[0] = 0.0;
    p = 6.283185306 / (1.0 * n);
    pr[1] = cos(p); //��w=e^-j2pi/n��ŷ����ʽ��ʾ
    pi[1] = -sin(p);

    for (i = 2; i <= n - 1; i++)  //����pr[]��ϵ��
    {
        p = pr[i - 1] * pr[1];
        q = pi[i - 1] * pi[1];
        s = (pr[i - 1] + pi[i - 1]) * (pr[1] + pi[1]);
        pr[i] = p - q; pi[i] = s - p - q;
    }

    //fprintf(fo, "//ϵ����\r");
    //for (int qq = 0; qq < 128; qq++)
    //{
    //    int x = pr[qq] * 32768;
    //    int y = pi[qq] * 32768;
    //    x = (x << 16) + (y & 0xffff);
    //    fprintf(fo, "%08x\r", x);
    //}

    for (it = 0; it <= n - 2; it = it + 2)//��һ�ֵ���
    {
        vr = fr[it];
        vi = fi[it];
        fr[it] = vr + fr[it + 1];
        fi[it] = vi + fi[it + 1];
        fr[it + 1] = vr - fr[it + 1];
        fi[it + 1] = vi - fi[it + 1];
    }
    //fprintf(fo, "//��һ�ֵ���\r");
    //for (int qq = 0; qq < 128; qq++)
    //{
    //    int x = fr[qq] * 32768;
    //    int y = fi[qq] * 32768;
    //    x = (x << 16) + (y & 0xffff);
    //    fprintf(fo, "%08x\r", x);
    //}

    m = n / 2;
    nv = 2;
    for (l0 = k - 2; l0 >= 0; l0--) //��������
    {
        m = m / 2;
        nv = 2 * nv;
        for (it = 0; it <= (m - 1) * nv; it = it + nv)
            for (j = 0; j <= (nv / 2) - 1; j++)
            {
#ifdef LOWER_FFT_ACCURACY//�;��ȣ����ӽ�Ӳ�����ȣ�ͬmatlab�����֤ʱʹ��HIGHER_FFT_ACCURACY
                p = pr[m * j] * fr[it + j + nv / 2];
                int xx = p * 32768;
                p = xx;
                p = p / 32768;
                q = pi[m * j] * fi[it + j + nv / 2];
                xx = q * 32768;
                q = xx;
                q = q / 32768;
                s = pr[m * j] + pi[m * j];
                s = s * (fr[it + j + nv / 2] + fi[it + j + nv / 2]);
                xx = s * 32768;
                s = xx;
                s = s / 32768;
                poddr = p - q;
                poddi = s - p - q;
                fr[it + j + nv / 2] = fr[it + j] - poddr;
                fi[it + j + nv / 2] = fi[it + j] - poddi;
                fr[it + j] = fr[it + j] + poddr;
                fi[it + j] = fi[it + j] + poddi;
#elif defined HIGHER_FFT_ACCURACY//�߾���
                p = pr[m * j] * fr[it + j + nv / 2];
                q = pi[m * j] * fi[it + j + nv / 2];
                s = pr[m * j] + pi[m * j];
                s = s * (fr[it + j + nv / 2] + fi[it + j + nv / 2]);
                poddr = p - q;
                poddi = s - p - q;
                fr[it + j + nv / 2] = fr[it + j] - poddr;
                fi[it + j + nv / 2] = fi[it + j] - poddi;
                fr[it + j] = fr[it + j] + poddr;
                fi[it + j] = fi[it + j] + poddi;
#endif
            }
        //fprintf(fo, "//��%d�ֵ���\r", 7-l0);
        //for (int qq = 0; qq < 128; qq++)
        //{
        //    int x = fr[qq] * 32768;
        //    int y = fi[qq] * 32768;
        //    x = (x << 16) + (y & 0xffff);
        //    fprintf(fo, "%08x\r", x);
        //}
    
    //������Ӳ��ģʽ�����������
    //    int order = 0;
    //    for (int ii = 0; ii < n; ii++)
    //    {
    //        if (fabs(fr[ii]) >= 0.25)
    //            order = 1;
    //        if (fabs(fi[ii]) >= 0.25)
    //            order = 1;
    //    }
    //    if (order == 1)
    //    {
    //        for (int ii = 0; ii < n; ii++)
    //        {
				//fr[ii] = fr[ii] / 2;
				//fi[ii] = fi[ii] / 2;
    //            RD0++;
    //        }
    //    }
    }
    for (i = 0; i <= n - 1; i++)
    {
        pr[i] = sqrt(fr[i] * fr[i] + fi[i] * fi[i]);  //��ֵ����
    }
    //fclose(fo);

    return;
}


////////////////////////////////////////////////////////
//  ����:
//      FFT_fix
//  ����:
//      FFT���㣨512�����٣�
//      ������ڣ�RN_FFT_COFF_GRAM_ADDR
//  ����:
//      1.RD0:��������ָ�룬������ʽ
//      2.RD1:�������ָ�룬������ʽ(out)
//  ����ֵ:
//      1.RD0:����ϵ��
////////////////////////////////////////////////////////
Sub_AutoField FFT_fix
{
    double pr[512], pi[512], fr[512], fi[512], x;

    RA0 = RD0;
    RA1 = RD1;

	for (int i = 0; i < 512; i++)
    {
        RD0 = M[RA0 + i * MMU_BASE];
        x = floor(RD0.m_data / 65536);
        x = x / 32768;
        pr[i] = x;
        x = *(short*)(&RD0.m_data);					//��16bit 
        x = x / 32768;
        pi[i] = x;
    }
    fft(pr, pi, 512, fr, fi);    //fft

   //�޷������
    int k = RD0.m_data;
    double max0 = 0;
    double max1 = 0;
    for (int i = 0; i < 512; i++)
    {
        if (fabs(fr[i]) > fabs(max0))
            max0 = fabs(fr[i]);
        if (fabs(fi[i]) > fabs(max1))
            max1 = fabs(fi[i]);
    }
    if (fabs(max0) < fabs(max1))
        max0 = max1;
    if (max0 < 1)
    {
        k = 0;
    }
    else
    {
        k = log2(max0) + 1;
    }


    int l = 1 << k;
    for (int i = 0; i < 512; i++)
    {
        fr[i] = fr[i] / l;
        fi[i] = fi[i] / l;
    }


	for (int i = 0; i < 512; i++)
    {
        x = fr[i];
        x = x * 32768;
        RD0 = x;
        RD0 <<= 16;
        x = fi[i];
        x = x * 32768;
        RD1 = x;
        RD1 &= 0xffff;
        RD0 += RD1;
		M[RA1 + i * MMU_BASE] = RD0;

    }
    RD0 = k;
    Return_AutoField(0);

}


////////////////////////////////////////////////////////
//  ����:
//      IFFT_512
//  ����:
//      512��IFFT����
//  ����:
//      1.RD0:��������ָ�룬������ʽ
//      2.RD1:�������ָ�룬������ʽ(out)
//  ����ֵ:
//      1.RD0:����ϵ��
//  ��ע��
//      
////////////////////////////////////////////////////////
Sub_AutoField IFFT_512
{
    //����
	RA0 = RD0;
    RA1 = RD0;
    RD1 = 0X7FFF8000;
    RD0 = 512;
    call_AutoField MultiConstH16L16;

    RA0 = RD0;
    RA1 = RD1;
    call_AutoField FFT_fix;
    RD2 = RD0;//��������

    //�������N
    RA0 = RD1;
    RA1 = RD1;
    RD1 = 0X0040FFC0;
    RD0 = 512;
    call_AutoField MultiConstH16L16;

    RD0 = RD2;

    Return_AutoField(0);

}
