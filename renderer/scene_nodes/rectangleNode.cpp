#include "rectangleNode.hpp"

#include <d3d11.h>

#include <resource_cache/iResourceCache.hpp>
#include <resource_cache/resources/meshResource.hpp>
#include <resource_cache/resources/textureResource.hpp>
#include <serviceProvider.hpp>

#include "../directx/directXRenderer.hpp"
#include "../iMesh.hpp"
#include "../iPixelShader.hpp"
#include "../iRenderer.hpp"
#include "../iVertexShader.hpp"
#include "../scene.hpp"

RectangleNode::RectangleNode(PtSceneNodeData* base_node_data, PtRectangleNodeData* rect_node_data)
	: SceneNode(base_node_data),
	  _vertex_shader_resource_id(rect_node_data->vetex_shader_path),
	  _pixel_shader_resource_id(rect_node_data->pixel_shader_path),
	  _texture_resource_id(rect_node_data->texture_path)
{}

RectangleNode::~RectangleNode()
{
	// This destructor needs to be in this file because:
	// A type must be defined by the time the unique_ptr destructor is called.
}

bool RectangleNode::initialize(const std::shared_ptr<Scene>& scene)
{
	auto renderer = scene->renderer();

	this->_vertex_shader = renderer->loadVertexShader(this->_vertex_shader_resource_id);
	this->_pixel_shader =
		renderer->loadPixelShader(this->_pixel_shader_resource_id, this->_texture_resource_id);

	this->_mesh = renderer->loadMesh(this->_mesh_resource_id);

	return this->reinitialize(scene);
}

bool RectangleNode::reinitialize(const std::shared_ptr<Scene>& scene)
{
	// hack for reading alpha in textures
	if (!this->_texture_resource_id.empty())
		this->properties()->material().setAlpha(0.9999f);

	if (!SceneNode::initialize(scene) || !this->_pixel_shader->initialize(scene) ||
		!this->_vertex_shader->initialize(scene))
	{
		return false;
	}

	// force the sprite to reload
	auto cache = scene->cache();
	if (!cache->exists(platypus::ResourceType::Mesh, this->_mesh_resource_id))
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

void RectangleNode::deinitialize(const std::shared_ptr<Scene>& /*scene*/)
{}

bool RectangleNode::render(const std::shared_ptr<Scene>& scene)
{
	if (!this->_pixel_shader->setupRender(scene, this->shared_from_this()) ||
		!this->_vertex_shader->setupRender(scene, this->shared_from_this()))
	{
		// error
		return false;
	}

	this->_mesh->render(scene->renderer());
	return true;
}
