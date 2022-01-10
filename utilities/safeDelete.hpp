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
		delete[] *ptr_array;
	}

	*ptr_array = nullptr;
}

#ifdef _WIN32

#include <Unknwnbase.h>

template<class T>
void safeRelease(T** object)
{
	if (*object != nullptr)
	{
		(*object)->Release();
	}

	*object = nullptr;
}

template<class T>
std::shared_ptr<T> toSharedPtr(T** object)
{
	auto shared_object = std::shared_ptr<T>(*object, [](T* raw_object) { raw_object->Release(); });
	*object = nullptr;
	return shared_object;
}

#endif
