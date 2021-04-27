/*
* ���ļ��ж����˷���˵Ļ����ӿ�
* ��TcpServer.h�з������̳��˸ýӿ�
* 
* Ŀǰ���������ĸ������¼�
* 2021/4/22
*/
#ifndef _INET_EVENT_H_
#define _INET_EVENT_H_

class ClientSocket;
class CellServer;
struct DataHeader;

//����˻����ӿ�
class INetEvent
{
public:
	//�ͻ����˳��¼� 
	virtual void OnNetJoin(ClientSocket* pClient) = 0;
	//�ͻ����˳��¼� 
	virtual void OnNetLeave(ClientSocket* pClient) = 0;
	//����˷�����Ϣ�¼� 
	virtual void OnNetMsg(CellServer* pCellServer, ClientSocket* pClient, DataHeader* pHead) = 0;
	//����˽�����Ϣ�¼�
	virtual void OnNetRecv(ClientSocket* pClient) = 0;
};

#endif
