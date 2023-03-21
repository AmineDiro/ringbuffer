#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ringbuffer.h"

RingBuffer::RingBuffer(size_t size)
{
    bufferSize = size;
    // allocate the buffer
    int fileno = memfd_create("ringbuffer", 0);
    ftruncate(fileno, size);
    // Ask mmap for an address at a location where we can put both virtual copies of the buffer
    buffer = (u_int8_t *)mmap(NULL, 2 * size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // Map the buffer at that address
    mmap(buffer, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fileno, 0);

    // Now map it again, in the next virtual page
    mmap(buffer + size, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fileno, 0);
}

int RingBuffer::Put(u_int8_t *data, size_t dataSize)
{
    if (bufferSize - (writePointer - readPointer) < dataSize)
    {
        printf("ERROR\n");
        return -1;
    }

    memcpy(buffer + writePointer, data, dataSize);
    writePointer += dataSize;

    return dataSize;
}

void RingBuffer::PrintBuffer()
{
    std::cout << "readPointer " << readPointer << " writePointer " << writePointer << std::endl;
    for (size_t i = 0; i < bufferSize; i++)
    {
        printf("%d", buffer[i]);
    }
    printf("\n");
}

u_int8_t *RingBuffer::Get(size_t length)
{
    if (readPointer - writePointer < length)
    {
        return nullptr;
    }

    readPointer += length;
    return NULL ;
}