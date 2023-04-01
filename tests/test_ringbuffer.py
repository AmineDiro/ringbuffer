import os

import pytest

from pyringbuffer import RingBuffer

size = 4096


def test_init():
    ring = RingBuffer(size)
    assert ring.size == size
    assert ring.read_idx == 0
    assert ring.write_idx == 0


def test_put():
    ring = RingBuffer(size)
    ring.put(os.urandom(size), size)
    assert ring.write_idx == 0
    assert ring.read_idx == 0

    with pytest.raises(IndexError):
        ring.put(os.urandom(size), size)


def test_get_buffer():
    ring = RingBuffer(size)
    n = 1024
    src = os.urandom(n)

    ring.put(src, n)
    result: memoryview = ring.get(n)

    assert src == result.tobytes()
    assert len(result) == n

    with pytest.raises(ValueError):
        result = ring.get(n)


def test_multiple_write():
    ring = RingBuffer(size)
    src = os.urandom(size)

    ring.put(src, size)
    _ = ring.get(size)
    assert ring.write_idx == 0
    assert ring.read_idx == 0

    n = 1024
    ring.put(src[:n], n)
    assert ring.write_idx == n % size
    _ = ring.get(n)
    assert ring.read_idx == n % size
