#pragma once
/*
- Developer: PDH
- Descriptor: 여러가지 타입 재정의
-
*/
#include<memory>

#include"DefineUtilty.h"
#include"WindowsHeader.h"

using ERROR_CODE  = uint32_t;
using PacketType  = int16_t;
using PacketSize  = int16_t;

//ERROR_CODE LIST 혹시 네임이 겹칠지 모르니 namespace
namespace SNOW_ERROR
{
    constexpr ERROR_CODE NOT_ERROR = 0;     //에러가 아니다.
};

