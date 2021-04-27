#include"MemoryMgr.h"

MemoryMgr::MemoryMgr()
{
	_mem64.setInit(64, 1000);
	init_szAlloc(0, 64, &_mem64);
	_mem128.setInit(128, 10);
	init_szAlloc(65, 128, &_mem128);
}

MemoryMgr::~MemoryMgr()
{
}

//��ʼ��
void MemoryMgr::init_szAlloc(int begin, int end, MemoryAlloc* pMem)
{
	//begin��end��С���ڴ����붼ӳ�䵽��ص��ڴ����
	for (int i = begin; i <= end; i++)
	{
		_szAlloc[i] = pMem;
	}
}

//����ʽ����ģʽ
MemoryMgr* MemoryMgr::Instance()
{
	static MemoryMgr myMemoryMgr;
	//��������
	return &myMemoryMgr;
}

//�����ڴ�
void* MemoryMgr::allocMem(size_t nSize)
{
	//��������ڴ��С��������ֱ������
	if (nSize <= MAX_MEMORY_SIZE)
	{
		return _szAlloc[nSize]->allocMem(nSize);
	}
	else//������malloc����һ��
	{
		MemoryBlock* pReturn = (MemoryBlock*)malloc(nSize + sizeof(MemoryBlock));
		if (nullptr != pReturn)
		{
			pReturn->_bPool = false;//���ڳ���
			pReturn->_nID = -1;
			pReturn->_nRef = 1;
			pReturn->_pAlloc = nullptr;
			pReturn->_pNext = nullptr;
			//debug��ӡ
			xPrintf("NEW - allocMem:%p,id=%d,size=%d\n", pReturn, pReturn->_nID, nSize);
		}
		return ((char*)pReturn + sizeof(MemoryBlock));
	}
}

//�ͷ��ڴ�
void MemoryMgr::freeMem(void* p)
{
	//������������Ϣ�� ��Ҫ������Ϣͷ
	MemoryBlock* pBlock = (MemoryBlock*)((char*)p - sizeof(MemoryBlock));
	//debug��ӡ
	xPrintf("DELETE - allocMem:%p,id=%d\n", pBlock, pBlock->_nID);
	//�ڴ���ڵ��ڴ��/�ڴ������ڴ�� ��ͬ�Ĵ���ʽ
	if (pBlock->_bPool == true)
	{
		pBlock->_pAlloc->freeMem(p);
	}
	else
	{
		if (--pBlock->_nRef == 0)
		{
			free(pBlock);
		}
	}
}

//�����ڴ�����ô���
void MemoryMgr::addRef(void* p)
{
	MemoryBlock* pBlock = (MemoryBlock*)((char*)p - sizeof(MemoryBlock));
	++pBlock->_nRef;
}
