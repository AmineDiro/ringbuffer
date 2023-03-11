package main

import (
	"crypto/rand"
	"log"
	"syscall"
)

const N = 1

func main() {
	// r, _ := NewRingBuffer()
	// defer r.Close()
	b, err := syscall.Mmap(-1, 0, syscall.Getpagesize(), syscall.PROT_NONE, syscall.MAP_ANON|syscall.MAP_PRIVATE)

	if err != nil {
		log.Fatalf("Mmap: %v", err)
	}

	defer func() {
		if err := syscall.Munmap(b); err != nil {
			log.Fatalf("Munmap: %v", err)
		}
	}()

	data := make([]byte, syscall.Getpagesize())
	if _, err := rand.Read(data); err != nil {
		log.Fatalf("%s", err)

	}
	copy(b, data)
	// for i := 0; i < N; i++ {
	// 	data := make([]byte, 2)
	// 	if _, err := rand.Read(data); err != nil {
	// 		log.Fatalf("%s", err)

	// 	}
	// 	// Put data
	// 	if err := r.Put(data); err != nil {
	// 		log.Fatalf("%s", err)
	// 	}
	// 	fmt.Printf("Data : %v\n", r.buffer)

	// 	// Read data
	// 	dataNew, _ := r.Get(2)
	// 	fmt.Printf("ReadData : %v , readPointer :%d, written:%d\n", dataNew, r.readPointer, r.writtenSize)

	// }

}
