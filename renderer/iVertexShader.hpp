#pragma once

#include <memory>

class Scene;
class SceneNode;

class IVertexShader
{
public:
	virtual ~IVertexShader() = default;

	virtual bool initialize(const std::shared_ptr<Scene>& scene) = 0;
	virtual bool setupRender(const std::shared_ptr<Scene>& scene, const std::shared_ptr<SceneNode>& node) = 0;
};
