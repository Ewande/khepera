#include "Buffer.h"

Buffer::Buffer(int length) : _length(length), _offset(0)
{
	_buffer = new uint8_t[length];
}

Buffer::~Buffer()
{
	delete[] _buffer;
}