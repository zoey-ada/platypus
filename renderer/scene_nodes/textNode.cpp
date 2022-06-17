#include "textNode.hpp"

#include <d3d11.h>

#include <renderer/directx/directXRenderer.hpp>
#include <renderer/iPixelShader.hpp>
#include <renderer/iRenderer.hpp>
#include <renderer/iVertexShader.hpp>
#include <resource_cache/loaders/directXTextureLoader.hpp>
#include <resource_cache/resourceCache.hpp>
#include <resource_cache/resources/meshResource.hpp>
#include <resource_cache/resources/textureResource.hpp>

#include "../scene.hpp"
#include "../textRenderer.hpp"

TextNode::TextNode(PtTextNodeDesc* text_node_desc)
	: SceneNode(text_node_desc->base_node_data),
	  _pixel_shader_path(text_node_desc->pixel_shader_path),
	  _vertex_shader_path(text_node_desc->vetex_shader_path),
	  _message(text_node_desc->message)
{
	this->properties()->material().setAlpha(0.9999f);
}

bool TextNode::initialize(const std::shared_ptr<Scene>& scene)
{
	if (this->_vertex_shader == nullptr)
	{
		this->_vertex_shader = scene->renderer()->loadVertexShader(this->_vertex_shader_path);
	}
	if (this->_pixel_shader == nullptr)
	{
		this->_pixel_shader = scene->renderer()->loadPixelShader(this->_pixel_shader_path);
	}

	if (!SceneNode::initialize(scene) || !this->_pixel_shader->initialize(scene) ||
		!this->_vertex_shader->initialize(scene))
	{
		return false;
	}

	auto cache = scene->cache();

	if (!cache->exists(ResourceType::Mesh, this->_message))
	{
		auto d3d_renderer = std::dynamic_pointer_cast<DirectXRenderer>(scene->renderer());
		auto texture = DirectXTextureLoader::rasterizeText(d3d_renderer, this->_message.c_str(),
			"hack.ttf", 48);
		// "umeboshi_.ttf", 48);

		if (!cache->addResource(texture))
		{
			// error
		}
	}

	this->_pixel_shader->setTexture(this->_message);

	// force the sprite to reload
	if (!cache->exists(ResourceType::Mesh, "rectangle_1x1"))
	{
		auto rect = scene->renderer()->createTextRectangle();
		if (!cache->addResource(rect))
		{
			// log error about creating sprite resource
		}
	}

	// this->SetRadius(1000);

	return true;
}

bool TextNode::render(const std::shared_ptr<Scene>& scene)
{
	if (!this->_pixel_shader->setupRender(scene, this->shared_from_this()) ||
		!this->_vertex_shader->setupRender(scene, this->shared_from_this()))
	{
		return false;
	}

	// get the mesh
	auto mesh_resource = scene->cache()->getMesh("rectangle_1x1");
	scene->renderer()->drawMesh(mesh_resource);
	return true;
}
