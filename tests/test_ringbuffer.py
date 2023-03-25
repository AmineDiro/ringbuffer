import os

from pyringbuffer import RingBuffer

size = 4096


def test_init():
    ring = RingBuffer(size)
    assert ring.get_size() == size


def test_put():
    n = 1024
    ring = RingBuffer(size)
    res = ring.put(os.urandom(n), n)

    assert res == n
