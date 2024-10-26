#pragma once
#include <stack>
#include <cassert>
#include <cstdlib>
#include<vector>

#include "MemoryPoolConfig.h"

class MemoryBlockStack
{
private:
	std::stack<MemoryPoolHeader*> memoryBlock_;
	const size_t blockSize_;

public:
	explicit MemoryBlockStack(const size_t blockSize) : blockSize_(blockSize) {}

	~MemoryBlockStack() noexcept
	{
		while (!memoryBlock_.empty())
		{
			const auto iter = memoryBlock_.top();
			memoryBlock_.pop();
			if (iter != nullptr)
			{
				free(iter);
			}

		}
	}
	DELETE_COPY(MemoryBlockStack);
	DELETE_MOVE(MemoryBlockStack);
public:
	void AllocateHeaders(const size_t blockCount)
	{
		for (size_t i = 0; i < blockCount; ++i)
		{
			//Malloc 
			MemoryPoolHeader* pHeader = static_cast<MemoryPoolHeader*>(malloc(blockSize_));
			memoryBlock_.push(pHeader);
		}
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
		if (!memoryBlock_.empty())
		{
			MemoryPoolHeader* pHeader = memoryBlock_.top();
			memoryBlock_.pop();
			return pHeader;
		}
		return  static_cast<MemoryPoolHeader*>(malloc(blockSize_));
	}
};

class MemoryPoolManager
{
private:
	std::vector<MemoryBlockStack*> pool_;
private:
	void InitFixedMemorySize(const int32_t index, const int32_t blockSize, const int32_t blockCount)
	{
		pool_[index] = new MemoryBlockStack(blockSize + sizeof(MemoryPoolHeader));
		pool_[index]->AllocateHeaders(blockCount);
	}
public:
	MemoryPoolManager() : pool_(NUM_BLOCK_TYPES)
	{
		for (int32_t i = 0; i < NUM_BLOCK_TYPES; ++i)
		{
			InitFixedMemorySize(i, BLOCK_SIZES[i], BLOCK_COUNTS[i]);
		}
	}

	~MemoryPoolManager()
	{
		for (const auto* block : pool_)
		{
			delete block;
		}
	}
	DELETE_COPY(MemoryPoolManager);
	DELETE_MOVE(MemoryPoolManager);
public:
	[[nodiscard]] void* Allocate(const size_t size) const
	{
		const int32_t index = BLOCK_SIZE_LOOKUP.GetBlockIndex(size);

		// 메모리 풀에 없는 사이즈면 할당해서 넘겨준다.
		if (index == INVALID_INDEX) return malloc(size);

		MemoryPoolHeader* pHeader = pool_[index]->Pop();

		// 생성자만 호출
		new(pHeader) MemoryPoolHeader(size);

		// Header 뒤에 있는 주소값을 리턴
		return pHeader + 1;
	}

	void Deallocate(void* ptr) const
	{
		if (ptr == nullptr) return;

		MemoryPoolHeader* pHeader = static_cast<MemoryPoolHeader*>(ptr) - 1;
		const int32_t size = pHeader->GetMemorySize();

		if (const int32_t index = BLOCK_SIZE_LOOKUP.GetBlockIndex(size); index == INVALID_INDEX)
		{
			//free(pHeader);
		}
		else
		{
			pool_[index]->Push(pHeader);
		}
	}

};

// 전역 메모리 풀 인스턴스
inline MemoryPoolManager g_memory_pool;