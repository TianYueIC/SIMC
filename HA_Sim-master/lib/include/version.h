#pragma once
#include <iostream>

using namespace std;

// version = para1.para2
typedef struct verNum
{
    int para1;
    int para2;
    int para3;
};
typedef struct YMD
{
    int year;
    int mon;
    int day;
};

class CVersion
{
public:
    CVersion()
    {
		setVersion(0, 9, 2);
        setYMD(2021, 6, 23);
        setPlatform("HA350");
        showAll();
    }
    

private:
    verNum ver;
    YMD time;
    char platform[128];


    void setVersion(int para1, int para2,int para3)
    {
        ver.para1 = para1;
        ver.para2 = para2;
        ver.para3 = para3;
    }

    void setYMD(int y, int m, int d)
    {
        time.year = y;
        time.mon = m;
        time.day = d;
    }

    void setPlatform(char* p)
    {
        strcpy(platform, p);
    }


    void showAll()
    {
        cout << "********************************************************" << endl;
        showTitle();
        showPlatform();
        showVersion();
        showTime();
        cout << "********************************************************" << endl;
    }

    void showTitle()
    {
        cout << "*\t《天悦HA系列芯片C语言模型仿真器》" << endl;
    }

    void showVersion()
	{
		cout << "*\tVersion: \tv" << ver.para1 << "." << ver.para2 << "." << ver.para3 << endl;
	}

    void showTime()
    {
        cout << "*\tRelease Date:\t" << time.year << "." << time.mon << "." << time.day << endl;
    }

    void showPlatform()
    {
        cout << "*\tPlatform: \t" << platform << endl;
    }


};

