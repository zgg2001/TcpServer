/*
* ���̷߳��Ͳ���
* ��ͷ�ļ���ʵ��������ִ�еķ������
* ͨ��list�ṹ�洢��Ҫִ�е�����
* start()�����߳̽���������
* Ϊ��ֹ���ֳ�ͻ�������ٽ����������������������ʹ�û���������������
* 
* 2021/4/22
*/
#ifndef _CELL_Task_hpp_
#define _CELL_Task_hpp_

#include<thread>
#include<mutex>
#include<list>
#include <functional>//mem_fn

//�������ӿ�
class CellTask
{
public:
	//ִ������ 
	virtual void DoTask() = 0;
};

//�����߳���
class CellTaskServer
{
public:
	CellTaskServer();
	virtual ~CellTaskServer();
	//������� 
	void addTask(CellTask* ptask);
	//��������
	void Start();

protected:
	//�������� 
	void OnRun();

private:
	//�������� 
	std::list<CellTask*>_tasks;
	//�������ݻ����� 
	std::list<CellTask*>_tasksBuf;
	//�� �����ݻ����� 
	std::mutex _mutex;
};

#endif
