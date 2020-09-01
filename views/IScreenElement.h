#pragma once

#include "../utilities/time.h"

class IScreenElement
{
public:
	virtual ~IScreenElement() = default;

	virtual const bool onRestore() = 0;
	virtual void onDeviceLost() = 0;
	virtual void onUpdate(const Milliseconds delta) = 0;
	virtual const bool onRender(const Milliseconds now, const Milliseconds delta) = 0;

	virtual const int zOrder() const = 0;
	virtual void setZOrder(const int zOrder) = 0;
	virtual const bool isVisible() const = 0;
	virtual void setVisibility(const bool isVisible) = 0;
};

inline const bool operator<(const IScreenElement& lhs, const IScreenElement& rhs)
{
	return lhs.zOrder() < rhs.zOrder();
}
