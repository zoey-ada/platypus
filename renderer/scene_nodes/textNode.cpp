#include "textNode.hpp"

#include <renderer/iPixelShader.hpp>
#include <renderer/iRenderer.hpp>
#include <renderer/iVertexShader.hpp>
#include <resource_cache/resourceCache.hpp>
#include <resource_cache/resources/meshResource.hpp>
#include <resource_cache/resources/textureResource.hpp>

#include "../scene.hpp"
#include "../textRenderer.hpp"

TextNode::TextNode(PtTextNodeDesc* text_node_desc)
	: SceneNode(text_node_desc->base_node_data),
	  _message(text_node_desc->message),
	  _pixel_shader_path(text_node_desc->pixel_shader_path),
	  _vertex_shader_path(text_node_desc->vertex_shader_path)
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
	this->generateTexture(cache, scene->renderer());

	// force the sprite to reload
	if (!cache->exists(ResourceType::Mesh, "rectangle_1x1_u_flipped"))
	{
		auto rect = scene->renderer()->createCommonMesh(CommonMesh::Rectangle_uFlipped);
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
	if (this->_texture_is_out_of_date)
	{
		this->generateTexture(scene->cache(), scene->renderer());
	}

	if (!this->_pixel_shader->setupRender(scene, this->shared_from_this()) ||
		!this->_vertex_shader->setupRender(scene, this->shared_from_this()))
	{
		return false;
	}

	// get the mesh
	auto mesh_resource = scene->cache()->getMesh("rectangle_1x1_u_flipped");
	scene->renderer()->drawMesh(mesh_resource);
	return true;
}

void TextNode::setText(const char* new_text)
{
	this->_message = new_text;
	this->_texture_is_out_of_date = true;
}

void TextNode::generateTexture(const std::shared_ptr<ResourceCache>& cache,
	const std::shared_ptr<IRenderer>& renderer)
{
	if (!cache->exists(ResourceType::Texture, this->_message))
	{
		auto texture = renderer->rasterizeText(this->_message.c_str(), "hack.ttf", 48);
		// "umeboshi_.ttf", 48);

		if (!cache->addResource(texture))
		{
			// error
		}
	}

	this->_pixel_shader->setTexture(this->_message);
	this->_texture_is_out_of_date = false;
}
