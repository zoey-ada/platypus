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
	bool operator<(AlphaSceneNode const& right) const { return this->screen_z < right.screen_z; }

	bool operator>(AlphaSceneNode const& right) const { return !(*this < right); }

	bool operator==(AlphaSceneNode const& right) const { return this->screen_z == right.screen_z; }

	bool operator!=(AlphaSceneNode const& right) const { return !(*this == right); }
};

inline bool operator<(const std::shared_ptr<AlphaSceneNode>& a,
	const std::shared_ptr<AlphaSceneNode>& b)
{
	return (*a) < (*b);
}
