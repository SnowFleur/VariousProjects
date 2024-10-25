#pragma once

#include <stack>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <cstdlib>

// 블록 크기 및 개수 상수 정의
constexpr int32_t BLOCK_32_SIZE = 32;
constexpr int32_t BLOCK_32_COUNT = 30;

constexpr int32_t BLOCK_64_SIZE = 64;
constexpr int32_t BLOCK_64_COUNT = 25;

constexpr int32_t BLOCK_128_SIZE = 128;
constexpr int32_t BLOCK_128_COUNT = 20;

constexpr int32_t BLOCK_256_SIZE = 256;
constexpr int32_t BLOCK_256_COUNT = 15;

constexpr int32_t BLOCK_512_SIZE = 512;
constexpr int32_t BLOCK_512_COUNT = 10;

// 블록 설정 관련 상수 정의
constexpr int32_t MIN_BLOCK_SIZE = BLOCK_32_SIZE;
constexpr int32_t MAX_BLOCK_SIZE = BLOCK_512_SIZE;
constexpr int32_t BLOCK_KIND = 5;

class MemoryPoolHeader
{
private:
	int32_t memorySize_;
public:
	explicit MemoryPoolHeader(int32_t memorySize) : memorySize_(memorySize) {}

	constexpr int32_t GetMemorySize() const { return memorySize_; }
	void ResetMemorySize() { memorySize_ = 0; }
};

class FixedMemoryBlock
{
private:
	std::stack<MemoryPoolHeader*> memoryBlock_;
	const int32_t blockSize_;
public:
	explicit FixedMemoryBlock(int32_t blockSize) : blockSize_(blockSize) {}

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

		if (!memoryBlock_.empty())
		{
			pHeader = memoryBlock_.top();
			memoryBlock_.pop();
		}
		else
		{
			pHeader = static_cast<MemoryPoolHeader*>(malloc(blockSize_));
			if (pHeader == nullptr) assert(false && "pHeader bad allocated");
		}
		return pHeader;
	}
};

class MultiSizeMemoryPool
{
private:
	std::vector<FixedMemoryBlock*> pool_;
public:
	MultiSizeMemoryPool() : pool_(MAX_BLOCK_SIZE + sizeof(MemoryPoolHeader), nullptr)
	{
		int32_t currentIndex = 1;

		constexpr int32_t blockSizes[BLOCK_KIND] = { BLOCK_32_SIZE, BLOCK_64_SIZE, BLOCK_128_SIZE, BLOCK_256_SIZE, BLOCK_512_SIZE };
		constexpr int32_t blockCounts[BLOCK_KIND] = { BLOCK_32_COUNT, BLOCK_64_COUNT, BLOCK_128_COUNT, BLOCK_256_COUNT, BLOCK_512_COUNT };

		for (int i = 0; i < BLOCK_KIND; ++i)
		{
			int32_t currentBlockSize = blockSizes[i] + sizeof(MemoryPoolHeader);
			InitFixedMemorySize(currentIndex, blockCounts[i], currentBlockSize);
			currentIndex += currentBlockSize;
		}
	}

	~MultiSizeMemoryPool() noexcept
	{
		std::unordered_set<FixedMemoryBlock*> uniqueBlocks;
		for (auto& block : pool_)
		{
			if (block != nullptr && uniqueBlocks.find(block) == uniqueBlocks.end())
			{
				delete block;
				uniqueBlocks.insert(block);
				block = nullptr;
			}
		}
	}

	void InitFixedMemorySize(int32_t startIndex, int32_t blockCount, int32_t blockSize)
	{
		FixedMemoryBlock* memoryBlock = new FixedMemoryBlock(blockSize);

		for (int i = 0; i < blockCount; ++i)
		{
			MemoryPoolHeader* pHeader = static_cast<MemoryPoolHeader*>(malloc(blockSize));
			memoryBlock->Push(pHeader);
		}

		for (int i = 0; i < blockSize; ++i)
		{
			pool_[startIndex + i] = memoryBlock;
		}
	}

	[[nodiscard]] void* Allocate(size_t size) const
	{
		if (size == 0) return nullptr;

		MemoryPoolHeader* pHeader = nullptr;

		if (size >= MAX_BLOCK_SIZE)
		{
			pHeader = static_cast<MemoryPoolHeader*>(malloc(size));
		}
		else
		{
			pHeader = pool_[size + sizeof(MemoryPoolHeader)]->Pop();
			new(pHeader) MemoryPoolHeader(size);
			pHeader = (pHeader + sizeof(MemoryPoolHeader));
		}
		return pHeader;
	}

	void Deallocate(void* ptr) const
	{
		if (ptr == nullptr) return;

		MemoryPoolHeader* pHeader = static_cast<MemoryPoolHeader*>(ptr);
		pHeader -= sizeof(MemoryPoolHeader);

		if (pHeader->GetMemorySize() >= MAX_BLOCK_SIZE)
		{
			free(pHeader);
		}
		else
		{
			pool_[pHeader->GetMemorySize() + sizeof(MemoryPoolHeader)]->Push(pHeader);
		}
	}
};

static MultiSizeMemoryPool g_memory_pool;