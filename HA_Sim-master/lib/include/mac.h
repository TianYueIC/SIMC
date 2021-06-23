#pragma once
#include "cpu.h"
#include "memory.h"



//MAC函数与用户层交际部分，采用类硬件写法
#define SingleSerSquare SingleSerSquare()
#define MultiSum MultiSum()
#define MAC_MultiConst16 MAC_MultiConst16()
#define MultiConstH16L16 MultiConstH16L16()
#define MultiVector2 MultiVector2()
#define ModulationToZero ModulationToZero()
#define DualSequMulti_Q0 DualSequMulti_Q0()
#define MultiConst24_DivQ7_LMT MultiConst24_DivQ7_LMT()
#define SeqMulti_DivQ7 SeqMulti_DivQ7()
#define ModulationToZero2 ModulationToZero2()
#define SingleSerSquareL SingleSerSquareL()
#define SingleSerSquare32 SingleSerSquare32()
#define MAC_MultiConst16_Q2207 MAC_MultiConst16_Q2207()
#define FIR_MAC FIR_MAC()
#define FIR_DATA FIR_DATA()
#define ComplexMulti_Q3116 ComplexMulti_Q3116()
#define ComplexMulti_Q2308 ComplexMulti_Q2308()
#define ComplexMulti_Q1500 ComplexMulti_Q1500()
#define MultiConst32_Dual_Q4615 MultiConst32_Dual_Q4615()
#define MultiConst32_Dual_Q3807 MultiConst32_Dual_Q3807()
#define MultiConst32_Dual_Q3100 MultiConst32_Dual_Q3100()
#define MultiConst32_Single_Q4615 MultiConst32_Single_Q4615()
#define MultiConst32_Single_Q3807 MultiConst32_Single_Q3807()
#define MultiConst32_Single_Q3100 MultiConst32_Single_Q3100()
#define SeqMulti_32X32_Q6231 SeqMulti_32X32_Q6231()
#define SeqMulti_32X32_Q5423 SeqMulti_32X32_Q5423()
#define SeqMulti_32X32_Q4615 SeqMulti_32X32_Q4615()
#define SeqMulti_32X32_Q3807 SeqMulti_32X32_Q3807()
#define SeqMulti_32X32_Q3100 SeqMulti_32X32_Q3100()
#define SeqMulti_32X16H_Q4615 SeqMulti_32X16H_Q4615()
#define SeqMulti_32X16H_Q3807 SeqMulti_32X16H_Q3807()
#define SeqMulti_32X16H_Q3100 SeqMulti_32X16H_Q3100()
#define SeqMulti_32X16L_Q4615 SeqMulti_32X16L_Q4615()
#define SeqMulti_32X16L_Q3807 SeqMulti_32X16L_Q3807()
#define SeqMulti_32X16L_Q3100 SeqMulti_32X16L_Q3100()


extern Sub_AutoField SingleSerSquare;

extern Sub_AutoField MultiSum;

extern Sub_AutoField MAC_MultiConst16;

extern Sub_AutoField MultiConstH16L16;

extern Sub_AutoField MultiVector2;

extern Sub_AutoField ModulationToZero;

extern Sub_AutoField DualSequMulti_Q0;

extern Sub_AutoField MultiConst24_DivQ7_LMT;

extern Sub_AutoField SeqMulti_DivQ7;

extern Sub_AutoField ModulationToZero2;

extern Sub_AutoField SingleSerSquareL;

extern Sub_AutoField SingleSerSquare32;

extern Sub_AutoField MAC_MultiConst16_Q2207;

extern Sub_AutoField FIR_MAC;

extern Sub_AutoField FIR_DATA;

extern Sub_AutoField ComplexMulti_Q3116;

extern Sub_AutoField ComplexMulti_Q2308;

extern Sub_AutoField ComplexMulti_Q1500;

extern Sub_AutoField MultiConst32_Dual_Q4615;

extern Sub_AutoField MultiConst32_Dual_Q3807;

extern Sub_AutoField MultiConst32_Dual_Q3100;

extern Sub_AutoField MultiConst32_Single_Q4615;

extern Sub_AutoField MultiConst32_Single_Q3807;

extern Sub_AutoField MultiConst32_Single_Q3100;

extern Sub_AutoField SeqMulti_32X32_Q6231;

extern Sub_AutoField SeqMulti_32X32_Q5423;

extern Sub_AutoField SeqMulti_32X32_Q4615;

extern Sub_AutoField SeqMulti_32X32_Q3807;

extern Sub_AutoField SeqMulti_32X32_Q3100;

extern Sub_AutoField SeqMulti_32X16H_Q4615;

extern Sub_AutoField SeqMulti_32X16H_Q3807;

extern Sub_AutoField SeqMulti_32X16H_Q3100;

extern Sub_AutoField SeqMulti_32X16L_Q4615;

extern Sub_AutoField SeqMulti_32X16L_Q3807;

extern Sub_AutoField SeqMulti_32X16L_Q3100;
