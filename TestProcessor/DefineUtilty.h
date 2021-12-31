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

