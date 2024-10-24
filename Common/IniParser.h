#pragma once
/*
- Descriptor: Ini파일을 Read하면서 Section과 KeyName, Key 값으로 값을 가지는 클래스
- Key값들사이에 공백이 있으면 안됨
- 다른 클래스도 해당 값을 가져야 하는데...  나중에는 프로스세스가 켜져있는 상태에서도 변경이 가능하도록 할 것임
- 1. 각 클래스 안에 가지고 있는다(메모리가 좀 커지네)
- 2. 서버클래스안에 넣는다.(서버밖에서 쓰는 클래스는 못쓴다.)
- 3. 전역으로 접근할 수 있게 한다(싱글에서만 작동해야한다.)
--> 3번으로 진행
*/

#include<vector>
#include<string>
#include<map>
#include<fstream>
#include<sstream>
#include"LogCollector.h"

constexpr int32_t FILE_PATH_SIZE = 150;

struct IniKey
{
	std::string   keyName;  //키 이름
	std::string   key;      //키에 들어있는 값
public:
	IniKey() :
		keyName{}
		, key{}
	{}
};

class IniFile : public Singleton<IniFile>
{
private:
	//Key: Section, Value: Keys
	std::map<std::string, std::vector<IniKey>>    mapIniFile_;
	wchar_t	currFilePath_[FILE_PATH_SIZE];

	void ReadKey(std::ifstream& in, std::string& section)
	{
		std::vector<IniKey> vecKey;
		vecKey.reserve(30);
		IniKey stKey;
		std::string s;

		while (true)
		{
			try
			{
				//더 이상 Section이 없다면 그만~
				if (in.eof() == true) break;

				getline(in, s);

				size_t ignoreOffset = s.find("=");
				size_t endOffset = s.find(";");

				//주석처리된 문자열
				if (endOffset < ignoreOffset) continue;

				// = 혹은 ; 를 찾지 못했다면 잘못된 문자 혹은 다음 Section이니 넘기자
				if (ignoreOffset == std::string::npos || endOffset == std::string::npos) break;

				stKey.keyName = s.substr(0, ignoreOffset);
				stKey.key = s.substr(ignoreOffset + 1, (endOffset - ignoreOffset) - 1);

				vecKey.emplace_back(std::move(stKey));
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << "\n";

				PRINT_ERROR_LOG(__FUNCTION__, e.what(), "\n");
			}
		}

		mapIniFile_.emplace(std::move(section), std::move(vecKey));
	}

	template<class _Ty>
	_Ty StringConvert(std::string&& value)
	{
		//템플릿 변수의 명시적 초기화
		_Ty convertType{};
		std::istringstream ss(value);
		ss >> convertType;
		return convertType;
	}

	template<class _Ty>
	_Ty StringConvert(std::string& value)
	{
		//템플릿 변수의 명시적 초기화
		_Ty convertType{};
		std::istringstream ss(value);
		ss >> convertType;
		return convertType;
	}

public:
	IniFile() 
	{
		GetCurrentDirectory(FILE_PATH_SIZE, currFilePath_);
	}

	~IniFile() override
	{
		for (auto iter = mapIniFile_.begin(); iter != mapIniFile_.end(); ++iter)
		{
			iter->second.clear();
		}
		mapIniFile_.clear();
	}

	IniFile(const IniFile&&) = delete;
	IniFile(IniFile&&)noexcept = delete;
	IniFile& operator=(const IniFile&) = delete;
	IniFile& operator=(IniFile&&)noexcept = delete;
public:
	//반드시 싱글스레드 혹은 동기화가 보장된 상태에서 실행!
	bool ReadIniFile(const char* pFileName)
	{
		if (pFileName == nullptr) return false;

		// 파일 읽기 준비
		std::ifstream in(pFileName);

		if (in.is_open() == false)
		{
			PRINT_ERROR_LOG("파일을 찾을 수 없습니다!\n");
			return false;
		}

		std::string s;
		while (in) 
		{
			getline(in, s);

			//이거는 Section이다.
			if (s[0] == '[')
			{
				// ], \n 삭제
				s.erase(s.end() - 1, s.end());
				//[ 삭제
				s.erase(s.begin());
				//Key처리
				ReadKey(in, s);
			}
		}
		return true;
	}

	void PrintIniFiles()const
	{
		for (auto mapIter = mapIniFile_.cbegin(); mapIter != mapIniFile_.cend(); ++mapIter)
		{
			std::cout << "Section Name: " << "[" << mapIter->first << "]" << "\n";
			for (auto vecIter = mapIter->second.cbegin(); vecIter != mapIter->second.cend(); ++vecIter)
			{
				std::cout << "Key name: " << vecIter->keyName << "\n";
				std::cout << "Key: " << vecIter->key << "\n";
			}
			std::cout << "---------------------------------------------\n";
		}
	}

	bool GetSection(const std::string& sectionName, std::vector<IniKey>& vecKey)
	{
		auto iter = mapIniFile_.find(sectionName);
		if (iter == mapIniFile_.end()) return false;

		vecKey.reserve(iter->second.size());
		vecKey = iter->second;
		return true;
	}

	//세션이름, 키이름
	template<class _Ty>
	_Ty GetKey(std::string sectionName, std::string keyName)
	{
		auto iter = mapIniFile_.find(sectionName);
		if (iter == mapIniFile_.end())
		{
			PRINT_ERROR_LOG("GetKey() SectionName: ", sectionName, "  KeyName: ", keyName, "\n");
			return _Ty{ static_cast<_Ty>(NULL) };
		}

		for (auto vecIter = iter->second.begin(); vecIter != iter->second.end(); ++vecIter)
		{
			if (vecIter->keyName == keyName) {
				return StringConvert<_Ty>(vecIter->key);
			}
		}
		PRINT_ERROR_LOG("GetKey() SectionName: ", sectionName, "  KeyName: ", keyName, "\n");
		return _Ty{ static_cast<_Ty>(NULL) };
	}
};

//#define GetKeyForIniFile(_Ty,sectionName,keyName) IniFile::GetInstance()->GetKey<_Ty>(sectionName,keyName)
#define g_IniFile  IniFile::GetInstance()