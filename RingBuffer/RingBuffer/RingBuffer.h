#pragma once

#define TEST_RING_BUFFER

#include<iostream>

using BYTE = unsigned char;
using DWORD = unsigned long;

class CRingBuffer {
private:
    const DWORD     BUFFER_SIZE;

    BYTE*   buffer_;
    BYTE*   beginPointer_;
    BYTE*   endPointer_;
    BYTE*   writePointer_;
    BYTE*   readPointer_;

public:
    CRingBuffer(const DWORD ringBufferSize) :
        BUFFER_SIZE{ ringBufferSize },
        buffer_{ nullptr },
        beginPointer_{ nullptr },
        endPointer_{ nullptr },
        writePointer_{ nullptr },
        readPointer_{ nullptr }
    {

        buffer_ = new BYTE[BUFFER_SIZE]{};
        readPointer_ = writePointer_ = beginPointer_ = buffer_;
        endPointer_ = buffer_ + BUFFER_SIZE;
    }

    ~CRingBuffer() {
        if (buffer_ != nullptr) {
            delete[] buffer_;
            endPointer_ = beginPointer_ = nullptr;
            writePointer_ = readPointer_ = nullptr;
        }

    }

    BYTE* GetReadBuffer(const DWORD requestSize) {

        BYTE* resultPointer{ nullptr };

        //버퍼 사이즈보다 큰 사이즈를 요청한다면
        if (requestSize > BUFFER_SIZE) {
            throw std::exception("requestSize > BUFFER_SIZE");
        }
        // 충분한 경우
        if (endPointer_ - readPointer_ > requestSize) {
            resultPointer = readPointer_;
        }

        // 부족한 경우
        else {
            readPointer_ = beginPointer_;
            resultPointer = readPointer_;
        }

        readPointer_ += requestSize;
        return resultPointer;
    }

    BYTE* GetWriteBuffer(const DWORD requestSize) {

        BYTE* resultPointer{ nullptr };

        //버퍼 사이즈보다 큰 사이즈를 요청한다면
        if (requestSize > BUFFER_SIZE) {
            throw std::exception("requestSize > BUFFER_SIZE");
        }

        // 충분한 경우
        if (endPointer_ - writePointer_ > requestSize) {
            resultPointer = writePointer_;
        }

        // 부족한 경우
        else {
            writePointer_ = beginPointer_;
            resultPointer = writePointer_;
        }

        writePointer_ += requestSize;
        return resultPointer;
    }

#ifdef TEST_RING_BUFFER

    void PrintAllBuffer()const noexcept {

        for (DWORD i = 0; i < BUFFER_SIZE; ++i) {

            if (buffer_[i] == 0) {
                std::cout << buffer_[i] << "-";
            }
            else {
                std::cout << buffer_[i] << " ";
            }
        }

    }
#endif

};