#ifndef MY_TIMER_H_
#define MY_TIMER_H_

#include<chrono>

class __declspec(dllexport) mytimer
{
private:
    
public:
    mytimer();

    virtual ~mytimer();

    //����updateʱ��ʹ��ʼʱ����ڵ�ǰʱ��
    void UpDate();

    //����getsecond����ʱ��������ʱ��Ϊ��ǰʱ���ȥ֮ǰͳ�ƹ�����ʼʱ�䡣
    double GetSecond();

};


#endif
