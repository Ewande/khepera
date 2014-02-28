#include "Buffer.h"

Buffer::Buffer(int length)
{
	_buffer.reserve(length);
}

Buffer::~Buffer()
{
}