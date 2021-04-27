/*
* �������
* ʵ�ֻ�����socket���Ӳ���
* ͨ��start�����������߳� ����հ�
* ���߳̽����пͻ��˼����˳����
* 2021/4/22
*/
#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include"CellServer.h"
#include<stdio.h>
#include<atomic>

//������� 
class TcpServer : INetEvent
{
public:
	//���� 
	TcpServer();
	//���� 
	virtual ~TcpServer();
	//��ʼ��socket ����1Ϊ���� 
	int InitSocket();
	//��IP/�˿�
	int Bind(const char* ip, unsigned short port);
	//�����˿�
	int Listen(int n);
	//��������
	int Accept();
	//�ر�socket 
	void CloseSocket();
	//��ӿͻ����������  
	void AddClientToServer(ClientSocket* pClient);
	//�߳����� 
	void Start(int nCellServer);
	//�ж��Ƿ����� 
	inline bool IsRun();
	//��ѯ�Ƿ��д�������Ϣ 
	bool OnRun();
	//��ʾ���߳�������Ϣ 
	void time4msg();
	//�ͻ��˼����¼� 
	virtual void OnNetJoin(ClientSocket* pClient);
	//�ͻ����˳�
	virtual void OnNetLeave(ClientSocket* pClient);
	//�ͻ��˷�����Ϣ�¼�
	virtual void OnNetMsg(CellServer* pCellServer, ClientSocket* pClient, DataHeader* pHead);
	virtual void OnNetRecv(ClientSocket* pClient);

private:
	//socket��� 
	SOCKET _sock;
	std::vector<CellServer*> _cellServers;//�̴߳��� 
	//��ʱ��
	mytimer _time;
	//���Ͱ�������
	std::atomic_int _msgCount;
	//���հ�������
	std::atomic_int _recvCount;
	//�ͻ��˼���
	std::atomic_int _clientCount;
};

#endif
