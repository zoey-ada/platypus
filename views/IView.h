#pragma once

#include "../entities/entity.h"
#include "../utilities/time.h"

#include <cstdint>

enum class ViewType
{
	Human
};

typedef uint16_t ViewId;

class IView
{
public:
	virtual ~IView() = default;

	virtual const bool onRestore() = 0;
	virtual void onDeviceLost() = 0;

	virtual void onUpdate(const Milliseconds delta) = 0;
	virtual void onRender(const Milliseconds now, const Milliseconds delta) = 0;

	virtual void onAttach(const ViewId viewId, const EntityId entityId) = 0;
};
