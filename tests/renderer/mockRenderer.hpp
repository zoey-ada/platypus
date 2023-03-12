#pragma once

#include <renderer/iRenderer.hpp>

class MockRenderer: public IRenderer
{
public:
	virtual ~MockRenderer() = default;

	bool initialize(const platypus::RendererSettings& settings,
		const std::weak_ptr<ResourceCache>& cache) override
	{
		return this->_can_initialize;
	}

	void deinitialize() override {}

	bool preRender() override { return this->_can_pre_render; }

	bool postRender() override { return this->_can_post_render; }

	void enableDebugMode() override {}

	std::shared_ptr<IShaderManager> shaderManager() override { return this->_shader_manager; }

	void drawMesh(const std::shared_ptr<MeshResource>& mesh) override {}

	std::shared_ptr<MeshResource> createCommonMesh(const CommonMesh mesh_type) override
	{
		return this->_mesh_resource;
	}

	std::shared_ptr<IRendererState> prepareAlphaPass() override { return this->_render_state; }

	void setBackgroundColor(const Color& backgroundColor) override {}

	void setWorldTransform(const Mat4x4& world) override {}

	PtInputLayout createInputLayout(std::byte* shader_data, const uint64_t data_size,
		PtInputLayoutDesc* layout_elements, const uint64_t element_count) override
	{
		return this->_input_layout;
	}

	void destroyInputLayout(PtInputLayout layout) override {}

	PtSamplerState createSamplerState(const PtAddressOverscanMode overscan_mode) override
	{
		return this->_sampler_state;
	}

	void destroySamplerState(PtSamplerState sampler_state) override {}

	PtTexture createTexture(std::byte* shader_data, const uint64_t data_size) override
	{
		return this->_texture;
	}

	void destroyTexture(PtTexture texture) override {}

	PtVertexBuffer createVertexBuffer(const graphics::Vertex* vertices,
		const uint64_t vertex_count) override
	{
		return this->_vertex_buffer;
	}

	void destroyVertexBuffer(PtVertexBuffer buffer) override {}

	PtIndexBuffer createIndexBuffer(const uint32_t* indices, const uint64_t index_count) override
	{
		return this->_index_buffer;
	}

	void destroyIndexBuffer(PtIndexBuffer buffer) override {}

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

	std::shared_ptr<IVertexShader> loadVertexShader(std::string path) override
	{
		return this->_vertex_shader;
	}

	std::shared_ptr<IPixelShader> loadPixelShader(std::string path,
		std::string texture = std::string()) override
	{
		return this->_pixel_shader;
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

	bool _can_initialize {false};
	bool _can_pre_render {false};
	bool _can_post_render {false};
};
