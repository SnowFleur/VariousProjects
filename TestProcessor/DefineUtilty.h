#pragma once
/*
- Developer: PDH
- Descriptor: 여러가지 매크로 기능들
-
*/

//Link: https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
//https://docs.microsoft.com/ko-kr/cpp/preprocessor/predefined-macros?view=msvc-160

#define VER_CPP_20      _MSVC_LANG==202002L
#define VER_CPP_17      _MSVC_LANG==201703L
#define VER_CPP_14      _MSVC_LANG==201402L
#define VER_CPP_11      _MSVC_LANG==201103L
#define VER_CPP_LEGACY  _MSVC_LANG<201103L

#define SAFE_DELETE(x)          if(x!=nullptr)delete x;
#define SAFE_DELETE_MULTI(x)    if(x!=nullptr)delete[] x;

#if !VER_CPP_LEGACY  //C++11이상 지원하는 컴파일러
#define DELETE_COPY(name)                    \
public:                                      \
name& operator=(const name&)    = delete;    \
name(const name&)               = delete;                     

#define DELETE_MOVE(name)                    \
public:                                      \
name& operator=(name&&)noexcept =delete;     \
name(name&&)noexcept            =delete;      
#elif VER_CPP_LEGACY
#define DELETE_COPY(name)                    \
private:                                     \
name& operator=(const name&);                \
name(const name&);                     
#endif


// Session 소멸자가 잘 호출되는지 확인용
#define PRINT_DEBUG_SESSION_DESTRUCTOR

// 메모리 풀이 아닌 New/Delete 함수를 사용하겠다. 
#define USED_NEW_DELETE_FUNCTION

