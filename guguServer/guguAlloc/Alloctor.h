/*
* ���ļ���������new/delete����
* ʹnew/delete�����ڴ��
* 2021/4/22
*/
#ifndef _Alloctor_h_
#define _Alloctor_h_

void* operator new(size_t size);
void operator delete(void* p);
void* operator new[](size_t size);
void operator delete[](void* p);
void* mem_alloc(size_t size);
void mem_free(void* p);

#endif