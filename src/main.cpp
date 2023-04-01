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

namespace py = pybind11;

class RingBuffer
{
    size_t readPointer = 0;
    size_t writePointer = 0;
    size_t bufferSize;
    char *buffer;

public:
    RingBuffer(const size_t size)
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

    const int GetSize()
    {
        return bufferSize;
    };

    const int GetReadPointer()
    {
        return readPointer%bufferSize;
    };
    const int GetWritePointer()
    {
        return writePointer%bufferSize;
    };

    int Put(char *data, size_t dataSize)
    {

        if (bufferSize - (writePointer - readPointer) < dataSize)
        {
            throw std::out_of_range("RingBuffer is full, read some data if you want to write more.");
            return -1;
        }

        memcpy(buffer + writePointer, data, dataSize);
        writePointer += dataSize;

        return dataSize;
    };

    py::memoryview Get(size_t length)
    {
        if (writePointer-readPointer < length)
        {
            throw std::range_error("cant't read for now");
        }
        auto membuffer = py::memoryview::from_memory(buffer + readPointer, length);
        readPointer+=static_cast<int>(length);
        return membuffer;
    };
};

PYBIND11_MODULE(pyringbuffer, m)
{
    py::class_<RingBuffer>(m, "RingBuffer")
        .def(py::init<const size_t>())
        .def_property_readonly("size", &RingBuffer::GetSize)
        .def_property_readonly("read_idx", &RingBuffer::GetReadPointer)
        .def_property_readonly("write_idx", &RingBuffer::GetWritePointer)
        .def("put", &RingBuffer::Put)
        .def("get", &RingBuffer::Get)
        .def("__repr__",
        [](RingBuffer &ring) {
            std::string r("RingBuffer(");
            r += std::to_string(ring.GetSize());
            r += ", ";
            r += "read idx :";
            r += std::to_string(ring.GetReadPointer());
            r += ", ";
            r += "write idx :";
            r += std::to_string(ring.GetWritePointer());
            r += ")";
            return r;
        });

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
