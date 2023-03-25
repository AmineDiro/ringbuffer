package main

import (
	"errors"
	"os"
	"syscall"
)

const BUFFER_SIZE = 10

type RingBuffer struct {
	buffer       []byte
	readPointer  int
	writePointer int
	writtenSize  int
	file         *os.File
}

func NewRingBuffer() (*RingBuffer, error) {
	// Make an anonymous file and set its size
	f, _ := os.OpenFile("./file", os.O_RDWR, 0644)

	buffer, err := syscall.Mmap(-1, 0, 2*BUFFER_SIZE, syscall.PROT_NONE, syscall.MAP_PRIVATE|syscall.MAP_ANON)
	if err != nil {
		return nil, err
	}

	// Now map it again, in the next virtual page

	// Ask mmap for an address at a location
	// where we can put both virtual copies of the buffer

	// Map the buffer at that address

	// Now map it again, in the next virtual page

	return &RingBuffer{
		buffer: buffer,
		file:   f,
	}, nil
}

func (r *RingBuffer) Close() error {
	defer r.file.Close()
	if err := syscall.Munmap(r.buffer); err != nil {
		return err
	}

	return nil
}

func (r *RingBuffer) Put(data []byte) error {
	if BUFFER_SIZE-r.writtenSize < len(data) {
		return errors.New("data bigger than buffer size")
	}

	for i, b := range data {
		r.buffer[(r.writePointer+i)%BUFFER_SIZE] = b
	}
	r.writePointer = (r.writePointer + len(data)) % BUFFER_SIZE
	r.writtenSize += len(data)

	return nil
}

func (r *RingBuffer) Get(size int) ([]byte, error) {
	if size > r.writtenSize {
		return nil, errors.New("can't read more than written")
	}
	r.writtenSize -= size

	b := make([]byte, size)
	for i := 0; i < size; i++ {
		b[i] = r.buffer[(r.readPointer+i)%BUFFER_SIZE]
	}
	r.readPointer = (r.readPointer + size) % BUFFER_SIZE
	return b, nil
}
