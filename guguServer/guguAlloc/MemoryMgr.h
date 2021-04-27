/*
�ڴ��������
���ڴ�ؽ��й���
2021/4/22
*/

#ifndef _Memory_Mgr_h_
#define _Memory_Mgr_h_
//�ڴ���������
#define MAX_MEMORY_SIZE 128

//�����ڴ��ģ����
#include"MemoryAlloc.h"

class MemoryMgr
{
public:
	//����ʽ����ģʽ
	static MemoryMgr* Instance();
	//�����ڴ�
	void* allocMem(size_t nSize);
	//�ͷ��ڴ�
	void freeMem(void* p);
	//�����ڴ�����ô���
	void addRef(void* p);

private:
	MemoryMgr();
	virtual ~MemoryMgr();
	//�ڴ�ӳ���ʼ��
	void init_szAlloc(int begin, int end, MemoryAlloc* pMem);

private:
	//ӳ������
	MemoryAlloc* _szAlloc[MAX_MEMORY_SIZE + 1];
	//64�ֽ��ڴ��
	MemoryAlloc _mem64;
	//128�ֽ��ڴ��
	MemoryAlloc _mem128;
};

#endif