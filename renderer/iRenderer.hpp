#pragma once

#include <memory>

#include <platypus_proto/settings.hpp>

#include "color.hpp"
#include "commonMesh.hpp"
#include "graphics.hpp"
#include "iShaderManager.hpp"
#include "ptAddressOverscanMode.hpp"

class IPixelShader;
class IRendererState;
class ITexture;
class IVertexShader;
class Mat4x4;
class ResourceCache;
class MeshResource;
class TextureResource;

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual bool initialize(const platypus::RendererSettings& settings,
		const std::weak_ptr<ResourceCache>& cache) = 0;
	virtual void deinitialize() = 0;

	virtual bool preRender() = 0;
	virtual bool postRender() = 0;

	virtual void enableDebugMode() = 0;

	virtual std::shared_ptr<IShaderManager> shaderManager() = 0;

	virtual void drawMesh(const std::shared_ptr<MeshResource>& mesh) = 0;
	virtual std::shared_ptr<MeshResource> createCommonMesh(const CommonMesh mesh_type) = 0;

	virtual std::shared_ptr<IRendererState> prepareAlphaPass() = 0;

	virtual void setBackgroundColor(const Color& backgroundColor) = 0;

	virtual void setWorldTransform(const Mat4x4& world) = 0;

	virtual PtInputLayout createInputLayout(std::byte* shader_data, const uint64_t data_size,
		PtInputLayoutDesc* layout_elements, const uint64_t element_count) = 0;
	virtual void destroyInputLayout(PtInputLayout layout) = 0;

	virtual PtSamplerState createSamplerState(const PtAddressOverscanMode overscan_mode) = 0;
	virtual void destroySamplerState(PtSamplerState sampler_state) = 0;

	virtual PtTexture createTexture(std::byte* shader_data, const uint64_t data_size) = 0;
	virtual void destroyTexture(PtTexture texture) = 0;

	virtual PtVertexBuffer createVertexBuffer(const graphics::Vertex* vertices,
		const uint64_t vertex_count) = 0;
	virtual void destroyVertexBuffer(PtVertexBuffer buffer) = 0;

	virtual PtIndexBuffer createIndexBuffer(const uint32_t* indices,
		const uint64_t index_count) = 0;
	virtual void destroyIndexBuffer(PtIndexBuffer buffer) = 0;

	virtual std::shared_ptr<TextureResource> rasterizeText(const char* message,
		const char* font_family, const uint16_t point_size) = 0;

	virtual std::shared_ptr<IVertexShader> loadVertexShader(std::string path) = 0;
	virtual std::shared_ptr<IPixelShader> loadPixelShader(std::string path,
		std::string texture = std::string()) = 0;
	// virtual std::shared_ptr<ITexture> loadTexture(std::string path) = 0;
};
