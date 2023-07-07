#pragma once

#include <renderer/iRenderer.hpp>

class MockRenderer: public IRenderer
{
public:
	virtual ~MockRenderer() = default;

	bool initialize(const platypus::RendererSettings& settings,
		const std::weak_ptr<IResourceCache>& cache) override
	{
		return this->_can_initialize;
	}

	void deinitialize() override {}

	bool preRender() override { return this->_can_pre_render; }

	bool postRender() override { return this->_can_post_render; }

	void enableDebugMode() override {}

	std::shared_ptr<IShaderManager> shaderManager() override { return this->_shader_manager; }

	void drawMesh(const std::shared_ptr<MeshResource>& mesh) override {}

	std::shared_ptr<MeshResource> createCommonMesh(const CommonMesh mesh_type) const override
	{
		return this->_mesh_resource;
	}

	std::shared_ptr<MeshResource> createCommonMesh(const CommonMesh mesh_type,
		const std::string& resource_id) const override
	{
		return this->_mesh_resource;
	}

	std::shared_ptr<IRendererState> prepareAlphaPass() override { return this->_render_state; }

	void setBackgroundColor(const Color& backgroundColor) override {}

	void setWorldTransform(const Mat4x4& world) override {}

	PtInputLayout createInputLayout(std::byte* shader_data, const uint64_t data_size,
		PtInputLayoutDesc* layout_elements, const uint64_t element_count) const override
	{
		return this->_input_layout;
	}

	void destroyInputLayout(PtInputLayout layout) const override {}

	PtSamplerState createSamplerState(const PtAddressOverscanMode overscan_mode) const override
	{
		return this->_sampler_state;
	}

	void destroySamplerState(PtSamplerState sampler_state) const override {}

	PtTexture createTexture(std::byte* shader_data, const uint64_t data_size,
		platypus::Extent& /*dimensions__out*/) const override
	{
		return this->_texture;
	}

	void destroyTexture(PtTexture texture) const override {}

	PtVertexBuffer createVertexBuffer(const graphics::Vertex* vertices,
		const uint64_t vertex_count) const override
	{
		return this->_vertex_buffer;
	}

	void destroyVertexBuffer(PtVertexBuffer buffer) const override {}

	PtIndexBuffer createIndexBuffer(const uint32_t* indices,
		const uint64_t index_count) const override
	{
		return this->_index_buffer;
	}

	void destroyIndexBuffer(PtIndexBuffer buffer) const override {}

	PtTextMetrics measureText(const char* message, const char* font_family,
		const uint16_t point_size) override
	{
		return this->_text_metrics;
	}

	std::shared_ptr<TextureResource> rasterizeText(const char* message, const char* font_family,
		const uint16_t point_size) override
	{
		return this->_texture_resource;
	}

	std::unique_ptr<IVertexShader> loadVertexShader(std::string path) const override
	{
		return nullptr;
	}

	std::unique_ptr<IPixelShader> loadPixelShader(std::string path,
		std::string texture = std::string()) const override
	{
		return nullptr;
	}

	std::unique_ptr<IMesh> loadMesh(const std::string& resource_id) const override
	{
		return nullptr;
	}

	std::unique_ptr<IMesh> loadCommonMesh(const CommonMesh mesh_type,
		const std::string& resource_id) const override
	{
		return nullptr;
	}

	PtInputLayout _input_layout;
	PtSamplerState _sampler_state;
	PtTexture _texture;
	PtVertexBuffer _vertex_buffer;
	PtIndexBuffer _index_buffer;
	PtTextMetrics _text_metrics;

	std::shared_ptr<IShaderManager> _shader_manager;
	std::shared_ptr<IRendererState> _render_state;
	std::shared_ptr<MeshResource> _mesh_resource;
	std::shared_ptr<TextureResource> _texture_resource;
	std::shared_ptr<IVertexShader> _vertex_shader;
	std::shared_ptr<IPixelShader> _pixel_shader;
	std::shared_ptr<IMesh> _mesh;

	bool _can_initialize {false};
	bool _can_pre_render {false};
	bool _can_post_render {false};
};
