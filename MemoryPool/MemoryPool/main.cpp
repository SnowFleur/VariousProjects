#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include "MemoryPool.h"

constexpr std::array<int32_t, 5> ALLOCATION_SIZES = { 32, 64, 128, 256, 512 };

// �޸� Ǯ �׽�Ʈ �Լ�
void MemoryPoolTest(int32_t iterations)
{
    // �ɸ� �ð� ����
    auto start = std::chrono::high_resolution_clock::now();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dist(0, ALLOCATION_SIZES.size() - 1);

    std::vector<int*> allocatedPointers;

    try
    {
        for (int32_t i = 0; i < iterations; ++i)
        {
            int32_t size = ALLOCATION_SIZES[dist(gen)]; // ���� �Ҵ� ũ�� ����
            int* p = static_cast<int*>(g_memory_pool.Allocate(size));

            if (p == nullptr)
            {
                throw std::bad_alloc();
            }

            *p = i; // �� ����
            allocatedPointers.push_back(p); // �Ҵ�� ������ ����
            std::cout << "Allocated: " << *p << " (Size: " << size << " bytes)" << std::endl;
        }

        // �Ҵ� ����
        for (int* p : allocatedPointers)
        {
            g_memory_pool.Deallocate(p);
        }
    }
    catch (const std::bad_alloc& e)
    {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }

    // �ɸ� �ð� ���� ����
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Total time for " << iterations << " iterations: " << duration.count() << " seconds" << std::endl;
}

// ���� �Լ�
int main()
{
    int32_t iterations = 10000; // �ݺ� Ƚ��
    MemoryPoolTest(iterations);

    return 0;
}
