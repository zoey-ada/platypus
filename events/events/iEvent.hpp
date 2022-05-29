#pragma once

class IEvent
{
public:
	virtual const char* getType() = 0;
};
