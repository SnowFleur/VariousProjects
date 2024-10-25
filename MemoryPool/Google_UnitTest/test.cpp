#include "pch.h"

#include"../MemoryPool/MemoryPool.h"


class MemoryPoolTest : public ::testing::Test
{
protected:
    // �׽�Ʈ���� ���� �����Ǵ� �޸� Ǯ �ν��Ͻ�
    MultiSizeMemoryPool pool;

    void SetUp() override {
        // �� �׽�Ʈ�� ����Ǳ� ���� �ʱ�ȭ�� �ʿ��ϴٸ� �ۼ�
    }

    void TearDown() override {
        // �� �׽�Ʈ�� ���� �Ŀ� ������ �۾��� �ʿ��ϴٸ� �ۼ�
    }
};

TEST_F(MemoryPoolTest, AllocateAndDeallocate)
{
    for (int32_t i = 0; i < 1000; ++i) 
    {
        int* p = (int*)pool.Allocate(4); // 4����Ʈ �Ҵ�
        ASSERT_NE(p, nullptr);           // �Ҵ�� �޸𸮰� null�� �ƴ��� Ȯ��
        *p = i;
        EXPECT_EQ(*p, i);                // ���� ����� �Ҵ�Ǿ����� Ȯ��

        pool.Deallocate(p);              // �޸� ����
    }
}

// ��Ʈ���� �׽�Ʈ�� ���� �⺻ ����
TEST_F(MemoryPoolTest, StressTest)
{
    MultiSizeMemoryPool pool;

    const int iterations = 1000000; // �ݺ� Ƚ��
    for (int i = 0; i < iterations; ++i) 
    {
        int* p = (int*)pool.Allocate(4);
        ASSERT_NE(p, nullptr); // �Ҵ��� �����ߴ��� Ȯ��
        *p = i;
        pool.Deallocate(p);
    }

    SUCCEED(); // ��� �׽�Ʈ�� �Ϸ�Ǹ� ���� ǥ��
}

//TEST_F(MemoryPoolTest, LargeMemoryAllocation)
//{
//    void* p = pool.Allocate(1024);  // ū �޸� �Ҵ�
//    ASSERT_NE(p, nullptr);          // �Ҵ��� �����ߴ��� Ȯ��
//
//    pool.Deallocate(p);             // �޸� ����
//}

//
//TEST(TestCaseName, TestName)
//{
//  EXPECT_EQ(1, 1);
//  EXPECT_TRUE(true);
//}