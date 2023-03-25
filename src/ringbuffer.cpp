#include <iostream>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>

#define BUFFER_SIZE 4096
#define RAND_MAX 255 

class RingBuffer
{
    size_t readPointer = 0;
    size_t writePointer = 0;
    size_t bufferSize;
    u_int8_t *buffer;

public:
    RingBuffer(size_t size)
    {
        if (size % getpagesize() != 0)
        {
            fprintf(stderr, "Can't create the queue ");
            fprintf(stderr, "\n");
            abort();
        }
        bufferSize = size;
        // allocate the buffer
        int fileno = memfd_create("ringbuffer", 0);
        ftruncate(fileno, size);
        // Ask mmap for an address at a location where we can put both virtual copies of the buffer
        buffer = (u_int8_t *)mmap(NULL, 2 * size, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

        // Map the buffer at that address
        mmap(buffer, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fileno, 0);

        // Now map it again, in the next virtual page
        mmap(buffer + size, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fileno, 0);
    }
    int Put(u_int8_t *data, size_t dataSize)
    {

        if (bufferSize - (writePointer - readPointer) < dataSize)
        {
            printf("ERROR\n");
            return -1;
        }

        memcpy(buffer + writePointer, data, dataSize);
        writePointer += dataSize;

        return dataSize;
    };
    u_int8_t *Get(size_t length)
    {

        if (readPointer - writePointer < length)
        {
            return nullptr;
        }

        readPointer += length;
        return NULL;
    };
    void PrintBuffer()
    {

        // Checks only the beginning to see if we wrap around
        std::cout << "readPointer " << readPointer << " writePointer " << writePointer << std::endl;
        for (size_t i = 0; i < 10; i++)
        {
            printf("%d", buffer[i]);
        }
        printf("\n");
    };
};

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
    u_int8_t data_[2] = {0, 2};
    ring.Put(data_, 2);

    std::cout << "After PUT" << std::endl;
    ring.PrintBuffer();
}