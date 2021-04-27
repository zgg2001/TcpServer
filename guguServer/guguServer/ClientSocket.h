/*
* 客户端类 
* 服务端对象中每加入一个新的客户端，都会新建一个客户端对象
* 通过该客户端对象向客户端进行发送消息等操作
* 
* 目前来说只实现了定量发送数据，即发送缓冲区满后发送消息，下一步预备完善为定时定量发送信息
* 2021/4/22
*/
#ifndef _CLIENT_SOCKET_H_
#define _CLIENT_SOCKET_H_

//socket相关内容
#ifdef _WIN32
	#define FD_SETSIZE 1024 
	#define WIN32_LEAN_AND_MEAN
	#include<winSock2.h>
	#include<WS2tcpip.h>
	#include<windows.h>
	#pragma comment(lib, "ws2_32.lib")//链接此动态链接库 windows特有 
	//连接动态库 此动态库里含有计时器类timer 和 cmd命令
	#include "pch.h"
	#pragma comment(lib, "guguDll.lib")
	//连接静态库 此静态库里含有一个内存池
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

//缓冲区大小 
#ifndef RECV_BUFFER_SIZE
	#define RECV_BUFFER_SIZE 4096
	#define SEND_BUFFER_SIZE 40
#endif

//客户端类 
class ClientSocket
{
public:
	//构造 
	ClientSocket(SOCKET sockfd = INVALID_SOCKET);
	//析构 
	virtual ~ClientSocket();
	//获取socket 
	SOCKET GetSockfd();
	//获取接收缓冲区 
	char* MsgBuf();
	//获取接收缓冲区尾部变量 
	int GetLen();
	//设置缓冲区尾部变量
	void SetLen(int len);
	//发送数据 
	int SendData(DataHeader* head);

private:
	SOCKET _sockfd;
	//缓冲区相关 
	char* _Msg_Recv_buf;//消息缓冲区 
	int _Len_Recv_buf;//缓冲区数据尾部变量

	char* _Msg_Send_buf;//消息发送缓冲区 
	int _Len_Send_buf;//发送缓冲区数据尾部变量
};

#endif
