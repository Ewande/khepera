#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>
#include <vector>
#include <stdint.h>

// class used for binary data packing and unpacking for network transmission

class Buffer
{
	public:
		Buffer(int length = 0);
		~Buffer();

		template <typename T>
		void pack(const T& data);

		uint8_t* getBuffer() { return &_buffer[0]; }
		int getLength() const { return _buffer.size(); }

	private:
		// no clonning
		Buffer(const Buffer& other) {}

		std::vector<uint8_t>   _buffer;
};

template <typename T>
void Buffer::pack(const T& data)
{
	const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&data);

	for (int i = 0; i < sizeof(data); i++)
		_buffer.push_back(ptr[i]);
}

#endif