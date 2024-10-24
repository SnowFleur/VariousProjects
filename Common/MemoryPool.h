#pragma once
/*
- Descriptor: C++ �޸� ���������� �޸� Ǯ ����ϴ� �ڵ�
- ����� ��� X (����ϴ� ��� �ּ�����)
*/
#include<stdlib.h>


class FixedMemoryBlock
{
private:
	void*	m_memoryHandle;
	size_t	m_fixedMemorySize;    //  �߶�� �ϴ� �޸� ������
	size_t	m_maxMemorySize;      //  �� �޸� ������
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
		//�޸� �ڵ��� 1����Ʈ�� �ڸ� �� �ִ� char* ������������ ĳ����
		char* p = reinterpret_cast<char*>(m_memoryHandle);

		size_t divideMemory = m_maxMemorySize / m_fixedMemorySize;

		//��ϰ��� ���Ḯ��Ʈ ����
		for (; divideMemory > 1; --divideMemory, p += size) 
		{
			*reinterpret_cast<char**>(p) = p + size;
		}
		//�������� NULL
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
