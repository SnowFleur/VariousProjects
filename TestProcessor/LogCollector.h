#pragma once
/*
- Developer: PDH
- Descriptor:  Log 출력 및 로그 저장용? --> Log Server를 만드는 것도 방법 (UDP)
- 컴파일 버전에 따른 버전처리
*/
#include<iostream>
#include"DefineUtilty.h"
#include"Singleton.h"

class CLogCollector :public CSingleton<CLogCollector> {
private:

public:
#if VER_CPP_17 // c++17이상
    template<class... _Args>
    void PrintLog(_Args... args)const {
        //((std::cout << args << " "), ...) << "\n";
        ((std::cout << args), ...);
    }

#elif VER_CPP_14 || VER_CPP_11  //C++ 14 || C++11
    template<class _Ty, class... _Args>
    void PrintLog(_Ty message, _Args... args)const {
        std::cout << message;
        PrintLog(args...);
    }

    template<class _Ty>
    void PrintLog(_Ty message)const {
        //std::cout << message << "\n";
        std::cout << message;
    }
#else
    //ERROR
#endif //  _MSC_VER 

};


/*
- Meesage: 출력 내용
*/
#define PRINT_LOG(message,...)          CLogCollector::GetInstance()->PrintLog(message,__VA_ARGS__)
#define PRINT_INFO_LOG(message,...)     CLogCollector::GetInstance()->PrintLog("[INFOR] ",message,__VA_ARGS__)
#define PRINT_ERROR_LOG(message,...)    CLogCollector::GetInstance()->PrintLog("[ERROR] ",message,__VA_ARGS__)


