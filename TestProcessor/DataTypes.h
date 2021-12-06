#pragma once
/*
- Developer: PDH
- Descriptor: 여러가지 타입 재정의
-
*/
#include<memory>

#include"DefineUtilty.h"
#include"WindowsHeader.h"

//Forward Declaration
class CSnowSession;
class CSnowSocket;
class CSnowServer;
class CNetAddress;
class CIocpObject;
class CIocpHandle;
class CSnowAcceptor;
class CSnowThread;
class CSnowConnector;

//Using Data type
#ifdef _WIN64
using SessionID = uint64_t;
#else
using SessionID = int32_t;
#endif

using ERROR_CODE  = uint32_t;
using PacketType  = int16_t;
using PacketSize  = int16_t;

//Server Infor
#define SERVER_ADDR "127.0.0.1"
constexpr int32_t   SERVER_PORT         = 9000;
constexpr int32_t   BUFFER_SIZE         = 1024;
constexpr int32_t   MAX_WORKER_THREAD   = 16;
constexpr uint32_t  GQCS_WAIT_TIME      = INFINITE;

//*************************
//enum class
//*************************
enum class SOCKET_TYPE  : uint8_t{ TCP_TYPE = 1, UDP_TYPE };
enum class LOGIC_EVENT  : uint8_t { RECV = 1, ACCEPT, DISCONNECT };


//*************************
//Smart Pointer
//*************************

#define SHARED_PTR(x)   using x##Sptr   = std::shared_ptr<x>
#define UNIQUE_PTR(x)   using x##Uptr   = std::unique_ptr<x>

SHARED_PTR(CIocpObject);
SHARED_PTR(CIocpHandle);
SHARED_PTR(CSnowSession);
SHARED_PTR(CSnowAcceptor);
SHARED_PTR(CSnowConnector);

UNIQUE_PTR(CSnowThread);



//ERROR_CODE LIST 혹시 네임이 겹칠지 모르니 namespace
namespace SNOW_ERROR
{
    constexpr ERROR_CODE NOT_ERROR = 0;     //에러가 아니다.
};

