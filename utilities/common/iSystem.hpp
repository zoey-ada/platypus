#pragma once

#include "../time/utils.hpp"

class ISystem
{
public:
	virtual ~ISystem() = default;

	virtual void deinitialize() = 0;

	virtual void update(Milliseconds delta) = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;
};

// template<class... _Types>
// class ISystem
// {
// public:
// 	virtual ~ISystem() = default;

// 	virtual bool initialize(_Types&&... types) = 0;
// 	virtual void deinitialize() = 0;

// 	virtual void update(Milliseconds delta) = 0;

// 	virtual void pause() = 0;
// 	virtual void resume() = 0;
// };
