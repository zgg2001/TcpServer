#include"TcpServer.h"

TcpServer::TcpServer()
{
	_msgCount = 0;
	_recvCount = 0;
	_clientCount = 0;
	_sock = INVALID_SOCKET;
}

TcpServer::~TcpServer()
{
	//�ر�socket 
	CloseSocket();
}

int TcpServer::InitSocket()
{
#ifdef _WIN32
	//����windows socket 2,x���� 
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	if (0 != WSAStartup(ver, &dat))
	{
		return -1;//-1Ϊ�������� 
	}
#endif 
	//����socket 
	if (INVALID_SOCKET != _sock)
	{
		printf("<Socket=%d>�ر�����\n", _sock);
		CloseSocket();//���֮ǰ������ �͹ر����� 
	}
	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == _sock)
	{
		return 0;//0Ϊsocket�������� 
	}
	return 1;
}

int TcpServer::Bind(const char* ip, unsigned short port)
{
	//���Ϊ��Ч�׽��� ���ʼ�� 
	if (INVALID_SOCKET == _sock)
	{
		InitSocket();
	}
	//������˿ں�IP��ַ 
	sockaddr_in _myaddr = {};
	_myaddr.sin_family = AF_INET;//IPV4
	_myaddr.sin_port = htons(port);//�˿�
#ifdef _WIN32
	if (ip)//ipΪ��������������� 
	{
		//_myaddr.sin_addr.S_un.S_addr = inet_addr(ip);//IP ��ʽд�� VS2019��֧��
		inet_pton(AF_INET, ip, &_myaddr.sin_addr.S_un.S_addr);
	}
	else
	{
		_myaddr.sin_addr.S_un.S_addr = INADDR_ANY;//IP
	}
#else
	if (ip)//ipΪ��������������� 
	{
		//_myaddr.sin_addr.S_un.S_addr = inet_addr(ip);//IP ��ʽд�� VS2019��֧��
		inet_pton(AF_INET, ip, &_myaddr.sin_addr.s_addr);
	}
	else
	{
		_myaddr.sin_addr.S_un.S_addr = INADDR_ANY;//IP
	}
#endif
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_myaddr, sizeof(sockaddr_in)))//socket (ǿ��ת��)sockaddr�ṹ�� �ṹ���С 
	{
		printf("��ʧ��\n");
		return 0;
	}
	else
	{
		printf("�󶨳ɹ�\n�󶨶˿�Ϊ%d\n", port);
		return 1;
	}
}

int TcpServer::Listen(int n)
{
	//���Ϊ��Ч�׽��� ����ʾ 
	if (INVALID_SOCKET == _sock)
	{
		printf("���ȳ�ʼ���׽��ֲ���IP�˿�\n");
		return 0;
	}
	//��������˿�
	if (SOCKET_ERROR == listen(_sock, n))//������Ӷ��� 
	{
		printf("����ʧ��\n");
		return 0;
	}
	else
	{
		printf("�����ɹ�\n");
		return 1;
	}
}

int TcpServer::Accept()
{
	//�ȴ����տͻ�������
	sockaddr_in clientAddr = {};//�½�sockadd�ṹ����տͻ������� 
	int addr_len = sizeof(sockaddr_in);//��ȡsockadd�ṹ�峤�� 
	SOCKET temp_socket = INVALID_SOCKET;//�����ͻ����׽��� 
#ifdef _WIN32	
	temp_socket = accept(_sock, (sockaddr*)&clientAddr, &addr_len);//�����׽��� �ͻ��˽ṹ�� �ṹ���С 	
#else
	temp_socket = accept(_sock, (sockaddr*)&clientAddr, (socklen_t*)&addr_len);//�����׽��� �ͻ��˽ṹ�� �ṹ���С 
#endif
	if (INVALID_SOCKET == temp_socket)//����ʧ�� 
	{
		printf("<Socket=%d>����,���ܵ���Ч�ͻ���SOCKET\n", temp_socket);
		return 0;
	}
	else
	{
		/*
		�¿ͻ���IP��ַ
		inet_ntoa(clientAddr.sin_addr)
		*/
		//���µĿͻ��˼��붯̬����
		AddClientToServer(new ClientSocket(temp_socket));
		return 1;
	}
}

void TcpServer::CloseSocket()
{
	if (INVALID_SOCKET != _sock)
	{
#ifdef _WIN32
		//�ر�socket
		closesocket(_sock);
		//���windows socket ���� 
		WSACleanup();
#else
		//�ر�socket/LINUX
		close(_sock);
#endif
		_sock = INVALID_SOCKET;
	}
}

void TcpServer::AddClientToServer(ClientSocket* pClient)
{
	//�ҳ��ͻ������ٵ��߳� Ȼ����� 
	auto pMinServer = _cellServers[0];
	for (auto pCellServer : _cellServers)
	{
		if (pMinServer->GetClientCount() > pCellServer->GetClientCount())
		{
			pMinServer = pCellServer;
		}
	}
	pMinServer->addClient(pClient);
	//�ͻ�������++ 
	OnNetJoin(pClient);
}

void TcpServer::Start(int nCellServer)
{
	for (int n = 0; n < nCellServer; n++)
	{
		//�̼߳������� 
		auto ser = new CellServer(_sock);
		_cellServers.push_back(ser);
		ser->setEventObj(this);
		ser->Start();
	}
}

bool TcpServer::IsRun()
{
	return _sock != INVALID_SOCKET;
}

bool TcpServer::OnRun()
{
	if (IsRun())
	{
		time4msg();//�鿴���߳�������Ϣ 
		fd_set fdRead;//�������� ����Ҫnewһ�� Ҫ��ջ�ᳬ16K��warning
		//fd_set fdWrite; 
		//fd_set fdExcept;
		FD_ZERO(&fdRead);//��ռ��� 
		//FD_ZERO(&fdWrite); 
		//FD_ZERO(&fdExcept); 
		FD_SET(_sock, &fdRead);//���뼯�� 
		//FD_SET(_sock,&fdWrite); 
		//FD_SET(_sock,&fdExcept);
		timeval s_t = { 0,0 };//select�����Ӧʱ�� 

		//select����ɸѡselect 
		int ret = select(_sock + 1, &fdRead, 0, 0, &s_t);
		if (ret < 0)
		{
			printf("select�������\n");
			CloseSocket();
			return false;
		}
		if (FD_ISSET(_sock, &fdRead))//��ȡ�Ƿ�����socket���� 
		{
			FD_CLR(_sock, &fdRead);//����
			Accept();//���� 
		}
		return true;
	}
	return false;
}

void TcpServer::time4msg()
{
	auto t1 = _time.GetSecond();
	if (1.0 <= t1)
	{
		//ʱ����  ����socket�������  �ͻ�������  ÿ���հ��� 
		printf("time<%lf>,socket<%d>,clients<%d>,msgCount<%d>,recvCount<%d>\n", t1, _sock, (int)_clientCount, (int)(_msgCount / t1 + 0.5), (int)(_recvCount / t1 + 0.5));
		_msgCount = 0;
		_recvCount = 0;//���� 
		_time.UpDate();
	}
}

void TcpServer::OnNetJoin(ClientSocket* pClient)
{
	_clientCount++;
}

void TcpServer::OnNetLeave(ClientSocket* pClient)
{
	//�ͻ�������-- 
	_clientCount--;
}

void TcpServer::OnNetMsg(CellServer* pCellServer, ClientSocket* pClient, DataHeader* pHead)
{
	//������++ 
	_msgCount++;
}

void TcpServer::OnNetRecv(ClientSocket* pClient)
{
	//�հ��� 
	_recvCount++;
}