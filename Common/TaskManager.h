#pragma once
#include<map>
#include<functional>
#include<chrono>
#include<queue>

#include"../Common/Singleton.h"
#include"../Common/LogCollector.h"
#include"../Common/SnowThread.h"
#include"../Common/Lock.h"

template<typename  _Task>
class TaskManager;

template<class _Task>
class TaskThread : public SnowThread
{
private:
	std::queue<_Task*>   queTask_;       //Object Pool 연동
	Lock						lock_;

	//매번 빼고 락 거는건 비효율이니까 한번에 여러개를 담자
	void FlushTask(std::vector<_Task*>& vecTask, uint16_t& flushSize)
	{

		EXCLUSIVE_LOCK_GUARD(1, lock_);
		if (queTask_.empty())
		{
			flushSize = 0;
			return;
		}

		flushSize = static_cast<uint16_t>(flushSize > queTask_.size() ? queTask_.size() : flushSize);

		//스왑을 쓸까 차라리?
		vecTask.reserve(flushSize);
		for (int32_t i = 0; i < flushSize; ++i)
		{
			vecTask.emplace_back(std::move(queTask_.front()));
			queTask_.pop();
		}
	}
	
	void PushTask(_Task* pTask)
	{
		EXCLUSIVE_LOCK_GUARD(1, lock_);
		if (pTask != nullptr)
		{
			queTask_.push(pTask);
		}
	}

protected:
	//NRVO --->되나? 안될 거 같은데 -->체크
	std::vector<_Task*> PopTask()
	{
		std::vector<_Task*> tempTask;
		uint16_t flushCount = 300;
		FlushTask(tempTask, flushCount);
		return tempTask;
	}

public:
	//그대로 올려보내자
	template<class _Ty, class... _TArgs>
	TaskThread(_Ty&& ftn, _TArgs&&... args) :
		SnowThread(std::forward<_Ty>(ftn)
			, std::forward<_TArgs>(args)...)
	{}

	virtual ~TaskThread()
	{
		while (queTask_.empty() == false)
		{
			SAFE_DELETE(queTask_.front());
			queTask_.pop();
		}
	}

	virtual uint32_t Execute(const ThreadID threadID) = 0;

	friend class TaskManager<_Task>;
};

template <class _Ttask>
class TaskManager
{
private:
	//Task Dispatcher
	std::map<uint32_t, TaskThread<_Ttask>*> taskThreads_;
public:

	TaskManager() = default;

	virtual ~TaskManager()
	{
		for (int32_t i = 0; i < taskThreads_.size(); ++i)
		{
#ifdef USED_NEW_DELETE_FUNCTION
			SAFE_DELETE(taskThreads_[i]);
#endif 
		}
		taskThreads_.clear();
	}
public:
	size_t GetTaskThreadSize() const
	{
		return taskThreads_.size();
	}

	//반드시 싱글스레드에서 실행
	void AddTaskThread(const uint32_t threadKey, TaskThread<_Ttask>* pTaskThread)
	{
		if (pTaskThread == nullptr) return;
		taskThreads_.emplace(threadKey, pTaskThread);
	}

	void WaitForTaskThread()
	{
		for (auto& taskThread : taskThreads_)
		{
			taskThread.second->WaitForThread();
		}
	}

	TaskThread<_Ttask>* GetTaskThread(const int32_t threadKey)
	{
		TaskThread<_Ttask>* pTemp = nullptr;

		auto findIter = taskThreads_.find(threadKey);
		if (findIter != taskThreads_.end())
		{
			pTemp = findIter->second;
		}
		return pTemp;
	}

	void PushTask(const uint32_t key, _Ttask* pTask)
	{
		auto pMapIter = taskThreads_.find(key);
		if (pMapIter != taskThreads_.end())
		{
			pMapIter->second->PushTask(pTask);
		}
		else
		{
			PRINT_ERROR_LOG("Push Task Can't Find Key : ", key, "\n");
		}
	}

};
