#include"RingBuffer.h"


constexpr int BUFFER_SIZE = 100;

/*
- ��� ó��(©���� ������)
- ���� ó��
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