#include "pch.h"

#include"../MemoryPool/MemoryPool.h"


class MemoryPoolTest : public ::testing::Test
{
protected:
    // 테스트마다 새로 생성되는 메모리 풀 인스턴스
    MultiSizeMemoryPool pool;

    void SetUp() override {
        // 각 테스트가 실행되기 전에 초기화가 필요하다면 작성
    }

    void TearDown() override {
        // 각 테스트가 끝난 후에 정리할 작업이 필요하다면 작성
    }
};

TEST_F(MemoryPoolTest, AllocateAndDeallocate)
{
    for (int32_t i = 0; i < 1000; ++i) 
    {
        int* p = (int*)pool.Allocate(4); // 4바이트 할당
        ASSERT_NE(p, nullptr);           // 할당된 메모리가 null이 아닌지 확인
        *p = i;
        EXPECT_EQ(*p, i);                // 값이 제대로 할당되었는지 확인

        pool.Deallocate(p);              // 메모리 해제
    }
}

// 스트레스 테스트를 위한 기본 예시
TEST_F(MemoryPoolTest, StressTest)
{
    MultiSizeMemoryPool pool;

    const int iterations = 1000000; // 반복 횟수
    for (int i = 0; i < iterations; ++i) 
    {
        int* p = (int*)pool.Allocate(4);
        ASSERT_NE(p, nullptr); // 할당이 성공했는지 확인
        *p = i;
        pool.Deallocate(p);
    }

    SUCCEED(); // 모든 테스트가 완료되면 성공 표시
}

//TEST_F(MemoryPoolTest, LargeMemoryAllocation)
//{
//    void* p = pool.Allocate(1024);  // 큰 메모리 할당
//    ASSERT_NE(p, nullptr);          // 할당이 성공했는지 확인
//
//    pool.Deallocate(p);             // 메모리 해제
//}

//
//TEST(TestCaseName, TestName)
//{
//  EXPECT_EQ(1, 1);
//  EXPECT_TRUE(true);
//}