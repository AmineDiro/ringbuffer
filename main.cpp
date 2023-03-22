#include <iostream>
#include "ringbuffer.h"

#define BUFFER_SIZE 4096
#define RAND_MAX 255 

int main()
{
    auto ring = RingBuffer(BUFFER_SIZE);
    std::cout << "Created Ring" << std::endl;

    u_int8_t data[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        data[i] = rand() % 100;
    }
    ring.Put(data, BUFFER_SIZE);

    // First 3ammer lbuffer
    std::cout << "After PUT" << std::endl;
    ring.PrintBuffer();

    // 9rah
    ring.Get(BUFFER_SIZE);
    std::cout << "After Read" << std::endl;

    ring.PrintBuffer();

    //  Put marra khra
    u_int8_t data_[2] = {1, 1};
    ring.Put(data_, 2);

    std::cout << "After PUT" << std::endl;
    ring.PrintBuffer();
}