

  //C++20 ��� --> ������ �ʹ� ���� ���ͼ� ��� 
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
//// #define GLOBAL_FUNCTION static   // ����� static �� ����� �� ����
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
