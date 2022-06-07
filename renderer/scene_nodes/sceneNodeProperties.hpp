#pragma once

#include <memory>
#include <string>

#include <entities/entity.hpp>
#include <renderer/color.hpp>
#include <utilities/math/mathTypes.hpp>

#include "renderPass.hpp"

class SceneNodeProperties
{
	friend class SceneNode;

public:
	[[nodiscard]] inline const EntityId& entityId() const { return _entity_id; }
	[[nodiscard]] inline const std::string& name() const { return _name; }
	[[nodiscard]] inline const RenderPass& renderPass() const { return _render_pass; }
	[[nodiscard]] inline const Color& diffuse() const { return _diffuse; }
	[[nodiscard]] inline const Color& ambient() const { return _ambient; }
	[[nodiscard]] inline const Mat4x4& toWorld() const { return _to_world; }
	[[nodiscard]] inline const Mat4x4& fromWorld() const { return _from_world; }

	inline void transform(Mat4x4& toWorld, Mat4x4& fromWorld) const
	{
		toWorld = _to_world;
		fromWorld = _from_world;
	}

	inline void entityId(const EntityId& entityId) { _entity_id = entityId; }
	inline void name(const std::string& name) { _name = name; }
	inline void renderPass(const RenderPass renderPass) { _render_pass = renderPass; }
	inline void diffuse(const Color& diffuse) { _diffuse = diffuse; }
	inline void ambient(const Color& ambient) { _ambient = ambient; }
	inline void toWorld(const Mat4x4* toWorld) { _to_world = Mat4x4(*toWorld); }
	inline void toWorld(const Mat4x4& toWorld) { _to_world = Mat4x4(toWorld); }
	inline void fromWorld(const Mat4x4* fromWorld) { _from_world = Mat4x4(*fromWorld); }
	inline void fromWorld(const Mat4x4& fromWorld) { _from_world = Mat4x4(fromWorld); }

private:
	EntityId _entity_id = InvalidEntityId;
	std::string _name;
	RenderPass _render_pass = RenderPass::NotRendered;
	Color _diffuse = Color::white;
	Color _ambient = Color::white;
	Mat4x4 _to_world;
	Mat4x4 _from_world;
};
