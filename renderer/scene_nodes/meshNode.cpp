#include "meshNode.hpp"

#include <resource_cache/resourceCache.hpp>
#include <resource_cache/resources/meshResource.hpp>

#include "../iPixelShader.hpp"
#include "../iRenderer.hpp"
#include "../iVertexShader.hpp"
#include "../scene.hpp"

MeshNode::MeshNode(PtSceneNodeData* base_node_data, PtMeshNodeData* mesh_node_data)
	: SceneNode(base_node_data),
	  _mesh_path(mesh_node_data->mesh_path),
	  _texture_path(mesh_node_data->texture_path),
	  _pixel_shader_path(mesh_node_data->pixel_shader_path),
	  _vertex_shader_path(mesh_node_data->vetex_shader_path)
{}

[[nodiscard]] bool MeshNode::initialize(const std::shared_ptr<Scene>& scene)
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

	// force the mesh to reload
	auto mesh = scene->cache()->getMesh(this->_mesh_path);

	// calculate bounding sphere

	return true;
}

bool MeshNode::render(const std::shared_ptr<Scene>& scene)
{
	if (!this->_pixel_shader->setupRender(scene, this->shared_from_this()) ||
		!this->_vertex_shader->setupRender(scene, this->shared_from_this()))
	{
		return false;
	}

	// get the mesh
	auto mesh_resource = scene->cache()->getMesh(this->_mesh_path);
	scene->renderer()->drawMesh(mesh_resource);
	return true;
}
