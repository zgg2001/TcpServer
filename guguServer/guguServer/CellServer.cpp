#include"CellServer.h"

CellServer::CellServer(SOCKET sock)
{
	_sock = sock;
	_pThread = nullptr;
	_pNetEvent = nullptr;
	//��������� 
	_Recv_buf = new char[RECV_BUFFER_SIZE];
	//�ͻ��˼��������� 
	_maxSock = INVALID_SOCKET;
	FD_ZERO(&_fd_read_bak);
	_client_change = true;
}

CellServer::~CellServer()
{
	delete[] _Recv_buf;
	delete _pThread;
	delete _pNetEvent;
	//�ر�socket 
	CloseSocket();
	_sock = INVALID_SOCKET;
}

void CellServer::setEventObj(INetEvent* event)
{
	_pNetEvent = event;
}

void CellServer::CloseSocket()
{
	if (INVALID_SOCKET != _sock)
	{
#ifdef _WIN32
		//�رտͻ���socket
		for (int n = 0; n < (int)_clients.size(); ++n)
		{
			closesocket(_clients[n]->GetSockfd());
			delete _clients[n];
		}
		//�ر�socket
		closesocket(_sock);
#else
		//�رտͻ���socket
		for (int n = 0; n < _clients.size(); ++n)
		{
			close(_clients[n]->GetSockfd());
			delete _clients[n];
		}
		//�ر�socket/LINUX
		close(_sock);
#endif
		_sock = INVALID_SOCKET;
		//������� 
		_clients.clear();
		_clientsBuf.clear();
	}
}

bool CellServer::IsRun()
{
	return _sock != INVALID_SOCKET;
}

bool CellServer::OnRun()
{
	while (IsRun())
	{
		//����������еĿͻ����ݼ�����ʽ���� 
		if (!_clientsBuf.empty())
		{
			std::lock_guard<std::mutex> lock(_mutex);//���� 
			for (auto client : _clientsBuf)
			{
				_clients.push_back(client);
			}
			_clientsBuf.clear();
			_client_change = true;
		}
		//���û����Ҫ����Ŀͻ��˾����� 
		if (_clients.empty())
		{
			std::chrono::milliseconds t(1);//����һ���� 
			std::this_thread::sleep_for(t);
			continue;
		}
		fd_set fdRead;//�������� 
		FD_ZERO(&fdRead);//��ռ��� 
		//�����ӵĿͻ��� ����read���� 
		if (false == _client_change)
		{
			//û�з����ı� �Ͷ�ȡ���� 
			memcpy(&fdRead, &_fd_read_bak, sizeof(fd_set));
		}
		else
		{
			//������Ϸ����ı� ������¼��һ�� 
			_maxSock = _clients[0]->GetSockfd();//���socket 
			for (int n = _clients.size() - 1; n >= 0; --n)
			{
				FD_SET(_clients[n]->GetSockfd(), &fdRead);
				if (_maxSock < _clients[n]->GetSockfd())
				{
					_maxSock = _clients[n]->GetSockfd();
				}
			}
			//���ݸ��� 
			memcpy(&_fd_read_bak, &fdRead, sizeof(fd_set));
			_client_change = false;
		}
		//select����ɸѡselect 
		int ret = select(_maxSock + 1, &fdRead, 0, 0, 0);
		if (ret < 0)
		{
			printf("select�������\n");
			CloseSocket();
			return false;
		}
		//��������socket �鿴�Ƿ��д������¼� 
		for (int n = 0; n < (int)_clients.size(); ++n)
		{
			if (FD_ISSET(_clients[n]->GetSockfd(), &fdRead))
			{
				if (-1 == RecvData(_clients[n]))//�������� �ͻ����˳��Ļ� 
				{
					std::vector<ClientSocket*>::iterator iter = _clients.begin() + n;//�ҵ��˳��ͻ��˵ĵ�ַ
					if (iter != _clients.end())//����Ǻ���ֵ
					{
						if (_pNetEvent)//���߳���ɾ���ͻ��� 
						{
							_pNetEvent->OnNetLeave(_clients[n]);
						}
						delete _clients[n];
						_clients.erase(iter);//�Ƴ�
						_client_change = true;//�ͻ����˳� ��Ҫ֪ͨϵͳ����¼��fdset���� 
					}
				}
			}
		}
		//printf("����ʱ�䴦������ҵ��\n");
	}
	return true;
}

int CellServer::RecvData(ClientSocket* t_client)
{
	//�������߳� ��ʾ�յ��� 
	_pNetEvent->OnNetRecv(t_client);
	//���տͻ��˷��͵�����  
	int buf_len = recv(t_client->GetSockfd(), _Recv_buf, RECV_BUFFER_SIZE, 0);
	if (buf_len <= 0)
	{
		//printf("�ͻ������˳�\n");
		return -1;
	}

	//�����ջ����������ݿ�������Ϣ������ 
	memcpy(t_client->MsgBuf() + t_client->GetLen(), _Recv_buf, buf_len);
	//��Ϣ������������ĩβ���� 
	t_client->SetLen(t_client->GetLen() + buf_len);
	//�ж���Ϣ�����������ݳ����Ƿ���ڵ��ڰ�ͷ���� 
	while (t_client->GetLen() >= sizeof(DataHeader))//����ճ������ 
	{
		//ѡ����ͷ���� 
		DataHeader* header = (DataHeader*)t_client->MsgBuf();
		//�ж���Ϣ�����������ݳ����Ƿ���ڵ��ڱ��ĳ��� �����ٰ����� 
		if (t_client->GetLen() >= header->date_length)
		{
			//�������Ϣ��������ʣ��δ�������ݵĳ���
			int size = t_client->GetLen() - header->date_length;
			//��Ӧ���� 
			NetMsg(header, t_client);
			//����Ϣ������ʣ��δ���������ǰ��
			memcpy(t_client->MsgBuf(), t_client->MsgBuf() + header->date_length, size);
			//��Ϣ������������ĩβǰ��
			t_client->SetLen(size);
		}
		else
		{
			//��Ϣ���������ݲ��� 
			break;
		}
	}
	return 0;
}

void CellServer::NetMsg(DataHeader* pHead, ClientSocket* pClient)
{
	//�������߳� ���Ͱ� 
	_pNetEvent->OnNetMsg(this, pClient, pHead);
}

void CellServer::addClient(ClientSocket* client)
{
	std::lock_guard<std::mutex> lock(_mutex);
	//_mutex.lock();
	_clientsBuf.push_back(client);
	//_mutex.unlock();
}

void CellServer::Start()
{
	//mem_fn ����Ա����תΪ��������ʹ�ö���ָ����߶�����а�  ���mem_fun��mem_fun_ref�������� 
	_pThread = new std::thread(std::mem_fn(&CellServer::OnRun), this);
	_taskServer.Start();//���������߳� 
}

int CellServer::GetClientCount() const
{
	return _clients.size() + _clientsBuf.size();
}

void CellServer::AddSendTask(ClientSocket* pClient, DataHeader* pHead)
{
	CellSendMsgTask* ptask = new CellSendMsgTask(pClient, pHead);
	_taskServer.addTask(ptask);
}
