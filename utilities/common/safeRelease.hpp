#pragma once

template<class T>
void safeRelease(T** object)
{
	if (*object != nullptr)
	{
		(*object)->Release();
	}

	*object = nullptr;
}
