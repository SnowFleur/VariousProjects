#pragma once

#include<memory>
#include<map>
#include<functional>
#include"../Common/LogCollector.h"

#define VER1

// Type-Erasure
class IAction 
{
public:
	virtual ~IAction() = default;
	virtual uint32_t GetKey()const = 0;
};

// ��ȯ���� �ʿ��� ���� �ֱ� ������ int32_t�� ���� 
template<class... _TFuncArgs>
using CallBackAction = std::function<int32_t(_TFuncArgs...)>;

template<class... _TFuncArgs>
class Action :public IAction // �������̽� �ڷ� ���ø��� �����.
{
private:
	uint32_t		key_;
	CallBackAction< _TFuncArgs...>	callBackActionFunc_;
public:

	Action(CallBackAction< _TFuncArgs...>&& newCallBack)
	{
		callBackActionFunc_ = std::forward<CallBackAction< _TFuncArgs...>>(newCallBack);
	}
	~Action() override = default;

	uint32_t GetKey()const override
	{
		return key_;
	}

	void DoAction(_TFuncArgs&&... args)
	{
		callBackActionFunc_(std::forward<_TFuncArgs>(args)...);
	}
};

template<class _Tkey>
class ActionController
{
private:
#ifdef VER1
	std::map<_Tkey, std::unique_ptr<IAction>> actionController_;
#else
	std::map<_Tkey, std::shared_ptr<IAction>> actionController_;
#endif
public:
	template<class _TKey, class... _TFuncArgs>
	bool AddAction(const _TKey actionKey, CallBackAction< _TFuncArgs...>&& funcion )noexcept
	{
		// try_emplace�� �ߺ����� üũ�� �����ϸ� emplace�� �ٸ��� ���ο��� �������Ѵ�.
		// ���� �� ȥ����?
		return actionController_.try_emplace(actionKey, std::make_unique<Action<_TFuncArgs...>>(std::forward<CallBackAction< _TFuncArgs...>>(funcion))).second;
	}

	template<class _TKey, class... _TArgs>
	void DoAction(const _TKey actionKey, _TArgs&&... context)
	{
		if (auto findIter = actionController_.find(actionKey); findIter != actionController_.end())
		{
			try
			{
				// �̷��鼭 ���� ����Ʈ�����͸�?
#ifdef VER1
				static_cast<Action<_TArgs...>*>(findIter->second.get())->DoAction(std::forward<_TArgs>(context)...);
#else
				std::static_pointer_cast<Action<_TArgs...>>(findIter->second)->DoAction(std::forward<_TArgs>(context)...);
#endif
			}
			catch (const std::exception& ex)
			{
				PRINT_ERROR_LOG(__FUNCTION__, ex.what(), "\n");
			}
			catch (...)
			{
				PRINT_ERROR_LOG(__FUNCTION__, "oh my god", "\n");
			}
		}
		else
		{
			if (std::is_convertible_v<_TKey, int32_t>)
			{
				PRINT_ERROR_LOG(__FUNCTION__, " Find Fail", static_cast<int32_t>(actionKey), "\n");
			}
			else
			{
				PRINT_ERROR_LOG(__FUNCTION__, " Find Fail \n");
			}
		}
	}
};

