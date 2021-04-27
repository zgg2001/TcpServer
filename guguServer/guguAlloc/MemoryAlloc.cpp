#include"MemoryAlloc.h"

MemoryAlloc::MemoryAlloc()
{
	_pBuf = nullptr;
	_pHeader = nullptr;
	_nSize = 0;
	_nBlockSize = 0;
}

MemoryAlloc::~MemoryAlloc()
{
	if (_pBuf)
	{
		free(_pBuf);
		//������һ����������ڴ����������ڴ治�ᱻ�����ͷ�
	}
}

void MemoryAlloc::setInit(size_t nSize, size_t nBlockSize)
{
	/*��ȫnSize
	const size_t n = sizeof(void*)
	_nSize = (nSize/n) * n + (nSize % n ? n : 0);
	*/
	_pBuf = nullptr;
	_pHeader = nullptr;
	_nSize = nSize;
	_nBlockSize = nBlockSize;
	initMemory();
}

void MemoryAlloc::initMemory()
{
	//����
	assert(nullptr == _pBuf);
	//���������򷵻�
	if (nullptr != _pBuf)
	{
		return;
	}
	//�����ڴ�صĴ�С  (���С+��ͷ) * ������
	size_t temp_size = _nSize + sizeof(MemoryBlock);//��Ҫƫ�Ƶ�������С
	size_t bufSize = temp_size * _nBlockSize;
	//��ϵͳ������ڴ�
	_pBuf = (char*)malloc(bufSize);
	//��ʼ���ڴ��
	_pHeader = (MemoryBlock*)_pBuf;
	if (nullptr != _pHeader)
	{
		_pHeader->_bPool = true;//�ڳ���
		_pHeader->_nID = 0;//��0��
		_pHeader->_nRef = 0;//���ô���Ϊ0
		_pHeader->_pAlloc = this;//���ڵ�ǰ�ڴ��
		_pHeader->_pNext = nullptr;//��һ��
		MemoryBlock* pTemp1 = _pHeader;
		//�����ڴ����г�ʼ��
		for (size_t n = 1; n < _nBlockSize; n++)
		{
			MemoryBlock* pTemp2 = (MemoryBlock*)(_pBuf + (n * temp_size));//ָ��ƫ�Ƶ���һ��
			pTemp2->_bPool = true;//�ڳ���
			pTemp2->_nID = n;//��n��
			pTemp2->_nRef = 0;
			pTemp2->_pAlloc = this;
			pTemp2->_pNext = nullptr;
			pTemp1->_pNext = pTemp2;
			pTemp1 = pTemp2;
		}
	}
}

void* MemoryAlloc::allocMem(size_t nSize)
{
	//�Խ���
	std::lock_guard<std::mutex> lock(_mutex);
	//���ڴ�ز��������ʼ��
	if (nullptr == _pBuf)
	{
		initMemory();
	}
	MemoryBlock* pReturn = nullptr;
	if (nullptr == _pHeader)//���ڴ������ ��������
	{
		pReturn = (MemoryBlock*)malloc(nSize + sizeof(MemoryBlock));
		if (nullptr != pReturn)
		{
			pReturn->_bPool = false;//���ڳ���
			pReturn->_nID = -1;
			pReturn->_nRef = 1;
			pReturn->_pAlloc = this;
			pReturn->_pNext = nullptr;
		}
	}
	else//����ֱ��ʹ���ڴ��
	{
		pReturn = _pHeader;
		_pHeader = _pHeader->_pNext;
		assert(0 == pReturn->_nRef);
		pReturn->_nRef = 1;
	}
	//debug��ӡ
	if (nullptr != pReturn)
	{
		xPrintf("NEW - allocMem:%p,id=%d,size=%d\n", pReturn, pReturn->_nID, nSize);
	}
	return ((char*)pReturn + sizeof(MemoryBlock));
}

void MemoryAlloc::freeMem(void* p)
{
	//������������Ϣ�� ��Ҫ������Ϣͷ
	MemoryBlock* pBlock = (MemoryBlock*)((char*)p - sizeof(MemoryBlock));
	assert(1 == pBlock->_nRef);
	//�ж��Ƿ񱻶������
	if (--pBlock->_nRef != 0)
	{
		return;
	}
	//�ж��Ƿ����ڴ����
	if (pBlock->_bPool)
	{
		//�Խ���
		std::lock_guard<std::mutex> lock(_mutex);
		//���ڴ������ڴ����λ
		pBlock->_pNext = _pHeader;
		_pHeader = pBlock;
	}
	else
	{
		free(pBlock);
	}
}