import os
from re import I

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


def test_print_buffer():
    ring = RingBuffer(size)
    n = 1024
    ring.put(os.urandom(n), n)
    ring.print(size)
    pass


def test_get_buffer():
    ring = RingBuffer(size)
    n = 1024
    src = os.urandom(n)

    ring.put(src, n)
    result = ring.get(n)

    assert src == result
    assert len(result) == n
