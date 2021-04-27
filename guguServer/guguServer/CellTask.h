/*
* 子线程发送部分
* 本头文件中实现了任务执行的分离操作
* 通过list结构存储需要执行的任务
* start()启动线程进行任务处理
* 为防止出现冲突，所有临界操作均进行上锁，且首先使用缓冲区储存新任务
* 
* 2021/4/22
*/
#ifndef _CELL_Task_hpp_
#define _CELL_Task_hpp_

#include<thread>
#include<mutex>
#include<list>
#include <functional>//mem_fn

//任务基类接口
class CellTask
{
public:
	//执行任务 
	virtual void DoTask() = 0;
};

//发送线程类
class CellTaskServer
{
public:
	CellTaskServer();
	virtual ~CellTaskServer();
	//添加任务 
	void addTask(CellTask* ptask);
	//启动服务
	void Start();

protected:
	//工作函数 
	void OnRun();

private:
	//任务数据 
	std::list<CellTask*>_tasks;
	//任务数据缓冲区 
	std::list<CellTask*>_tasksBuf;
	//锁 锁数据缓冲区 
	std::mutex _mutex;
};

#endif
