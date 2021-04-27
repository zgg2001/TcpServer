/*
* �ͻ����� 
* ����˶�����ÿ����һ���µĿͻ��ˣ������½�һ���ͻ��˶���
* ͨ���ÿͻ��˶�����ͻ��˽��з�����Ϣ�Ȳ���
* 
* Ŀǰ��˵ֻʵ���˶����������ݣ������ͻ�������������Ϣ����һ��Ԥ������Ϊ��ʱ����������Ϣ
* 2021/4/22
*/
#ifndef _CLIENT_SOCKET_H_
#define _CLIENT_SOCKET_H_

//socket�������
#ifdef _WIN32
	#define FD_SETSIZE 1024 
	#define WIN32_LEAN_AND_MEAN
	#include<winSock2.h>
	#include<WS2tcpip.h>
	#include<windows.h>
	#pragma comment(lib, "ws2_32.lib")//���Ӵ˶�̬���ӿ� windows���� 
	//���Ӷ�̬�� �˶�̬���ﺬ�м�ʱ����timer �� cmd����
	#include "pch.h"
	#pragma comment(lib, "guguDll.lib")
	//���Ӿ�̬�� �˾�̬���ﺬ��һ���ڴ��
	#pragma comment(lib, "guguAlloc.lib")
#else
	#include<sys/socket.h>
	#include<arpa/inet.h>//selcet
	#include<unistd.h>//uni std
	#include<string.h>

	#define SOCKET int
	#define INVALID_SOCKET (SOCKET)(~0)
	#define SOCKET_ERROR (-1)
#endif

//��������С 
#ifndef RECV_BUFFER_SIZE
	#define RECV_BUFFER_SIZE 4096
	#define SEND_BUFFER_SIZE 40
#endif

//�ͻ����� 
class ClientSocket
{
public:
	//���� 
	ClientSocket(SOCKET sockfd = INVALID_SOCKET);
	//���� 
	virtual ~ClientSocket();
	//��ȡsocket 
	SOCKET GetSockfd();
	//��ȡ���ջ����� 
	char* MsgBuf();
	//��ȡ���ջ�����β������ 
	int GetLen();
	//���û�����β������
	void SetLen(int len);
	//�������� 
	int SendData(DataHeader* head);

private:
	SOCKET _sockfd;
	//��������� 
	char* _Msg_Recv_buf;//��Ϣ������ 
	int _Len_Recv_buf;//����������β������

	char* _Msg_Send_buf;//��Ϣ���ͻ����� 
	int _Len_Send_buf;//���ͻ���������β������
};

#endif
