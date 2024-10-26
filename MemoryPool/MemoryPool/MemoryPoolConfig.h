#pragma once
#include <array>
#include <cstdint>


// �Ʒ� 2���� ���������� ���� �������� �������� ����
constexpr std::array BLOCK_SIZES = { 32, 64, 128, 256, 512 };
constexpr std::array BLOCK_COUNTS = { 30, 25, 20, 15, 10 };

constexpr int32_t NUM_BLOCK_TYPES = BLOCK_SIZES.size();
constexpr int32_t MAX_BLOCK_SIZE = BLOCK_SIZES[NUM_BLOCK_TYPES - 1];
constexpr int32_t INVALID_INDEX = -1;

class MemoryPoolHeader
{
private:
    size_t memorySize_;
public:
    explicit MemoryPoolHeader(const size_t memorySize) : memorySize_(memorySize) {}

    // ��ȯ���� constexpr �̸� const, constexpr ��� ����
    [[nodiscard]] constexpr size_t GetMemorySize() const { return memorySize_; }
    void ResetMemorySize() { memorySize_ = 0; }
};

class BlockSizeLookup
{
private:
	std::array<size_t, MAX_BLOCK_SIZE + 1> indexMap_;
public:
	constexpr BlockSizeLookup()
		// ���� ǥ�������� constexpr std::array �ʱ�ȭ
		: indexMap_([]() constexpr
			{
				std::array<size_t, MAX_BLOCK_SIZE + 1> indexMap{};
				indexMap.fill(INVALID_INDEX);

				for (int32_t i = 0; i < NUM_BLOCK_TYPES; ++i)
				{
					for (int32_t j = (i == 0 ? 1 : BLOCK_SIZES[i - 1] + 1); j <= BLOCK_SIZES[i]; ++j)
					{
						indexMap[j] = i;
					}
				}
				return indexMap;
			}()) {}

	[[nodiscard]] constexpr size_t GetBlockIndex(size_t size) const
	{
		return (size <= MAX_BLOCK_SIZE) ? indexMap_[size] : INVALID_INDEX;
	}
};


constexpr BlockSizeLookup BLOCK_SIZE_LOOKUP;
