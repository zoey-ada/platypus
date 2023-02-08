#pragma once

enum class RenderPass
{
	First,
	Static = First,
	Entity,
	Sky,
	UserInterface,
	NotRendered,
	Last
};
