#include "WDRC.h"


//////////////////////////////////////////////////////////////////////////
//  ����:
//      nondb2db_fix
//  ����:
//      ������ֵ��dB��תΪ������
//  ����:
//      1.short ig(q8)
//  ����ֵ��
//      1.RD0: gain ������ʱ���24q7��������ʱ���25q7
//////////////////////////////////////////////////////////////////////////
Sub nondb2db_fix
{
	push(RD2);

#define ig         GET_M(RSP+2*MMU_BASE)

	//gain = power_fix(ig >> 1);
	RD0 = ig;
	RF_Abs(RD0);
	RF_ShiftR1(RD0);
	call_AutoField power_fix;
	RF_ShiftR1(RD0);// ��q8ת��q7
	RD2 = RD0;

	RD0 = ig;
	if (RD0 >= 0) goto L_nondb2db_fix_End;
	RD0 = RD2;
	call recip_fix_Q7;
	RD2 = RD0;

L_nondb2db_fix_End:
	RD0 = RD2;

	pop(RD2);

#undef ig
	Return(1 * MMU_BASE);
}