import os

from pyringbuffer import RingBuffer

size = 4096


def test_init():
    ring = RingBuffer(size)
    assert ring.get_size() == size


def test_put():
    N = 1024
    ring = RingBuffer(size)
    res = ring.put(os.urandom(N), N)

    assert res == N
