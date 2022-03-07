#pragma once
#include<map>
#include<string>
#include<functional>

/*
- String 기반의 함수매니저
*/

class CFunctionManager
{
private:
	std::map <std::string, std::function<void()>> mapFunctions_;
public:



};