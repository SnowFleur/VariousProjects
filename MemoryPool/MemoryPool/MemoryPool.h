#pragma once
/*
- Descriptor: C++ �޸� ���������� �޸� Ǯ ����ϴ� �ڵ�
- ����� ��� X (����ϴ� ��� �ּ�����)
*/
#include <cassert>
#include<stdlib.h>
#include <vector>
#include<stack>

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
constexpr int32_t MAX_ALLOCATE_SIZE = 1024;

constexpr  int32_t MIN_BLOCK_SIZE = 32;
constexpr  int32_t MAX_BLOCK_SIZE = 1024;


class MemoryPoolHeader
{
private:
	int32_t memorySize_;
public:
	MemoryPoolHeader(const int32_t memorySize) :memorySize_(memorySize)
	{
	}

	constexpr int32_t GetMemorySize()const { return memorySize_; }
	void ResetMemorySize() { memorySize_ = 0; }
};

class FixedMemoryBlock
{
private:
	std::stack<MemoryPoolHeader*>	memoryBlock_;
	const int32_t					blockSize_;
public:

	explicit FixedMemoryBlock(const int32_t blockSize) :blockSize_(blockSize)
	{

	}

	void Push(MemoryPoolHeader* pHeader)
	{
		if (pHeader == nullptr)
		{
			assert(false && "Push pHeader is nullptr");
		}
		pHeader->ResetMemorySize();
		memoryBlock_.push(pHeader);
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
			if (pHeader == nullptr) assert(false && "pHeader bad allocated");

		}
		return pHeader;
	}

};

class MultiSizeMemoryPool
{
private:
	std::vector<FixedMemoryBlock*>	pool_;

public:

	MultiSizeMemoryPool() :pool_(MEMORY_POOL_SIZE)
	{
		int32_t currentIndex = 1;

		int32_t currentBlockSize = MIN_BLOCK_SIZE;

		while (currentIndex <= 2)
		{
			InitFixedMemorySize(currentIndex, currentBlockSize);
			// ���� �ε����� ��� ũ�⸸ŭ ����
			currentIndex += currentBlockSize;

			// ��� ũ��� 2��� ���� (32, 64, 128 ��)
			currentBlockSize *= 2;
		}
	}

	void InitFixedMemorySize(const int32_t startIndex, const int32_t blockSize)
	{
		FixedMemoryBlock* memoryBlock = new FixedMemoryBlock(blockSize);

		for (int i = 0; i < blockSize; ++i) 
		{
			MemoryPoolHeader* pHeader = static_cast<MemoryPoolHeader*>(malloc(blockSize));
			memoryBlock->Push(pHeader);

			pool_[startIndex + i] = memoryBlock;
		}
	}

	void* Allocate(const size_t size)
	{

		MemoryPoolHeader* pHeader = nullptr;

		if (size >= MAX_ALLOCATE_SIZE)
		{
			//������ �ִ� �޸� Ǯ���� ũ�� new �Ҵ�
			pHeader = static_cast<MemoryPoolHeader*>(malloc(size));
		}
		else
		{
			pHeader = pool_[size]->Pop();
		}

		return pHeader;
	}

	void Deallocate(void* ptr)
	{

	}

};



