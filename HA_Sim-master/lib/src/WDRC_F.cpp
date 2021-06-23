#include "WDRC.h"


//////////////////////////////////////////////////////////////////////////
//  名称:
//      nondb2db_fix
//  功能:
//      将增益值从dB域转为线性域
//  参数:
//      1.short ig(q8)
//  返回值：
//      1.RD0: gain 正增益时输出24q7，负增益时输出25q7
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
	RF_ShiftR1(RD0);// 从q8转到q7
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