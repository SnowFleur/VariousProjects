#pragma once
/*
- Descriptor: C++ �޸� ���������� �޸� Ǯ ����ϴ� �ڵ�
*/
#include <cassert>
#include<stdlib.h>
#include <vector>
#include<stack>
#include<unordered_set>

constexpr int32_t MEMORY_POOL_SIZE = 1024 * 100;
constexpr int32_t MAX_ALLOCATE_SIZE = 1024;

constexpr  int32_t MIN_BLOCK_SIZE = 4;
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

	MultiSizeMemoryPool() :pool_(MEMORY_POOL_SIZE, nullptr)
	{
		int32_t currentIndex = 1;

		// ���ũ�⸸ŭ �� Ȯ���ؼ� �Ҵ�޴´�.
		int32_t currentBlockSize = MIN_BLOCK_SIZE + sizeof(MemoryPoolHeader);

		while (currentIndex <= 2)
		{
			InitFixedMemorySize(currentIndex, 1, currentBlockSize);
			// ���� �ε����� ��� ũ�⸸ŭ ����
			currentIndex += currentBlockSize;

			// ��� ũ��� 2��� ���� (32, 64, 128 ��)
			currentBlockSize *= 2;
		}
	}

	~MultiSizeMemoryPool() noexcept
	{
		// �ߺ� ������ �����ϱ� ���� ����
		std::unordered_set<FixedMemoryBlock*> uniqueBlocks;

		for (auto iter = pool_.begin(); iter != pool_.end(); ++iter)
		{
			if (FixedMemoryBlock* block = *iter; block != nullptr && uniqueBlocks.find(block) == uniqueBlocks.end())
			{
				delete block;

				//�ߺ����� ���� 
				uniqueBlocks.insert(block);

				// ���� �� �����͸� null�� �����Ͽ� �ߺ� delete ����
				*iter = nullptr;
			}
		}
	}

	void InitFixedMemorySize(const int32_t startIndex, const int32_t blockCount, const int32_t blockSize)
	{
		FixedMemoryBlock* memoryBlock = new FixedMemoryBlock(blockSize);

		// �޸� �� ����
		for (int i = 0; i < blockCount; ++i)
		{
			MemoryPoolHeader* pHeader = static_cast<MemoryPoolHeader*>(malloc(blockSize));
			memoryBlock->Push(pHeader);
		}

		// �ش� ũ����� Index ó��
		for (int i = 0; i < blockSize; ++i)
		{
			pool_[startIndex + i] = memoryBlock;
		}
	}

	void* Allocate(const size_t size) const
	{

		if (size == 0) return nullptr;

		MemoryPoolHeader* pHeader = nullptr;

		if (size >= MAX_ALLOCATE_SIZE)
		{
			//������ �ִ� �޸� Ǯ���� ũ�� new �Ҵ�
			pHeader = static_cast<MemoryPoolHeader*>(malloc(size));
		}
		else
		{
			// ��û�Ѱ� + ��������� ��ŭ
			pHeader = pool_[size + sizeof(MemoryPoolHeader)]->Pop();

			// �����ڸ� ȣ�� 
			new(pHeader)MemoryPoolHeader(size);

			// ��� ũ�⺸�� �տ� �ִ� ������ �ּҸ� ��ȯ
			pHeader = (pHeader + sizeof(MemoryPoolHeader));
		}
		return pHeader;
	}

	void Deallocate(void* ptr) const
	{
		if (ptr == nullptr) return;

		MemoryPoolHeader* pHeader = static_cast<MemoryPoolHeader*>(ptr);

		// �տ� �ִ� ��� �ּҷ� �ű�
		pHeader -= sizeof(MemoryPoolHeader);

		if (pHeader->GetMemorySize() >= MAX_ALLOCATE_SIZE)
		{
			// �� ū �޸𸮶�� �׳� �����.
			free(pHeader);
		}
		else
		{
			// �ݳ�
			pHeader = pool_[pHeader->GetMemorySize() + sizeof(MemoryPoolHeader)]->Pop();
		}
	}

};


static 	MultiSizeMemoryPool g_memory_pool;