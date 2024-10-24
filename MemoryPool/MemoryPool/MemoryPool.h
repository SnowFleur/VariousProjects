#pragma once
/*
- Descriptor: C++ 메모리 블럭형식으로 메모리 풀 사용하는 코드
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

			// 서버를 죽인다.
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

		// 헤더크기만큼 더 확장해서 할당받는다.
		int32_t currentBlockSize = MIN_BLOCK_SIZE + sizeof(MemoryPoolHeader);

		while (currentIndex <= 2)
		{
			InitFixedMemorySize(currentIndex, 1, currentBlockSize);
			// 다음 인덱스는 블록 크기만큼 증가
			currentIndex += currentBlockSize;

			// 블록 크기는 2배로 증가 (32, 64, 128 등)
			currentBlockSize *= 2;
		}
	}

	~MultiSizeMemoryPool() noexcept
	{
		// 중복 해제를 방지하기 위한 집합
		std::unordered_set<FixedMemoryBlock*> uniqueBlocks;

		for (auto iter = pool_.begin(); iter != pool_.end(); ++iter)
		{
			if (FixedMemoryBlock* block = *iter; block != nullptr && uniqueBlocks.find(block) == uniqueBlocks.end())
			{
				delete block;

				//중복해제 방지 
				uniqueBlocks.insert(block);

				// 해제 후 포인터를 null로 설정하여 중복 delete 방지
				*iter = nullptr;
			}
		}
	}

	void InitFixedMemorySize(const int32_t startIndex, const int32_t blockCount, const int32_t blockSize)
	{
		FixedMemoryBlock* memoryBlock = new FixedMemoryBlock(blockSize);

		// 메모리 블럭 생성
		for (int i = 0; i < blockCount; ++i)
		{
			MemoryPoolHeader* pHeader = static_cast<MemoryPoolHeader*>(malloc(blockSize));
			memoryBlock->Push(pHeader);
		}

		// 해당 크기까지 Index 처리
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
			//가지고 있는 메모리 풀보다 크면 new 할당
			pHeader = static_cast<MemoryPoolHeader*>(malloc(size));
		}
		else
		{
			// 요청한거 + 헤더사이즈 만큼
			pHeader = pool_[size + sizeof(MemoryPoolHeader)]->Pop();

			// 생성자만 호출 
			new(pHeader)MemoryPoolHeader(size);

			// 헤더 크기보다 앞에 있는 포인터 주소를 반환
			pHeader = (pHeader + sizeof(MemoryPoolHeader));
		}
		return pHeader;
	}

	void Deallocate(void* ptr) const
	{
		if (ptr == nullptr) return;

		MemoryPoolHeader* pHeader = static_cast<MemoryPoolHeader*>(ptr);

		// 앞에 있는 헤더 주소로 옮김
		pHeader -= sizeof(MemoryPoolHeader);

		if (pHeader->GetMemorySize() >= MAX_ALLOCATE_SIZE)
		{
			// 더 큰 메모리라면 그냥 지운다.
			free(pHeader);
		}
		else
		{
			// 반납
			pHeader = pool_[pHeader->GetMemorySize() + sizeof(MemoryPoolHeader)]->Pop();
		}
	}

};


static 	MultiSizeMemoryPool g_memory_pool;