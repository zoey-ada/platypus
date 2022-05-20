#pragma once

#include <memory>

#include <platypus_proto/settings.hpp>

#include "color.hpp"

class IPixelShader;
class ITexture;
class IVertexShader;
class Mat4x4;
class ResourceCache;

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual bool initialize(const platypus::RendererSettings& settings,
		const std::weak_ptr<ResourceCache>& cache) = 0;
	virtual void shutdown() = 0;

	virtual bool preRender() = 0;
	virtual bool postRender() = 0;

	virtual void setBackgroundColor(const Color& backgroundColor) = 0;

	virtual void setWorldTransform(const std::shared_ptr<Mat4x4>& world) = 0;

	virtual std::shared_ptr<IVertexShader> loadVertexShader(std::string path) = 0;
	virtual std::shared_ptr<IPixelShader> loadPixelShader(std::string path) = 0;
	// virtual std::shared_ptr<ITexture> loadTexture(std::string path) = 0;
};
