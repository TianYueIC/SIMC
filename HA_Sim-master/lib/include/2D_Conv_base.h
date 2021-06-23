#pragma once

class C2D_Conv_base
{
public:
	int Multi16_Conv(short Multi16_0, short Multi16_1);
	int Sum16_Conv(int Sum[], int len);
	void Extend_2D_Conv_5x5(int* dataSource);
	void Extend_2D_Conv_5x5_2(int* dataSource);
	void Conv_3X3to5X5(int* key);
	void Conv_5x5(int* extend_data, int* key);
	void Conv_5x5_2(int* extend_data, int* key);
	void Multi_Array(short* A, short* B, int* C);

};

extern C2D_Conv_base conv;