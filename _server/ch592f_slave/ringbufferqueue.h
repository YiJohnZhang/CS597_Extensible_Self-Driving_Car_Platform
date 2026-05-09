#ifndef RINGBUFFERQUEUE_H
#define RINGBUFFERQUEUE_H

#pragma once
#include <cstddef>
#include <stdexcept>

template <typename T>
class RingBufferQueue {
public:
	RingBufferQueue();
	RingBufferQueue(size_t capacity);
	~RingBufferQueue();
	void enqueue(const T& input_value);
	const T& dequeue();
	const T& peek();
	bool is_empty();
	size_t size();
	size_t capacity();

private:
	T* _data{NULL};
	size_t _capacity{2};
	size_t _element_count{0};
	size_t _current_write_head{1};
	size_t _current_read_head{0};
		// note: technically read_head == write_head - 1
};

#endif