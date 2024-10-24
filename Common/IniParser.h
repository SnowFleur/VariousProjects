#pragma once
/*
- Descriptor: Ini������ Read�ϸ鼭 Section�� KeyName, Key ������ ���� ������ Ŭ����
- Key������̿� ������ ������ �ȵ�
- �ٸ� Ŭ������ �ش� ���� ������ �ϴµ�...  ���߿��� ���ν������� �����ִ� ���¿����� ������ �����ϵ��� �� ����
- 1. �� Ŭ���� �ȿ� ������ �ִ´�(�޸𸮰� �� Ŀ����)
- 2. ����Ŭ�����ȿ� �ִ´�.(�����ۿ��� ���� Ŭ������ ������.)
- 3. �������� ������ �� �ְ� �Ѵ�(�̱ۿ����� �۵��ؾ��Ѵ�.)
--> 3������ ����
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
	std::string   keyName;  //Ű �̸�
	std::string   key;      //Ű�� ����ִ� ��
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
				//�� �̻� Section�� ���ٸ� �׸�~
				if (in.eof() == true) break;

				getline(in, s);

				size_t ignoreOffset = s.find("=");
				size_t endOffset = s.find(";");

				//�ּ�ó���� ���ڿ�
				if (endOffset < ignoreOffset) continue;

				// = Ȥ�� ; �� ã�� ���ߴٸ� �߸��� ���� Ȥ�� ���� Section�̴� �ѱ���
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
		//���ø� ������ ����� �ʱ�ȭ
		_Ty convertType{};
		std::istringstream ss(value);
		ss >> convertType;
		return convertType;
	}

	template<class _Ty>
	_Ty StringConvert(std::string& value)
	{
		//���ø� ������ ����� �ʱ�ȭ
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
	//�ݵ�� �̱۽����� Ȥ�� ����ȭ�� ����� ���¿��� ����!
	bool ReadIniFile(const char* pFileName)
	{
		if (pFileName == nullptr) return false;

		// ���� �б� �غ�
		std::ifstream in(pFileName);

		if (in.is_open() == false)
		{
			PRINT_ERROR_LOG("������ ã�� �� �����ϴ�!\n");
			return false;
		}

		std::string s;
		while (in) 
		{
			getline(in, s);

			//�̰Ŵ� Section�̴�.
			if (s[0] == '[')
			{
				// ], \n ����
				s.erase(s.end() - 1, s.end());
				//[ ����
				s.erase(s.begin());
				//Keyó��
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

	//�����̸�, Ű�̸�
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