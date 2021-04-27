#include"ClientSocket.h"

ClientSocket::ClientSocket(SOCKET sockfd)
{
	_sockfd = sockfd;
	//��������� 
	_Msg_Recv_buf = new char[RECV_BUFFER_SIZE * 5];
	_Len_Recv_buf = 0;
	_Msg_Send_buf = new char[SEND_BUFFER_SIZE];
	_Len_Send_buf = 0;
}

ClientSocket::~ClientSocket()
{
	delete[] _Msg_Recv_buf;
	delete[] _Msg_Send_buf;
}

SOCKET ClientSocket::GetSockfd()
{
	return _sockfd;
}

char* ClientSocket::MsgBuf()
{
	return _Msg_Recv_buf;
}

int ClientSocket::GetLen()
{
	return _Len_Recv_buf;
}

void ClientSocket::SetLen(int len)
{
	_Len_Recv_buf = len;
}

int ClientSocket::SendData(DataHeader* head)
{
	int ret = SOCKET_ERROR;
	//Ҫ���͵����ݳ��� 
	int nSendLen = head->date_length;
	//Ҫ���͵����� 
	const char* pSendData = (const char*)head;
	//���ʹ��� 
	int times = 0;
	while (1)
	{
		//����������ݳ������ͻ����� �ͷ��� 
		if (_Len_Send_buf + nSendLen >= SEND_BUFFER_SIZE)
		{
			//������ʣ��ռ��С 
			int nCopyLen = SEND_BUFFER_SIZE - _Len_Send_buf;
			//��������
			memcpy(_Msg_Send_buf + _Len_Send_buf, pSendData, nCopyLen);
			//ʣ������λ����ʣ�����ݳ���
			pSendData += nCopyLen;
			nSendLen -= nCopyLen;
			//���� 
			ret = send(_sockfd, (const char*)_Msg_Send_buf, SEND_BUFFER_SIZE, 0);
			//���ͺ󻺳������� 
			_Len_Send_buf = 0;
			//���ʹ��� 
			if (SOCKET_ERROR == ret)
			{
				return ret;
			}
			times++;
		}
		else//����ͷ��뻺���� 
		{
			//���� 
			memcpy(_Msg_Send_buf + _Len_Send_buf, pSendData, nSendLen);
			//����β������ 
			_Len_Send_buf += nSendLen;
			break;
		}
	}
	return times;
}
