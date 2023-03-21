#include <iostream>
#include "ringbuffer.h"

#define BUFFER_SIZE 16

int main()
{
    auto ring = RingBuffer(BUFFER_SIZE);
    std::cout << "Created Ring" << std::endl;

    u_int8_t data[2] = {1, 3};
    for (size_t i = 0; i < BUFFER_SIZE/2; i++)
    {
        auto res = ring.Put(data, 2);
    }
    ring.PrintBuffer();

    auto _ = ring.Get(BUFFER_SIZE);

    u_int8_t data_[2] = {0, 0};
    for (size_t i = 0; i < BUFFER_SIZE/2-1; i++)
    {
        auto res = ring.Put(data_, 2);
    }

    ring.PrintBuffer();
}