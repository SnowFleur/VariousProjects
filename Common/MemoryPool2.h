#pragma once
/*
- Descriptor: C++ 메모리 블럭형식으로 메모리 풀 사용하는 코드
- 현재는 사용 X (사용하는 경우 주석해제)
*/
#include <cassert>
#include<stdlib.h>
#include <vector>
#include<stack>

#include "../SnowDBCore/C# Api/API_Routher.h"

//
//class FixedMemoryBlock
//{
//private:
//	void* m_memoryHandle;
//	size_t	m_fixedMemorySize;    //  잘라야 하는 메모리 사이즈
//	size_t	m_maxMemorySize;      //  총 메모리 사이즈
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
//		//메모리 핸들을 1바이트씩 자를 수 있는 char* 포인터형으로 캐스팅
//		char* p = reinterpret_cast<char*>(m_memoryHandle);
//
//		size_t divideMemory = m_maxMemorySize / m_fixedMemorySize;
//
//		//블록간의 연결리스트 생성
//		for (; divideMemory > 1; --divideMemory, p += size)
//		{
//			*reinterpret_cast<char**>(p) = p + size;
//		}
//		//마지막은 NULL
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


// https://3dmpengines.tistory.com/2233
class MemoryPoolHeader
{
private:
	int32_t memorySize_;

	MemoryPoolHeader(const int32_t memorySize):memorySize_(memorySize)
	{

	}

	int32_t GetMemorySize();
};


class FixedMemoryBlock
{
private:
	std::stack<MemoryPoolHeader*>	memoryBlock_;
	const int32_t					blockSize_;
public:

	FixedMemoryBlock(const int32_t blockSize) :blockSize_(blockSize)
	{

	}

	void Push()
	{

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
			if(pHeader==nullptr) assert(false && "pHeader bad allocated");

		}
		return pHeader;
	}

};

class MultiSizeMemoryPool
{
private:
	
	std::vector<FixedMemoryBlock>	pool_;

public:
	void InitFixedMemorySize(size_t fixedSize)
	{

		m_memoryHandle = (char*)malloc(m_maxMemorySize);

	}

	void* Allocate(const size_t size) const
	{
		return nullptr;
	}

	void Deallocate(void* ptr)
	{

	}

};



