#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include "MemoryPool.h"

constexpr std::array<int32_t, 5> ALLOCATION_SIZES = { 32, 64, 128, 256, 512 };

// 메모리 풀 테스트 함수
void MemoryPoolTest(int32_t iterations)
{
    // 걸린 시간 측정
    auto start = std::chrono::high_resolution_clock::now();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dist(0, ALLOCATION_SIZES.size() - 1);

    std::vector<int*> allocatedPointers;

    try
    {
        for (int32_t i = 0; i < iterations; ++i)
        {
            int32_t size = ALLOCATION_SIZES[dist(gen)]; // 랜덤 할당 크기 선택
            int* p = static_cast<int*>(g_memory_pool.Allocate(size));

            if (p == nullptr)
            {
                throw std::bad_alloc();
            }

            *p = i; // 값 설정
            allocatedPointers.push_back(p); // 할당된 포인터 저장
            std::cout << "Allocated: " << *p << " (Size: " << size << " bytes)" << std::endl;
        }

        // 할당 해제
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

    // 걸린 시간 측정 종료
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Total time for " << iterations << " iterations: " << duration.count() << " seconds" << std::endl;
}

// 메인 함수
int main()
{
    int32_t iterations = 10000; // 반복 횟수
    MemoryPoolTest(iterations);

    return 0;
}
