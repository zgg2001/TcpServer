/*
�ڴ����
�ڴ�������С��λ
2021/4/22
*/

#ifndef _Memory_Block_h_
#define _Memory_Block_h_

//�����ڴ����
class MemoryAlloc;
//��ײ㵼���ڴ�ͷ�ļ�/����ͷ�ļ�/��ͷ�ļ�
#include<stdlib.h>
#include<assert.h>
#include<mutex>
//���Ϊdebugģʽ����������Ϣ
#ifdef _DEBUG
#include<stdio.h>
#define xPrintf(...) printf(__VA_ARGS__)
#else
#define xPrintf(...)
#endif

class MemoryBlock
{
public:
	//�ڴ����
	int _nID;
	//�������
	int _nRef;
	//�����ڴ��
	MemoryAlloc* _pAlloc;
	//��һ��λ��
	MemoryBlock* _pNext;
	//�Ƿ����ڴ����
	bool _bPool;

private:

};

#endif
