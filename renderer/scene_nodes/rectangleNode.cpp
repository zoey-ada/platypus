#include "rectangleNode.hpp"

#include <sstream>

#include <resource_cache/resourceCache.hpp>
#include <resource_cache/resources/meshResource.hpp>

#include "../iPixelShader.hpp"
#include "../iRenderer.hpp"
#include "../iVertexShader.hpp"
#include "../scene.hpp"

RectangleNode::RectangleNode(PtSceneNodeData* base_node_data, PtRectangleNodeData* rect_node_data)
	: SceneNode(base_node_data),
	  _texture_path(rect_node_data->texture_path),
	  _pixel_shader_path(rect_node_data->pixel_shader_path),
	  _vertex_shader_path(rect_node_data->vetex_shader_path)
{}

bool RectangleNode::initialize(const std::shared_ptr<Scene>& scene)
{
	if (this->_vertex_shader == nullptr)
	{
		this->_vertex_shader = scene->renderer()->loadVertexShader(this->_vertex_shader_path);
	}
	if (this->_pixel_shader == nullptr)
	{
		this->_pixel_shader =
			scene->renderer()->loadPixelShader(this->_pixel_shader_path, this->_texture_path);
	}

	if (!SceneNode::initialize(scene) || !this->_pixel_shader->initialize(scene) ||
		!this->_vertex_shader->initialize(scene))
	{
		return false;
	}

	// force the sprite to reload
	auto cache = scene->cache();
	if (!cache->exists(ResourceType::Mesh, "rectangle_1x1"))
	{
		auto rect = scene->renderer()->createRectangle();
		if (!cache->addResource(rect))
		{
			// log error about creating sprite resource
		}
	}

	// this->SetRadius(1000);

	return true;
}

bool RectangleNode::render(const std::shared_ptr<Scene>& scene)
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
