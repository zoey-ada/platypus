#pragma once

#include <memory>
#include <string>

class Scene;
class SceneNode;

class IPixelShader
{
public:
	virtual ~IPixelShader() = default;

	virtual bool initialize(const std::shared_ptr<Scene>& scene) = 0;
	virtual bool setupRender(const std::shared_ptr<Scene>& scene, const std::shared_ptr<SceneNode>& node) = 0;

	virtual bool setTexture(const std::string& texture_name) = 0;
};