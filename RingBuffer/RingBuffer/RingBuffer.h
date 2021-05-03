#pragma once

#define TEST_RING_BUFFER

#include<iostream>

using BYTE  = unsigned char;
using DWORD = unsigned long;

class CRingBuffer {
private:
    const DWORD     BUFFER_SIZE;
    DWORD           remainReadSize_;
    DWORD           remainWriteSize_;

    BYTE*           buffer_;
    BYTE*           readPointer_;
    BYTE*           writePointer_;

public:
    CRingBuffer(const DWORD ringBufferSize) :
        BUFFER_SIZE{ringBufferSize},
        remainReadSize_{ringBufferSize},
        remainWriteSize_{ ringBufferSize },
        buffer_{ nullptr },
        readPointer_{nullptr },
        writePointer_{ nullptr }
    {

        buffer_ = new BYTE[BUFFER_SIZE]{};
        writePointer_ = readPointer_ = buffer_;
    }

    ~CRingBuffer() {
        if (buffer_ != nullptr) {
            delete[] buffer_;
        }
    }


    BYTE* GetWrtieBuffer(const DWORD requestSize) {

        BYTE* getPointer{ nullptr };
        try {

            //여유가 있다면
            if (remainWriteSize_ >= requestSize) {

                getPointer = writePointer_;
                writePointer_ += requestSize;
                remainWriteSize_ -= requestSize;
            }
            
            //경계 처리
            else if (remainWriteSize_ < requestSize) {
                throw std::overflow_error("Write Remain Buffer Size is small");
            }

            return getPointer;

        }

        catch (const std::exception& e) {
            std::cout << __func__ << ": " << e.what() << "\n";
        }

    }

    BYTE* GetReadBuffer(const DWORD requestSize) {

        BYTE* getPointer{ nullptr };
        try {

            //여유가 있다면
            if (remainReadSize_ >= requestSize) {

                getPointer = readPointer_;
                readPointer_ += requestSize;
                remainReadSize_ -= requestSize;

            }
            //경계 처리
            else if (remainReadSize_ < requestSize) {
                throw std::overflow_error("Read Remain Buffer Size is small");
            }

            return getPointer;
        }

        catch (const std::exception& e) {
            std::cout << __func__ << ": " << e.what() << "\n";
        }

    }


#ifdef TEST_RING_BUFFER

    void PrintAllBuffer()const noexcept {

        for (DWORD i = 0; i < BUFFER_SIZE; ++i) {

            if (buffer_[i] == 0) {
                std::cout << buffer_[i] << " ";
            }
            else {
                std::cout << buffer_[i] << " ";
            }
        }
        
    }
#endif

};