#pragma once

class IRendererState
{
public:
	virtual ~IRendererState() = default;
	[[nodiscard]] virtual const char* description() const = 0;
};
