#pragma once

#include <cstdint>

#include <entities/entity.hpp>
#include <utilities/time/utils.hpp>

enum class ViewType
{
	Human
};

using ViewId = uint16_t;

class IView
{
public:
	virtual ~IView() = default;

	virtual bool onRestore() = 0;
	virtual void onDeviceLost() = 0;

	virtual void onUpdate(Milliseconds delta) = 0;
	virtual void onRender(Milliseconds now, Milliseconds delta) = 0;

	virtual void onAttach(ViewId viewId, EntityId entityId) = 0;
};
