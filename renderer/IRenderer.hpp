#pragma once

#include <memory>

#include <platypus_proto/settings.hpp>

#include "color.hpp"

class IPixelShader;
class IRendererState;
class ITexture;
class IVertexShader;
class Mat4x4;
class ResourceCache;
class MeshResource;

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual bool initialize(const platypus::RendererSettings& settings,
		const std::weak_ptr<ResourceCache>& cache) = 0;
	virtual void deinitialize() = 0;

	virtual bool preRender() = 0;
	virtual bool postRender() = 0;

	virtual void drawMesh(const std::shared_ptr<MeshResource>& mesh) = 0;
	virtual std::shared_ptr<MeshResource> createRectangle() = 0;

	virtual std::shared_ptr<IRendererState> prepareAlphaPass() = 0;

	virtual void setBackgroundColor(const Color& backgroundColor) = 0;

	virtual void setWorldTransform(const Mat4x4& world) = 0;

	virtual std::shared_ptr<IVertexShader> loadVertexShader(std::string path) = 0;
	virtual std::shared_ptr<IPixelShader> loadPixelShader(std::string path,
		std::string texture = std::string()) = 0;
	// virtual std::shared_ptr<ITexture> loadTexture(std::string path) = 0;
};
