#include<iostream>
#include<process.h>
#include<Windows.h>
#include<random>

struct SRW_ReadGuard {
	SRWLOCK& m_lock;
public:
	explicit SRW_ReadGuard(SRWLOCK lock) :m_lock(lock) {
		AcquireSRWLockShared(&m_lock);
	}
	~SRW_ReadGuard() {
		ReleaseSRWLockShared(&m_lock);
	}

	SRW_ReadGuard(const SRW_ReadGuard&) = delete;
	SRW_ReadGuard& operator=(const SRW_ReadGuard&) = delete;
	SRW_ReadGuard(SRW_ReadGuard&&) = delete;
	SRW_ReadGuard& operator=(SRW_ReadGuard&&) = delete;
};



struct SRW_WriteGuard {
	SRWLOCK& m_lock;
public:
	explicit SRW_WriteGuard(SRWLOCK lock) :m_lock(lock) {
		AcquireSRWLockExclusive(&m_lock);
	}
	~SRW_WriteGuard() {
		ReleaseSRWLockExclusive(&m_lock);
	}

	SRW_WriteGuard(const SRW_WriteGuard&) = delete;
	SRW_WriteGuard& operator=(const SRW_WriteGuard&) = delete;
	SRW_WriteGuard(SRW_WriteGuard&&) = delete;
	SRW_WriteGuard& operator=(SRW_WriteGuard&&) = delete;
};





#include<vector>
constexpr int THREAD_COUNT = 4;
constexpr int READ = 0;
constexpr int PUSH = 1;
constexpr int POP = 2;


std::vector<int> g_vector;
SRWLOCK			 g_lock{};
unsigned int __stdcall Thread(void* argPtr) {

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> uid{ 0,2 };

	for (int i = 0;i < 1000;++i) {

		switch (uid(mt)) {
		case READ: {
			SRW_ReadGuard lock{ g_lock };

			break;
		}
		case PUSH: {
			SRW_WriteGuard lock{ g_lock };
			g_vector.push_back(i);
			break;
		}
		case POP: {
			SRW_WriteGuard lock{ g_lock };
			if (g_vector.empty() == false) {
				g_vector.pop_back();
			}
			break;
		}
		default:
			std::cout << "Error\n";
			break;
		}
	}

	_endthreadex(0);
	return 0;
}


int main() {
	InitializeSRWLock(&g_lock);
	g_vector.reserve(1000);

	HANDLE thread[THREAD_COUNT]{};

	for (int i = 0;i < THREAD_COUNT;++i) {
		thread[i] = (HANDLE)_beginthreadex(nullptr, 0, Thread, nullptr, 0, nullptr);
	}

	WaitForMultipleObjects(THREAD_COUNT, thread, TRUE, INFINITE);


	std::cout << "---------------------------------\n";

	for (const auto i : g_vector) {
		std::cout << i << " ";
	}
	std::cout << "\n";
}
