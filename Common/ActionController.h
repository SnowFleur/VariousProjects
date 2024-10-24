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

// 반환형이 필요할 수도 있기 때문에 int32_t로 고정 
template<class... _TFuncArgs>
using CallBackAction = std::function<int32_t(_TFuncArgs...)>;

template<class... _TFuncArgs>
class Action :public IAction // 인터페이스 뒤로 템플릿을 숨긴다.
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
		// try_emplace는 중복여부 체크가 가능하며 emplace와 다르게 내부에서 생성을한다.
		// 뭐지 이 혼종은?
		return actionController_.try_emplace(actionKey, std::make_unique<Action<_TFuncArgs...>>(std::forward<CallBackAction< _TFuncArgs...>>(funcion))).second;
	}

	template<class _TKey, class... _TArgs>
	void DoAction(const _TKey actionKey, _TArgs&&... context)
	{
		if (auto findIter = actionController_.find(actionKey); findIter != actionController_.end())
		{
			try
			{
				// 이러면서 까지 스마트포인터를?
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

