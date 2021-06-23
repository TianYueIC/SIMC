#pragma once

//ALUָ�����

#define Op8Bit          0b00000000
#define Op16Bit         0b01000000
#define Op32Bit         0b10000000

#define Rff_Add         0b00000000
#define Rff_Sub         0b00000001
#define Rff_Xor         0b00000010
#define Rff_Or          0b00000011
#define Rff_And         0b00000100

#define RffC_Add        0b00001000
#define RffC_Sub        0b00001001
#define RffC_Xor        0b00001010
#define RffC_Or         0b00001011
#define RffC_And        0b00001100
#define RffC_AddSftSR1  0b00001101
#define RffC_SubSftSR1  0b00001110

#define Rf_SftR8        0b00010000
#define Rf_SftR4        0b00010001
#define Rf_SftR2        0b00010010
#define Rf_SftR1        0b00010011
#define Rf_SftL8        0b00010100
#define Rf_SftL4        0b00010101
#define Rf_SftL2        0b00010110
#define Rf_SftL1        0b00010111

#define Rf_SftSR8       0b00011000
#define Rf_SftSR4       0b00011001
#define Rf_SftSR2       0b00011010
#define Rf_SftSR1       0b00011011

#define Rf_Const        0b00100000
#define Rf_SfAdd        0b00100001
#define Rf_Neg          0b00100010

#define Rf_PassFast     0b00101111