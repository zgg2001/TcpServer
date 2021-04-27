/*
* 子线程接收部分
* 本文件中实现线程类以及DoTask接口
* 使服务端线程分离 主线程接收连接 其余子线程处理消息
* 
* 初步实现了发送任务的接口，使其调用客户端对象的SendData方法发送消息
* 目前采用的是select结构 未来可能尝试其他结构
* 2021/4/22
*/
#ifndef _CELL_SERVER_H_
#define _CELL_SERVER_H_

#include"INetEvent.h"
#include"CellTask.h"
#include"ClientSocket.h"

#include <vector>

//网络消息发送任务
class CellSendMsgTask : public CellTask
{
public:
	CellSendMsgTask(ClientSocket* pClient, DataHeader* pHead)
	{
		_pClient = pClient;
		_pHeader = pHead;
	}

	//执行任务
	virtual void DoTask()
	{
		_pClient->SendData(_pHeader);
		delete _pHeader;
	}

private:
	ClientSocket* _pClient;
	DataHeader* _pHeader;

};

//线程类
class CellServer
{
public:
	//构造 
	CellServer(SOCKET sock = INVALID_SOCKET);
	//析构
	virtual ~CellServer();
	//处理事件 
	void setEventObj(INetEvent* event);
	//关闭socket 
	void CloseSocket();
	//判断是否工作中 
	bool IsRun();
	//查询是否有待处理消息 
	bool OnRun();
	//接收数据
	int RecvData(ClientSocket* t_client);//处理数据 
	//响应数据
	void NetMsg(DataHeader* pHead, ClientSocket* pClient);
	//增加客户端 
	void addClient(ClientSocket* client);
	//启动线程
	void Start();
	//获取该线程内客户端数量
	int GetClientCount() const;
	//添加任务
	void AddSendTask(ClientSocket* pClient, DataHeader* pHead);

private:
	//select优化
	SOCKET _maxSock;//最大socket值 
	fd_set _fd_read_bak;//读集合备份
	bool _client_change;//客户端集合bool true表示发生改变 需重新统计 fd_read集合

	//缓冲区相关 
	char* _Recv_buf;//接收缓冲区 
	//socket相关 
	SOCKET _sock;
	//正式客户队列 
	std::vector<ClientSocket*> _clients;//储存客户端
	//客户缓冲区
	std::vector<ClientSocket*> _clientsBuf;
	std::mutex _mutex;//锁
	//线程 
	std::thread* _pThread;
	//退出事件接口 
	INetEvent* _pNetEvent;
	//发送线程队列 
	CellTaskServer _taskServer;

};

#endif
