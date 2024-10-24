#pragma once
/*
- Descriptor: 여러곳에서 사용할 수 있는 유용한 재사용 가능한 함수들 모음 ----> 제네릭 해야한다.
*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define GLOBAL_FUNCTION static 

#include"WindowsHeader.h"
#include <sql.h>
#include <sqlext.h>
#include"DefineUtilty.h"

#include<iostream>
#include<time.h>
#include<chrono>

namespace SnowUtility
{
#if VER_CPP_20 && USED_CPP20_MODULES
#else
	template<class _Ty>
	GLOBAL_FUNCTION inline bool SafeDeleteRawPointer(_Ty* pPointer)
	{
		if (pPointer != nullptr)
		{
			delete pPointer;
			return true;
		}
		return false;
	}

	template<class _Ty>
	GLOBAL_FUNCTION inline bool SafeDeleteArrayRawPointer(_Ty* pPointer)
	{
		if (pPointer != nullptr)
		{
			delete[] pPointer;
			return true;
		}
		return false;
	}

	GLOBAL_FUNCTION inline bool IsNullPointer(void* pPointer)
	{
		return pPointer == nullptr;
	}

	template <class _Ty>
	GLOBAL_FUNCTION inline bool IsNullPointer(const _Ty* pPointer)
	{
		return pPointer == nullptr;
	}

	template <class _Ty>
	GLOBAL_FUNCTION inline bool IsNullPointer(std::shared_ptr<_Ty> pPointer)
	{
		return pPointer == nullptr ? true : false;
	}

	template <class _Ty>
	GLOBAL_FUNCTION inline bool IsNullPointer(std::unique_ptr<_Ty> pPointer)
	{
		return pPointer == nullptr ? true : false;
	}
#endif

	GLOBAL_FUNCTION inline std::string GetCurrentYearToSecond()
	{
		tm dateInfo;
		time_t timer = time(NULL);
		if (localtime_s(&dateInfo, &timer) != 0)
		{
			return std::string{ "" };
		}

		char buff[255];
		memset(buff, 0, 255);
		//ZeroMemory(buff, 255);

		sprintf_s(buff, 255, "[%04d-%02d-%02d-%02d:%02d:%02d] ",
			dateInfo.tm_year + 1900, dateInfo.tm_mon + 1, dateInfo.tm_mday,
			dateInfo.tm_hour, dateInfo.tm_min, dateInfo.tm_sec);
		return buff;
	}

	// #include <atlconv.h>
	//GLOBAL_FUNCTION inline std::string WcharToString(const WCHAR* pData)
	//{
	//	if (pData == nullptr)
	//	{
	//		return std::string{};
	//	}

	//	// https://www.gpgstudy.com/forum/viewtopic.php?t=7047
	//	USES_CONVERSION;
	//	return std::string(W2A(pData));

	//}

	GLOBAL_FUNCTION inline std::string WcharToString(const WCHAR* pData)
	{
		// https://armful-log.tistory.com/5
		std::string str{};

		if (pData == nullptr)
		{
			return str;
		}
		int len = WideCharToMultiByte(CP_ACP, 0, pData, -1, NULL, 0, NULL, NULL);

		char* multiByte = new char[len];
		WideCharToMultiByte(CP_ACP, 0, pData, -1, multiByte, len, NULL, NULL);

		str = multiByte;

		SAFE_DELETE_MULTI(multiByte);
		return str;
	}

	GLOBAL_FUNCTION inline void OdbcError(const std::wstring& message, SQLRETURN ret,
		const SQLSMALLINT handleType, const SQLHANDLE handle)
	{
		SQLSMALLINT index = 1;
		SQLWCHAR sqlState[MAX_PATH] = { 0 };
		SQLINTEGER nativeErr = 0;
		SQLWCHAR errMsg[MAX_PATH] = { 0 };
		SQLSMALLINT msgLen = 0;
		//SQLRETURN errorRet = 0;

		// to do 코루틴?? 
		while (true)
		{
			ret = SQLGetDiagRecW(handleType, handle, index, sqlState,
				&nativeErr, errMsg, _countof(errMsg), &msgLen);

			if (ret == SQL_NO_DATA) break;
			if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) break;

			std::wcout.imbue(std::locale("kor"));
			std::wcout << message << errMsg << std::endl;
			index++;
		}
	}


	namespace  Math
	{
		GLOBAL_FUNCTION void Test(){}
	}

};