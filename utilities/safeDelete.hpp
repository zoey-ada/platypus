#include <concepts>
#include <memory>

template<class T>
void safeDelete(T** ptr)
{
	if (*ptr != nullptr)
	{
		delete *ptr;
	}

	*ptr = nullptr;
}

template<class T>
void safeDeleteArray(T** ptr_array)
{
	if (*ptr_array != nullptr)
	{
		delete[] * ptr_array;
	}

	*ptr_array = nullptr;
}

#ifdef _WIN32

	#include <Unknwnbase.h>

template<class T>
std::shared_ptr<T> toSharedPtr(T** object)
{
	auto shared_object = std::shared_ptr<T>(*object, [](T* raw_object) { raw_object->Release(); });
	*object = nullptr;
	return shared_object;
}

template<class T>
std::shared_ptr<T> toSharedPtr(T* object)
{
	auto shared_object = std::shared_ptr<T>(object, [](T* raw_object) { raw_object->Release(); });
	return shared_object;
}

template<class T>
class Scoped
{
public:
	explicit Scoped() = default;
	Scoped(T* ptr): _pointer(ptr) {}
	Scoped(const Scoped&) = delete;
	virtual ~Scoped()
	{
		if (_pointer)
		{
			_pointer->Release();
			_pointer = nullptr;
		}
	}

	[[nodiscard]] bool isNull() const { return (!_pointer); }

	T& operator*() { return *this->_pointer; }
	T* operator->() { return this->_pointer; }
	T** operator&() { return &this->_pointer; }

	Scoped& operator=(const Scoped&) = delete;
	Scoped& operator=(T* ptr)
	{
		this->_pointer = ptr;
		return this;
	}

	void reset(T* p = 0)
	{
		if (_pointer)
		{
			_pointer->Release();
		}
		_pointer = p;
	}

	T* get() const { return _pointer; }

	T* _pointer {nullptr};
};

#endif
