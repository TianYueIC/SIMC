#include "main.h"

using namespace std;

// 全局变量
char File_In[] = "./user/data/data_in.bin";		// 输入音频数据路径
char File_Out[] = "./user/data/data_out.bin";	// 输出音频数据路径

//CData_io data_io = CData_io(File_In, File_Out);     // 数据IO类初始化
CData_io data_io = CData_io("./user/data/steam_out.bin", "./user/data/data_out.bin");     // 数据IO类初始化
CData_io data_io2 = CData_io("./user/data/steamB_out.bin", "./user/data/data_out.bin");     // 数据IO类初始化



int main()
{

	// 系统初始化
	sys_init();

	cout << "start prj" << endl;

	// ===================临时测试===================
	//test_mem_oper();
	//test_simple_oper();
	//test_conditional();
	//test_float_model();
	//txt2bin();
	test_Conv();

	// ===================用户代码区====================
	// 算法核心流程(以时域衰减一半为例，可修改)
	RSP -= MMU_BASE;
	RA2 = RSP;

	FILE* fi0 = fopen("In0.txt", "w");
	FILE* fi1 = fopen("In1.txt", "w");
	FILE* fo = fopen("Out.txt", "w");

	int n = 0;
	RD0 = RN_GRAM2;
	RA2 = RD0;

	while (1)
	{
		// 从ADC取一帧数据存入RN_GRAM_IN
		RD0 = RN_GRAM_IN;
		RA0 = RD0;
		if (data_io.getData_1Frame(RA0, FRAME_LEN_Word) != FRAME_LEN_Word)
		{
			cout << "end prj" << endl;
			break;
		}
		RD0 = RN_GRAM_IN2;
		RA0 = RD0;
		if (data_io2.getData_1Frame(RA0, FRAME_LEN_Word) != FRAME_LEN_Word)
		{
			cout << "end prj" << endl;
			break;
		}

        //RD0 = RN_GRAM_IN;
        //RA0 = RD0;
        //RA1 = RD0;
		RD0 = RN_GRAM_IN;
		RA0 = RD0;
		RD0 = RN_GRAM_IN2;
		RA1 = RD0;
		int a[16], b[16], c[64];
		for (int i = 0; i < 16; i++)
		{
			RD0 = M[RA0++];
			a[i] = RD0.m_data;
			RD0 = M[RA1++];
			b[i] = RD0.m_data;
		}

		//n++;
		//RD0 = RN_GRAM_IN;
		//RA0 = RD0;
		//for (int i = 0; i < 16; i++)
		//{
		//	RD0 = M[RA0++];
		//	M[RA2++] = RD0;
		//}

		//if (n == 4)
		//{
		//	RD0 = RN_GRAM2 + 1 * MMU_BASE;
		//	RA0 = RD0;                                                                  // RA0:63点复数格式频谱,[Re1|Im1,...,Re63|Im63]
		//	RD0 = RN_GRAM2 + 127 * MMU_BASE;
		//	RA1 = RD0;                                                                  // RA1:WOLA综合fft复频谱的末尾地址
		//	RD0 = 63;																	// RD0:[127:65],后63点复数谱,63Dword
		//	call_AutoField DMA_TransDimin_PATH1;                                        // [RA1]=DMA_TransDimin_PATH1(RA0,RD0); 通过PATH1的DMA传输数据，目标地址为递减模式（倒序）; in: RA0:源指针, RD0:Dword长度; out: RD1:目标指针(out)

		//	RD0 = RN_GRAM2 + 1 * MMU_BASE;
		//	RA0 = RD0;                                                                  // RA0:63点复数格式频谱,[Re1|Im1,...,Re63|Im63]
		//	RA1 = RD0;                                                                  // RA1:WOLA综合的首地址加一
		//	RD1 = 0x7FFF8001;                                                           // RD1:Const值[ 1|-1 ]
		//	RD0 = 63;                                     // RD0:63点共轭谱长度,63Dword
		//	call_AutoField MultiConstH16L16;                                            // [RA1]=MultiConstH16L16(RA0,RD1,RD0);序列与Const相乘运算,Q15输出;H16L16都有数; in: RA0:源指针, RD1:Const值, RD0:序列长度; out: RD1:目标指针(out)

		//	RD0 = RN_GRAM2;
		//	RA0 = RD0;
		//	M[RA0] = 0;

		//	RD0 = RN_GRAM2;                                               // RD0:输入数据地址,WOLA综合输入的首地址
		//	RD1 = RN_GRAM2;                                               // RD1:输出结果地址,IFFT结果未加窗的四帧数据
		//	call_AutoField FFT_Fast128;                                                 // [RD0,RD1]=FFT_Fast128(RD0); 128点FFT运算，采用128点专用加速器; in:RD0:输入数据地址; out:RD0:增益系数, RD1:输出结果地址;
		//	RD2 = RD0;                                                                  // RD2:增益系数

		//	RA0 = RN_GRAM2;
		//	for (int i = 0; i < 64; i++)
		//	{
		//		RD0 = M[RA0++];
		//		c[i] = RD0.m_data;
		//	}
		//	for (int i = 0; i < 64; i++)
		//	{
		//		fprintf(fo, "%d\r", c[i]);
		//	}

		//}
		call_AutoField doublemic;

		RA0 = RN_GRAM_OUT;
		for (int i = 0; i < 16; i++)
		{
			RD0 = M[RA0++];
			c[i] = RD0.m_data;
		}

		for (int i = 0; i < 16; i++)
		{
			fprintf(fi0, "%d\r", a[i]);
			fprintf(fi1, "%d\r", b[i]);
			fprintf(fo, "%x\r", c[i]);
		}



		// RN_GRAM_OUT输出一帧数据到DAC
		RD0 = RN_GRAM_OUT;
		RA0 = RD0;
		data_io.outData_1Frame(RA0, FRAME_LEN_Word);


	}
	fclose(fi0);
	fclose(fi1);
	fclose(fo);




	return 0;
}

void sys_init()
{
	// 这块代码不允许修改
	CVersion ver111;		// 显示库版本信息
	CMemBase::init();		// 内存初始化，RSP初始化

	// 以下为用户代码区
	RD0 = RSP;
	RD1 = RN_GLOBAL_VAR_TOTAL_LEN_B;
	RD0 -= RD1;
	RSP = RD0;
	RA4 = RD0;				// RA4指向全局变量起始地址

	// 配置HP系数
	call_AutoField IIR_PATH3_HPInit_HP2;
	call_AutoField FIR_PATH3_HPInit_HP1;
	call_AutoField FIR_DATA;
	
}


