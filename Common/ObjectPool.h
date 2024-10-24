#pragma once
/*
- Descriptor: ����Ʈ������ ��� ObjectǮ
- Shared �����͸� ����ϸ鼭 GC�� ������ �� �� �ְ� �Ѵ�.
- To do Operator new�� ���� �޸� �Ҵ� ������ �ʿ���
*/

#define PRINT_MEMORY_ADDRESS
#include<memory>
#include<stack>
#include<iostream>

#define VER2		//GetFreeObject�Լ��� �Ű������� ���� ����

template<class _Ty>
class ObjectPool
{
private:
	std::stack<std::unique_ptr<_Ty>>	objectPool_;
	size_t	objectPoolSize_;
public:
	ObjectPool(const uint32_t objectPoolSize):
		objectPoolSize_(objectPoolSize)
	{
	}

	virtual ~ObjectPool() 
	{
		objectPoolSize_ = 0;
	}

	template<class... _TArgs>
	void InitFreeObject(_TArgs&&... args)
	{
		for (uint32_t i = 0; i < objectPoolSize_; ++i)
		{
			objectPool_.emplace(std::make_unique<_Ty>(std::forward<_TArgs>(args)...));
		}
	}

	void InitFreeObject(std::unique_ptr<_Ty> pFreeObject)
	{
		if (pFreeObject != nullptr)
		{
			objectPool_.emplace(std::move(pFreeObject));
		}
	}

	size_t GetObjectPoolSize()const
	{
		return objectPoolSize_;
	}

#ifdef VER2
	std::shared_ptr<_Ty> GetFreeObject()
	{
		std::unique_ptr<_Ty> pObject = nullptr;

		//������� �ʴٸ� ������ �ִ� �����͸��ش�.
		if (objectPool_.empty() == false)
		{
			pObject = std::move(objectPool_.top());
			objectPool_.pop();
		}
		else 
		{
			PRINT_ERROR_LOG(typeid(_Ty).name(), " Pool is Empty\n");
		}

		//8byte=(������+ ��Ʈ�Ѻ�)
		std::shared_ptr<_Ty> pMemory
		(
			pObject.release()
			, [this](_Ty* p)
			{
				std::cout << "Ŀ���� ������\n";
				if (p != nullptr)
				{
					objectPool_.emplace(p);
				}
			}
		);
		return pMemory;
	}
#else
	template<class... _Args>
	std::shared_ptr<_Ty> GetFreeObject(_Args&&... args)
	{
		std::unique_ptr<_Ty> pObject = nullptr;

		//������� �ʴٸ� ������ �ִ� �����͸��ش�.
		if (objectPool_.empty() == false)
		{
			pObject = std::move(objectPool_.top());
		}
		//�����ϴٸ� ���� ������
		else
		{
			pObject = std::make_unique<_Ty>(std::forward<_Args>(args)...);
		}
		//8byte=(������+ ��Ʈ�Ѻ�)
		std::shared_ptr<_Ty> pMemory
		(
			pObject.release()
			, [this](_Ty* p)
			{
				std::cout << "Ŀ���� ������\n";
				if (p != nullptr)
				{
					objectPool_.emplace(p);
				}
			}
		);
		return pMemory;
	}
#endif // VER2
	
};
