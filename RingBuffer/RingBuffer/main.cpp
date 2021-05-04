#include"RingBuffer.h"


constexpr int BUFFER_SIZE = 15;

/*
- ������ ����� �ְ� �ޱ⿡�� ���� ������
- ��Ʈ��ũ�� ����� �����̱� ������ �ڿ��� ©���� �տ��� ���̰� �ϴ°� ���ҿ���
- ���� ���� ����Ʈ�� �ִ��� �׳� �������� �ϱ�
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