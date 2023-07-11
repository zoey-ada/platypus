#pragma once

#include <memory>
#include <optional>

#include <platypus_proto/settings.hpp>

#include <utilities/common/data.hpp>

#include "color.hpp"
#include "commonMesh.hpp"
#include "graphics.hpp"
#include "texelOverscanMode.hpp"

class IMesh;
class IPixelShader;
class IRendererState;
class IShaderManager;
class ITexture;
class IVertexShader;

class Mat4x4;

namespace platypus
{
class IResourceCache;
class MeshResource;
class TextureResource;
};

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual bool initialize(const platypus::RendererSettings& settings,
		const std::weak_ptr<platypus::IResourceCache>& cache) = 0;
	virtual void deinitialize() = 0;

	virtual bool preRender() = 0;
	virtual bool postRender() = 0;

	virtual void enableDebugMode() = 0;

	virtual std::shared_ptr<IShaderManager> shaderManager() = 0;

	virtual void drawMesh(const std::shared_ptr<platypus::MeshResource>& mesh) = 0;
	virtual std::shared_ptr<platypus::MeshResource> createCommonMesh(
		const CommonMesh mesh_type) const = 0;
	virtual std::shared_ptr<platypus::MeshResource> createCommonMesh(const CommonMesh mesh_type,
		const std::string& resource_id) const = 0;

	virtual std::shared_ptr<IRendererState> prepareAlphaPass() = 0;

	virtual void setBackgroundColor(const Color& backgroundColor) = 0;

	virtual void setWorldTransform(const Mat4x4& world) = 0;

	virtual PtInputLayout createInputLayout(std::byte* shader_data, const uint64_t data_size,
		PtInputLayoutDesc* layout_elements, const uint64_t element_count) const = 0;
	virtual void destroyInputLayout(PtInputLayout layout) const = 0;

	virtual platypus::graphics::SamplerState createSamplerState(
		const platypus::TexelOverscanMode overscan_mode) const = 0;
	virtual void destroySamplerState(platypus::graphics::SamplerState sampler_state) const = 0;

	virtual std::optional<platypus::graphics::Texture> createTexture(
		const platypus::Data& texture_data) const = 0;
	virtual void destroyTexture(platypus::graphics::TextureResource texture) const = 0;

	virtual platypus::graphics::ConstantBuffer createConstantBuffer(uint32_t buffer_size) const = 0;
	virtual void destroyConstantBuffer(platypus::graphics::ConstantBuffer buffer) const = 0;

	virtual platypus::graphics::VertexBuffer createVertexBuffer(
		const platypus::graphics::Vertex* vertices, const uint64_t vertex_count) const = 0;
	virtual void destroyVertexBuffer(platypus::graphics::VertexBuffer buffer) const = 0;

	virtual platypus::graphics::IndexBuffer createIndexBuffer(const uint32_t* indices,
		const uint64_t index_count) const = 0;
	virtual void destroyIndexBuffer(platypus::graphics::IndexBuffer buffer) const = 0;

	virtual platypus::TextMetrics measureText(const char* message, const char* font_family,
		const uint16_t point_size) = 0;
	virtual std::shared_ptr<platypus::TextureResource> rasterizeText(const char* message,
		const char* font_family, const uint16_t point_size) = 0;

	virtual std::unique_ptr<IVertexShader> loadVertexShader(std::string path) const = 0;
	virtual std::unique_ptr<IPixelShader> loadPixelShader(std::string path,
		std::string texture = std::string()) const = 0;

	virtual std::unique_ptr<IMesh> loadMesh(const std::string& resource_id) const = 0;
	virtual std::unique_ptr<IMesh> loadCommonMesh(const CommonMesh mesh_type,
		const std::string& resource_id) const = 0;
	// virtual std::shared_ptr<ITexture> loadTexture(std::string path) = 0;
};
