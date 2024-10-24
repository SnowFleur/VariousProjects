#pragma once
/*
- Descriptor: C++ �޸� ���������� �޸� Ǯ ����ϴ� �ڵ�
- ����� ��� X (����ϴ� ��� �ּ�����)
*/
#include <cassert>
#include<stdlib.h>
#include <vector>
#include<stack>

#include "../SnowDBCore/C# Api/API_Routher.h"

//
//class FixedMemoryBlock
//{
//private:
//	void* m_memoryHandle;
//	size_t	m_fixedMemorySize;    //  �߶�� �ϴ� �޸� ������
//	size_t	m_maxMemorySize;      //  �� �޸� ������
//public:
//
//	FixedMemoryBlock(size_t size) :
//		m_memoryHandle(nullptr),
//		m_fixedMemorySize(size)
//	{
//		m_memoryHandle = (char*)malloc(m_maxMemorySize);
//	}
//
//	~FixedMemoryBlock()
//	{
//
//	}
//
//	void* Allocate(size_t size)
//	{
//		//�޸� �ڵ��� 1����Ʈ�� �ڸ� �� �ִ� char* ������������ ĳ����
//		char* p = reinterpret_cast<char*>(m_memoryHandle);
//
//		size_t divideMemory = m_maxMemorySize / m_fixedMemorySize;
//
//		//��ϰ��� ���Ḯ��Ʈ ����
//		for (; divideMemory > 1; --divideMemory, p += size)
//		{
//			*reinterpret_cast<char**>(p) = p + size;
//		}
//		//�������� NULL
//		*reinterpret_cast<char**>(p) = nullptr;
//		return m_memoryHandle;
//	}
//
//	bool IsEmpty()const
//	{
//		return false;
//	}
//};

constexpr int32_t MEMORY_POOL_SIZE = 1024 * 100;


// https://3dmpengines.tistory.com/2233
class MemoryPoolHeader
{
private:
	int32_t memorySize_;

	MemoryPoolHeader(const int32_t memorySize):memorySize_(memorySize)
	{

	}

	int32_t GetMemorySize();
};


class FixedMemoryBlock
{
private:
	std::stack<MemoryPoolHeader*>	memoryBlock_;
	const int32_t					blockSize_;
public:

	FixedMemoryBlock(const int32_t blockSize) :blockSize_(blockSize)
	{

	}

	void Push()
	{

	}

	MemoryPoolHeader* Pop()
	{
		MemoryPoolHeader* pHeader = nullptr;

		if (memoryBlock_.empty() == false)
		{
			pHeader = memoryBlock_.top();
			memoryBlock_.pop();
		}
		else
		{
			pHeader = static_cast<MemoryPoolHeader*>(malloc(blockSize_));

			// ������ ���δ�.
			if(pHeader==nullptr) assert(false && "pHeader bad allocated");

		}
		return pHeader;
	}

};

class MultiSizeMemoryPool
{
private:
	
	std::vector<FixedMemoryBlock>	pool_;

public:
	void InitFixedMemorySize(size_t fixedSize)
	{

		m_memoryHandle = (char*)malloc(m_maxMemorySize);

	}

	void* Allocate(const size_t size) const
	{
		return nullptr;
	}

	void Deallocate(void* ptr)
	{

	}

};



