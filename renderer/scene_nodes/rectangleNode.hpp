#pragma once

#include <memory>

#include "./SceneNode.h"

class IEvent;
class IPixelShader;
class IVertexShader;
class Scene;

class RectangleNode: public SceneNode, public std::enable_shared_from_this<RectangleNode>
{
public:
	RectangleNode(const ActorId actorId, const std::string name, const Color color,
		RenderPass renderPass, const uint32_t height, const uint32_t width, const float x = 0,
		const float y = 0, const std::string pixelShaderFilename = std::string(),
		const std::string vertexShaderFilename = std::string());

	virtual ~RectangleNode() {}

	bool onRestore(std::shared_ptr<Scene> scene) override;
	bool onLostDevice(std::shared_ptr<Scene> /*scene*/) override { return true; }
	bool render(std::shared_ptr<Scene> scene) override;

	uint32_t Height() const { return this->_height; }
	uint32_t Width() const { return this->_width; }
	uint32_t X() const { return this->_x; }
	uint32_t Y() const { return this->_y; }

	void ActorMovedDelegate(std::shared_ptr<IEvent> data);

private:
	uint32_t _height;
	uint32_t _width;
	float _x;
	float _y;
	Color _color;
	std::string _pixelShaderFilename;
	std::string _vertexShaderFilename;
	std::shared_ptr<IPixelShader> _pixelShader;
	std::shared_ptr<IVertexShader> _vertexShader;

	std::string GenerateMeshName();
};
