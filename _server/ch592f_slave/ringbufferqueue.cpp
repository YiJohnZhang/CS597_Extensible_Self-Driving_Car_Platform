/**	Yi Zhang
 *	RingBufferQueue.cpp
 *	
 *	A ring buffer queue intded to store sensor data polled
 *	as a background process. This is to allow quicker I2C
 *	fetches (in default high performance mode).
 *	
 *	NOTE: To avoid repeating `template <typename T>` before every
 *	method, merge `.h` and `.cpp` into one `.cpp` file.
 */

#include "ringbufferqueue.h"

/**
 *	@brief implicit constructor, `capacity` set to 2.
 */
template <typename T>
RingBufferQueue<T>::RingBufferQueue()
{
	// _capacity = 2;
		// header already implicitly sets this
	_data = new T[_capacity];
}

/**
 *	@brief explicit constructor variant. capacity must be >= 2
 */
template <typename T>
RingBufferQueue<T>::RingBufferQueue(size_t capacity)
{
	if (capacity < 2)
	{
		throw std::invalid_argument("RingBufferQueue<T>::RingBufferQueue(): expect `_capacity` > 0");
	}

	_capacity = capacity;
	_data = new T[_capacity];
}

/**
 *	@brief destructor frees
 *	consider using `std::unique_pointer` for automatic memory management.
 */
template <typename T>
RingBufferQueue<T>::~RingBufferQueue()
{
	delete[] _data;
}

/**
 *	@brief enqueue (input data, overwrite as necessary)
 *	1. Save the data at `write_head` (overwrite o.k.)
 *	2. Increment `read_head` and `write_head` (note: technically read_head == write_head - 1)
 *	3. Update `element_count` as appropriate
 *	
 *	@param value - (const T&) the value to input into the RBQ 
 */
template <typename T>
void RingBufferQueue<T>::enqueue(const T& value)
{
	_data[_current_write_head] = value;
	_current_read_head = (_current_read_head + 1) % _capacity;
	_current_write_head = (_current_write_head + 1) % _capacity;
	_element_count = (_element_count == _capacity) ? _capacity : _element_count + 1;
}

/**
 *	@brief dequeue (get data, shift internal state as needed)
 *	1. `peek()` data.
 *	2. update internal state appropriately: shift read and write heads, element_count
 *	3. return `peek()`ed data.
 *	
 *	@throw std::underflow_error if the RBQ is empty
 *	@return (T) the data at previous `read_head`
 */
template <typename T>
const T& RingBufferQueue<T>::dequeue()
{
	T return_value = peek();
	_current_read_head = (_current_read_head - 1) % _capacity;
	_current_write_head = (_current_write_head - 1) % _capacity;
	_element_count -= 1;

	return return_value;
}

/**
 *	@brief peek (get data, no internal state changes)
 *	
 *	@throw std::underflow_error if the RBQ is empty
 *	@return (T) the current data at `read_head`
 */
template <typename T>
const T& RingBufferQueue<T>::peek()
{
	if(is_empty()) {
		throw std::underflow_error("queue is empty");
	}

	return _data[_current_read_head];
}

/**
 *	@brief get RBQ size (element count)
 *	
 *	@return (size_t) number of elements
 */
template <typename T>
size_t RingBufferQueue<T>::size()
{
	size_t _element_count = (_element_count == _capacity) ? _capacity : _element_count;
	return _element_count;
}

/**
 *	@brief is RBQ empty? (element_count == 0)
 *	
 *	@return (bool) is rbq empty?
 */
template <typename T>
bool RingBufferQueue<T>::is_empty()
{
	return _element_count == 0;
}