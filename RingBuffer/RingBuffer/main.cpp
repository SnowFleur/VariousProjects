#include"RingBuffer.h"


constexpr int BUFFER_SIZE = 100;

/*
- 경계 처리(짤리는 데이터)
- 예외 처리
*/

int main() {
    CRingBuffer ringbuffer(BUFFER_SIZE);


    const char* temp = "HELLO WORLD";
    {
        auto ptr = ringbuffer.GetWrtieBuffer(strlen(temp));
        memcpy(ptr, temp, strlen(temp));
    }


    temp = "My Name Is Park";
    {
        auto ptr = ringbuffer.GetWrtieBuffer(strlen(temp));
        memcpy(ptr, temp, strlen(temp));
    }

    temp = "HELLO WORLD";

    auto ptr = ringbuffer.GetReadBuffer(strlen(temp));
    std::cout << ptr << "\n";


    ringbuffer.PrintAllBuffer();


}