/*
�ڴ����
���ڴ����й���
2021/4/22
*/

#ifndef _Memory_Alloc_h_
#define _Memory_Alloc_h_

//�����ڴ��ͷ�ļ�
#include"MemoryBlock.h"

class MemoryAlloc
{
public:
	MemoryAlloc();
	virtual ~MemoryAlloc();
	//���ó�ʼ��
	void setInit(size_t nSize, size_t nBlockSize);
	//��ʼ��
	void initMemory();
	//�����ڴ�
	void* allocMem(size_t nSize);
	//�ͷ��ڴ�
	void freeMem(void* p);

protected:
	//�ڴ�ص�ַ
	char* _pBuf;
	//ͷ���ڴ浥Ԫ
	MemoryBlock* _pHeader;
	//�ڴ���С
	size_t _nSize;
	//�ڴ������
	size_t _nBlockSize;
	//���߳���
	std::mutex _mutex;
};

#endif