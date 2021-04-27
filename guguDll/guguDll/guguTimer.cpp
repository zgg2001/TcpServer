#include"guguTimer.h"
#include"pch.h"

std::chrono::steady_clock::time_point _begin;//��ʼʱ��
std::chrono::steady_clock::time_point _end;//��ֹʱ��

mytimer::mytimer()
{
    _begin = std::chrono::steady_clock::now();
    _end = std::chrono::steady_clock::time_point();
}

mytimer::~mytimer()
{
}

void mytimer::UpDate()
{
    _begin = std::chrono::steady_clock::now();
}

double mytimer::GetSecond()
{
    _end = std::chrono::steady_clock::now();
    //ʹ��duration���ͱ�������ʱ��Ĵ���   duration_cast������ת������
    std::chrono::duration<double> temp = std::chrono::duration_cast<std::chrono::duration<double>>(_end - _begin);
    return temp.count();//count() ��ȡ��ǰʱ��ļ�������
}
