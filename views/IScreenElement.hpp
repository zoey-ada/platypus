#pragma once

#include "../utilities/time.hpp"

class IScreenElement
{
public:
	virtual ~IScreenElement() = default;

	virtual bool onRestore() = 0;
	virtual void onDeviceLost() = 0;
	virtual void onUpdate(Milliseconds delta) = 0;
	virtual bool onRender(Milliseconds now, Milliseconds delta) = 0;

	[[nodiscard]] virtual int zOrder() const = 0;
	virtual void setZOrder(int zOrder) = 0;
	[[nodiscard]] virtual bool isVisible() const = 0;
	virtual void setVisibility(bool isVisible) = 0;
};

inline bool operator<(const IScreenElement& lhs, const IScreenElement& rhs)
{
	return lhs.zOrder() < rhs.zOrder();
}
