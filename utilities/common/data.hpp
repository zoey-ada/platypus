#pragma once

#include <cstddef>
#include <cstdint>

namespace platypus
{

class Data
{
public:
	Data(std::byte* data, uint64_t size): _data(data), _size(size) {}
	virtual ~Data() = default;

	template<class T>
	T* dataAs() const
	{
		return reinterpret_cast<T*>(this->_data);
	}

	inline std::byte* data() const { return this->_data; }
	inline uint64_t size() const { return this->_size; }

	bool isEmpty() const { return this->_data == nullptr || this->_size == 0; }

private:
	std::byte* _data;
	uint64_t _size;
};

};
