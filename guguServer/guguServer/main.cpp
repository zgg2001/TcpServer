#include"TcpServer.h"

class MyServer : public TcpServer
{
public:
	//�ͻ��˼����¼� 
	virtual void OnNetJoin(ClientSocket* pClient)
	{
		TcpServer::OnNetJoin(pClient);
	}

	//�ͻ����˳�
	virtual void OnNetLeave(ClientSocket* pClient)
	{
		TcpServer::OnNetLeave(pClient);
	}

	//����˷�����Ϣ�¼�
	virtual void OnNetMsg(CellServer* pCellServer, ClientSocket* pClient, DataHeader* pHead)
	{
		//������++ 
		TcpServer::OnNetMsg(pCellServer, pClient, pHead);
		//printf("���յ���ͷ�����%d�����ݳ��ȣ�%d\n",head->cmd,head->date_length);
		switch (pHead->cmd)
		{
		case CMD_LOGIN://��¼ ���յ�¼���� 
		{
			Login* login = (Login*)pHead;
			/*����*/
			LoginResult* result = new LoginResult;
			result->Result = 1;
			pCellServer->AddSendTask(pClient, result);
		}
		break;
		case CMD_LOGOUT://�ǳ� ���յǳ����� 
		{
			Logout* logout = (Logout*)pHead;
			/*����*/
			LogoutResult* result = new LogoutResult();
			result->Result = 1;
			pCellServer->AddSendTask(pClient, result);
		}
		break;
		default://���� 
		{
			/*����*/
			pHead->cmd = CMD_ERROR;
			pHead->date_length = 0;
			pCellServer->AddSendTask(pClient, pHead);
		}
		break;
		}
	}

	//����˽�����Ϣ�¼�
	virtual void OnNetRecv(ClientSocket* pClient)
	{
		//�հ���++ 
		TcpServer::OnNetRecv(pClient);
	}
};

int main()
{
	printf("Welcome\n");

	MyServer* s1 = new MyServer();

	s1->InitSocket();
	s1->Bind(NULL, 8888);
	s1->Listen(5);
	s1->Start(4);
	while (s1->IsRun())
	{
		s1->OnRun();
	}
	s1->CloseSocket();

	return 0;
}