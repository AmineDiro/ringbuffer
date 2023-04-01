PyRingBuffer
==============

Python wrapper of an mmap circular buffer.

Here is a schema of the how mmap is used to automatically wraps around using CPU TLB black magic:

<p align = "center">
<img src = "https://miro.medium.com/v2/resize%3Afit%3A1400/0%2AxnvL-UaU90H9I8YW.png">
</p>
<p align = "center">
Top array shows vritual memory, while bottom one shows physical memory - (<a href="https://abhinavag.medium.com/a-fast-circular-ring-buffer-4d102ef4d4a3">link to post</a>)


</p>


## Example usage 

Create a ring buffer of size multiple of the page size of your system (default to 4KB). 
You can  put and get data from the buffer.
```ipython
In [1]: import os
   ...: from pyringbuffer import RingBuffer
   ...:
   ...: BUFFER_SIZE=4096
   ...: N = 1024
   ...:
   ...: ring = RingBuffer(BUFFER_SIZE)
   ...: ring.put(os.urandom(N))
   ...: data = ring.get(N)
   ...:
   ...:
   ...: print(f"Got memory view of {len(data)} bytes.")
   ...: print(ring)
Got memory view of 1024 bytes.
RingBuffer(4096, read idx :1024, write idx :1024)
```