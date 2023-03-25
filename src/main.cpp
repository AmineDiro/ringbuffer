#include <iostream>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pybind11/pybind11.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

class RingBuffer
{
    size_t readPointer = 0;
    size_t writePointer = 0;
    size_t bufferSize;
    char *buffer;

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
        buffer = (char *)mmap(NULL, 2 * size, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

        // Map the buffer at that address
        mmap(buffer, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fileno, 0);

        // Now map it again, in the next virtual page
        mmap(buffer + size, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fileno, 0);
    }

    int GetSize()
    {
        return bufferSize;
    };

    int Put(char *data, size_t dataSize)
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
    char *Get(size_t length)
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

namespace py = pybind11;
PYBIND11_MODULE(pyringbuffer, m)
{
    py::class_<RingBuffer>(m, "RingBuffer")
        .def(py::init<size_t>())
        .def("get_size", &RingBuffer::GetSize)
        .def("put", &RingBuffer::Put)
        .def("get", &RingBuffer::Get)
        .def("print", &RingBuffer::PrintBuffer);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
