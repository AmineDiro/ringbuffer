# MMAP Ring Buffer

Go implementation of the "black magic" circular ring buffer. The wikipedia page :

> A circular-buffer implementation may be optimized by mapping the underlying buffer to two contiguous regions of virtual memory.[7][disputed – discuss] (Naturally, the underlying buffer‘s length must then equal some multiple of the system’s page size.) Reading from and writing to the circular buffer may then be carried out with greater efficiency by means of direct memory access; those accesses which fall beyond the end of the first virtual-memory region will automatically wrap around to the beginning of the underlying buffer. When the read offset is advanced into the second virtual-memory region, both offsets—read and write—are decremented by the length of the underlying buffer.

## References : 
- C implementation : https://lo.calho.st/posts/black-magic-buffer/