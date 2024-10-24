#pragma once
/*
- Descriptor:  Log ��� �� �α� �����? --> Log Server�� ����� �͵� ��� (UDP)
- ������ ������ ���� ����ó��
- Todo: File IO ,UDP ��ű��
*/

#include<fstream>
#include"WindowsHeader.h"
#include"GlobalUtilFunctions.h"
#include"DefineUtilty.h"
#include"Singleton.h"
#include"Lock.h"

class CLogCollector :public Singleton<CLogCollector>
{
private:
	std::fstream	fileStream_;
	std::string		fileName_;
	Lock			lock_;
public:
	CLogCollector()
	{
#ifdef _DEBUG
		// �����ȯ�濡���� �ܼ� â ON �� �α� ���
		AllocConsole();
		FILE* consoleStream = nullptr;
		freopen_s(&consoleStream, "CONOUT$", "w", stdout);
#endif
	}
	void InsertLogFileName(const TCHAR* pFileName, const TCHAR* pFileDirectory = nullptr)
	{
		if (fileName_ == "")
		{
			if (pFileDirectory != nullptr)
			{
				CreateDirectory(pFileDirectory, NULL);
				fileName_ += SnowUtility::WcharToString(pFileDirectory);
				fileName_ += "\\";
			}

			fileName_ += SnowUtility::WcharToString(pFileName);
		}
	}

#if VER_CPP_17 // c++17�̻� ( Fold expressions)
	template<class... _Args>
	void PrintLog(_Args... args)
	{
		std::string timeString = SnowUtility::GetCurrentYearToSecond();
		std::cout << timeString;

		auto PrintConvertStr = [](auto& value)
		{
			// if constexpr�� �ش� ������ True�̸� else�� ������ ���� �ʴ´�.
			if constexpr (std::is_same_v<decltype(value), std::wstring&>)
			{
				std::string temp{};
				temp.assign(value.begin(), value.end());
				std::wcout << value;
			}
			else
			{
				std::cout << value;
			}
		};

#ifdef _DEBUG
		((PrintConvertStr(args)), ...);
#endif
		InsertLogFile(timeString, args...);
	}

	template<class... _Args>
	void InsertLogFile(std::string& time, _Args... args)
	{
		if (fileName_ == "") return;

		//RVO
		auto ReturnConvertStr = [](auto& value)->auto
		{
			// if constexpr�� �ش� ������ True�̸� else�� ������ ���� �ʴ´�.
			if constexpr (std::is_same_v<decltype(value), std::wstring&>)
			{
				std::string temp{};
				temp.assign(value.begin(), value.end());
				return temp;
			}
			else
			{
				return value;
			}
		};

		fileStream_.open(fileName_, std::ios::out | std::ios::app);
		if (fileStream_.fail() == false)
		{
			fileStream_ << time;

			//((fileStream_ << args), ...);
			((fileStream_ << ReturnConvertStr(args)), ...);
			fileStream_.close();
		}
	}

#elif VER_CPP_11  //C++11 �̻� 
	template<class _Ty, class... _Args>
	void PrintLog(_Ty message, _Args... args)const
	{
#ifdef _DEBUG
		std::cout << message;
		PrintLog(args...);
#endif
	}

	template<class _Ty>
	void PrintLog(_Ty message)const
	{
		//std::cout << message << "\n";
		std::cout << message;
	}
#else
	//ERROR
#endif //  _MSC_VER 

};

/*
- Message: ��� ����
*/
#define PRINT_LOG(message,...)          CLogCollector::GetInstance()->PrintLog(message,__VA_ARGS__)
#define PRINT_INFO_LOG(message,...)     CLogCollector::GetInstance()->PrintLog("[INFO] ",message,__VA_ARGS__)
#define PRINT_ERROR_LOG(message,...)    CLogCollector::GetInstance()->PrintLog("[ERROR] ",message,__VA_ARGS__)
#define INSERT_LOG_FILE_NAME(x, y)		CLogCollector::GetInstance()->InsertLogFileName(x, y)
