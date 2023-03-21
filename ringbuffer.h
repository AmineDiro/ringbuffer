
#pragma once
#include <vector>
#include <stdlib.h>

class RingBuffer { 
    size_t readPointer=0;
    size_t writePointer=0;
    size_t bufferSize;
    u_int8_t *buffer;

    public: 
        RingBuffer(size_t size);
        int Put(u_int8_t *data, size_t length);
        u_int8_t *Get( size_t length);
        void PrintBuffer();
};