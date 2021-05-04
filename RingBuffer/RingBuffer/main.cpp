#include"RingBuffer.h"


constexpr int BUFFER_SIZE = 15;

/*
- 정해진 사이즈를 주고 받기에는 문제 없을듯
- 네트워크에 사용할 예정이기 때문에 뒤에서 짤린걸 앞에서 붙이고 하는건 안할예정
- 차라리 남은 바이트를 주던지 그냥 버리던지 하기
*/

int main() {
    CRingBuffer ringbuffer(BUFFER_SIZE);

    try {
        const char* temp = "HELLO WORLD";
        {
            auto ptr = ringbuffer.GetWriteBuffer(strlen(temp));
            memcpy(ptr, temp, strlen(temp));
        }

        temp = "Ring Buffer";
        {
            auto ptr = ringbuffer.GetWriteBuffer(strlen(temp));
            memcpy(ptr, temp, strlen(temp));
        }


        temp = "C++";
        {
            auto ptr = ringbuffer.GetWriteBuffer(strlen(temp));
            memcpy(ptr, temp, strlen(temp));
        }

        temp = "Remain";
        {
            auto ptr = ringbuffer.GetWriteBuffer(strlen(temp));
            memcpy(ptr, temp, strlen(temp));
        }

        /*   auto ptr = ringbuffer.GetReadBuffer(strlen(temp));
           std::cout << ptr << "\n";*/

        ringbuffer.PrintAllBuffer();
    }
    catch (const std::exception& e) {
        std::cout << __func__ << ": " << e.what() << "\n";
    }

}