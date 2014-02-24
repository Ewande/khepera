#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>
#include <stdint.h>

// class used for binary data packing and unpacking for network transmission

class Buffer
{
	public:
		Buffer(int length);
		~Buffer();

		template <typename T>
		void Pack(const T& data);

		uint8_t* GetBuffer() { return _buffer; }
		int GetLength() const { return _length; }
		int GetOffset() const { return _offset; }

	private:
		// no clonning
		Buffer(const Buffer& other) {}

		uint8_t*  _buffer;
		int       _offset;
		int       _length;
};

template <typename T>
void Buffer::Pack(const T& data)
{
	memcpy((void*) (_buffer + _offset), &data, sizeof(data));
	_offset += sizeof(data);
}

#endif