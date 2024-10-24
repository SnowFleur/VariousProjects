#pragma once

/*
- Descriptor: �̱����� ����� Ŭ������ ��ӹ޴� �̱��� Class
*/

#include<mutex>
#include"DefineUtilty.h"

template<class _Ty>
class Singleton 
{
private:
    static _Ty* instance_;
	static std::once_flag onceFlag_; // ���� �����忡�� �ҷ��� 1���� �Ҹ��� 
protected:
	Singleton() = default;
public:
	virtual ~Singleton() = default;

    DELETE_COPY(Singleton);
    DELETE_MOVE(Singleton);

    static _Ty* GetInstance() 
    {
        std::call_once(onceFlag_, [&]()
            {
                if (instance_ == nullptr)
                {
                    instance_ = new _Ty();
                }
            });
        return instance_;
    }

    static void Release()
    {
        if (instance_ != nullptr)
        {
            delete instance_;
            instance_ = nullptr;
        }
    }

};


template<class _Ty>
_Ty* Singleton<_Ty>::instance_ = nullptr;

template<class _Ty>
std::once_flag Singleton<_Ty>::onceFlag_;


