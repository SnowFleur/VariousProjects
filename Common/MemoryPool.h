#pragma once
/*
- Descriptor: C++ 메모리 블럭형식으로 메모리 풀 사용하는 코드
- 현재는 사용 X (사용하는 경우 주석해제)
*/
#include<stdlib.h>


class FixedMemoryBlock
{
private:
	void*	m_memoryHandle;
	size_t	m_fixedMemorySize;    //  잘라야 하는 메모리 사이즈
	size_t	m_maxMemorySize;      //  총 메모리 사이즈
public:

	FixedMemoryBlock(size_t size) :
		m_memoryHandle(nullptr),
		m_fixedMemorySize(size)
	{
		m_memoryHandle = (char*)malloc(m_maxMemorySize);
	}

	~FixedMemoryBlock()
	{
			
	}

	void* Allocate(size_t size)
	{
		//메모리 핸들을 1바이트씩 자를 수 있는 char* 포인터형으로 캐스팅
		char* p = reinterpret_cast<char*>(m_memoryHandle);

		size_t divideMemory = m_maxMemorySize / m_fixedMemorySize;

		//블록간의 연결리스트 생성
		for (; divideMemory > 1; --divideMemory, p += size) 
		{
			*reinterpret_cast<char**>(p) = p + size;
		}
		//마지막은 NULL
		*reinterpret_cast<char**>(p) = nullptr;
		return m_memoryHandle;
	}

	bool IsEmpty()const
	{
		return false;
	}
};

class MemoryPool
{
private:
	struct freeList
	{
		freeList* next;
	};

	FixedMemoryBlock*	fixedMemoryBlock;
	freeList*			m_freeList;
public:
	void InitFixedMemorySize(size_t fixedSize)
	{
		fixedMemoryBlock = new FixedMemoryBlock(fixedSize);
	}

	void* Allocate(const size_t size) const
	{
		void* ptr = nullptr;

		if (fixedMemoryBlock->IsEmpty() == false)
		{
			ptr = fixedMemoryBlock->Allocate(size);
		}
		else
		{

		}
		return ptr;
	}

	void Deallocate(void* ptr)
	{
		if (ptr != nullptr)
		{
			freeList* pFree = static_cast<freeList*>(ptr);
			pFree->next = m_freeList;
			m_freeList = pFree;
		}
	}
};
