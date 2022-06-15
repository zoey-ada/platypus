#pragma once

#include <memory>

class ISceneNode;
class Mat4x4;

struct AlphaSceneNode
{
	std::shared_ptr<ISceneNode> node;
	Mat4x4 to_world;
	float screen_z;

	// for the stl sort...
	bool const operator<(AlphaSceneNode const& right) { return this->screen_z < right.screen_z; }

	bool const operator>(AlphaSceneNode const& right) { return !(*this < right); }

	bool const operator==(AlphaSceneNode const& right) { return this->screen_z == right.screen_z; }

	bool const operator!=(AlphaSceneNode const& right) { return !(*this == right); }
};
