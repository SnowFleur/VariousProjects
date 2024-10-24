

  //C++20 모듈 --> 에러가 너무 많이 나와서 취소 
// 
//export module GlobalUtilFunctions;
//
//
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
//
//#include<memory>
//
//// #define GLOBAL_FUNCTION static   // 모듈은 static 을 사용할 수 없다
//
//export namespace SnowUtility
//{
//    template<class _Ty>
//    inline bool SafeDeleteRawPointer(_Ty* pPointer)
//    {
//        if (pPointer != nullptr)
//        {
//            delete pPointer;
//            return true;
//        }
//        return false;
//    }
//
//    template<class _Ty>
//    inline bool SafeDeleteArrayRawPointer(_Ty* pPointer)
//    {
//        if (pPointer != nullptr)
//        {
//            delete[] pPointer;
//            return true;
//        }
//        return false;
//    }
//
//    template <class _Ty>
//    inline bool IsNullPointer(std::shared_ptr<_Ty> pPointer)
//    {
//        return pPointer == nullptr ? true : false;
//    }
//
//    template <class _Ty>
//    inline bool IsNullPointer(std::unique_ptr<_Ty> pPointer)
//    {
//        return pPointer == nullptr ? true : false;
//    }
//
//    template <class _Ty>
//    inline bool IsNullPointer(const _Ty* pPointer)
//    {
//        return pPointer == nullptr;
//    }
//};
