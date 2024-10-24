#pragma once
/*
- Descriptor: Thread ID ��� Task Ŭ������
- Raw Pointer, Smart Pointer ver
*/
#include<map>
#include<functional>
#include<chrono>
#include<queue>
#include<memory>

#include"../Common/Singleton.h"
#include"../Common/LogCollector.h"
#include"../Common/SnowThread.h"
#include"../Common/Lock.h"

template<class _Task>
class TaskManagerWithSmartPtr;

template<class _Task>
class TaskThreadWithSmartPtr : public SnowThread
{
private:
	std::queue <std::shared_ptr<_Task>>	taskQueue_;
	Lock	lock_;

	//�Ź� ���� �� �Ŵ°� ��ȿ���̴ϱ� �ѹ��� �������� ����
	void FlushTask(std::vector<std::shared_ptr<_Task>>& vecTask, uint16_t& flushSize)
	{

		EXCLUSIVE_LOCKGUARD(1, lock_);
		if (taskQueue_.empty())
		{
			flushSize = 0;
			return;
		}

		flushSize = static_cast<uint16_t>(flushSize > taskQueue_.size() ? taskQueue_.size() : flushSize);

		//������ ���� ����?
		vecTask.reserve(flushSize);
		for (int32_t i = 0; i < flushSize; ++i)
		{
			vecTask.emplace_back(std::move(taskQueue_.front()));
			taskQueue_.pop();
		}
	}

	void PushTask(std::shared_ptr<_Task>&& pTask)
	{
		EXCLUSIVE_LOCKGUARD(1, lock_);
		if (pTask != nullptr)
		{
			taskQueue_.push(pTask);
		}
	}

protected:
	//NRVO --->�ǳ�? �ȵ� �� ������ -->üũ
	std::vector<std::shared_ptr<_Task>> PopTask()
	{
		std::vector<std::shared_ptr<_Task>> tempTask;
		uint16_t flushCount = 300;
		FlushTask(tempTask, flushCount);
		return tempTask;
	}

public:
	//�״�� �÷�������
	template<class _Ty, class... _Args>
	TaskThreadWithSmartPtr(_Ty&& ftn, _Args&&... args) :
		SnowThread(std::forward<_Ty>(ftn)
			, std::forward<_Args>(args)...)
	{}

	virtual ~TaskThreadWithSmartPtr()
	{
		while (taskQueue_.empty() == false)
		{
			taskQueue_.pop();
		}
	}

	virtual uint32_t Excute(const ThreadID threadID) = 0;

	friend class TaskManagerWithSmartPtr<_Task>;
};

template <class _Task>
class TaskManagerWithSmartPtr
{
private:
	//Task Dispatcher
	std::map<uint32_t, std::shared_ptr<TaskThreadWithSmartPtr<_Task>>> taskThreads_;
public:

	TaskManagerWithSmartPtr() = default;

	virtual ~TaskManagerWithSmartPtr()
	{
		taskThreads_.clear();
	}
public:
	size_t GetmapTaskThreadSize() const { return taskThreads_.size(); }

	//�ݵ�� �̱۽����忡�� ����
	void AddTaskThread(const uint32_t threadKey, std::shared_ptr<TaskThreadWithSmartPtr<_Task>>&& pTaskThread) noexcept
	{
		if (pTaskThread == nullptr) return;
		taskThreads_.emplace(threadKey, std::move(pTaskThread));
	}

	void AllStopTaskThread()
	{
		for (auto& taskThread : taskThreads_)
		{
			taskThread.second->StopThread();
		}
	}

	void WaitForTaskThread()
	{
		for (auto& taskThread : taskThreads_)
		{
			taskThread.second->WaitForThread();
		}
	}

	std::shared_ptr<TaskThreadWithSmartPtr<_Task>> GetTaskThread(const int32_t taskThreadNumber)
	{
		std::shared_ptr<TaskThreadWithSmartPtr<_Task>> pTempThread = nullptr;

		auto findIter = taskThreads_.find(taskThreadNumber);
		if (findIter != taskThreads_.end())
		{
			pTempThread = findIter->second;
		}
		return pTempThread;
	}

	void PushTask(const uint32_t key, std::shared_ptr<_Task>&& pTask)
	{
		auto pMapIter = taskThreads_.find(key);
		if (pMapIter != taskThreads_.end())
		{
			pMapIter->second->PushTask(std::move(pTask));
		}
		else
		{
			PRINT_ERROR_LOG("Push Task Can't Find Key : ", key, "\n");
		}
	}
};
