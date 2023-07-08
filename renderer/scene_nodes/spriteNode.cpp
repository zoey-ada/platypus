#include "spriteNode.hpp"

#include <resource_cache/iResourceCache.hpp>
#include <resource_cache/resources/textureResource.hpp>
#include <utilities/uuid.hpp>

#include "../iMesh.hpp"
#include "../iPixelShader.hpp"
#include "../iRenderer.hpp"
#include "../iVertexShader.hpp"
#include "../scene.hpp"

namespace platypus
{

SpriteNode::SpriteNode(SpriteNodeData* data)
	: SceneNode(data->base_node_data),
	  _vertex_shader_resource_id(data->vetex_shader_path),
	  _pixel_shader_resource_id(data->pixel_shader_path),
	  _texture_resource_id(data->texture_path),
	  _mesh_resource_id(this->properties()->name() + generate_simple_uuid()),
	  _sprite_dimensions(data->sprite_dimensions)
{
	if (data->intial_sprite.has_value())
		this->_current_sprite = data->intial_sprite.value();
}

SpriteNode::SpriteNode(SpriteNodeData data)
	: SceneNode(data.base_node_data),
	  _vertex_shader_resource_id(std::move(data.vetex_shader_path)),
	  _pixel_shader_resource_id(std::move(data.pixel_shader_path)),
	  _texture_resource_id(std::move(data.texture_path)),
	  _mesh_resource_id(this->properties()->name() + generate_simple_uuid()),
	  _sprite_dimensions(std::move(data.sprite_dimensions))
{
	if (data.intial_sprite.has_value())
		this->_current_sprite = data.intial_sprite.value();
}

SpriteNode::~SpriteNode()
{}

bool SpriteNode::initialize(const std::shared_ptr<Scene>& scene)
{
	auto renderer = scene->renderer();

	this->_vertex_shader = renderer->loadVertexShader(this->_vertex_shader_resource_id);
	this->_pixel_shader =
		renderer->loadPixelShader(this->_pixel_shader_resource_id, this->_texture_resource_id);

	this->_mesh = renderer->loadCommonMesh(CommonMesh::Rectangle_uFlipped, this->_mesh_resource_id);

	return this->reinitialize(scene);
}

bool SpriteNode::reinitialize(const std::shared_ptr<Scene>& scene)
{
	// hack for reading alpha in textures
	this->properties()->material().setAlpha(0.9999f);

	if (!SceneNode::initialize(scene) || !this->_pixel_shader->initialize(scene) ||
		!this->_vertex_shader->initialize(scene))
	{
		return false;
	}

	// force the sprite to reload
	scene->cache()->touchResource(platypus::ResourceType::Mesh, this->_mesh_resource_id);
	this->calculateSpriteOffsets(this->_pixel_shader->getTexture()->getDimensions());
	this->setSprite(this->_current_sprite);

	// this->SetRadius(1000);

	return true;
}

void SpriteNode::deinitialize(const std::shared_ptr<Scene>& /*scene*/)
{}

bool SpriteNode::render(const std::shared_ptr<Scene>& scene)
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

void SpriteNode::setSprite(const Vec2& coordinate)
{
	this->_current_sprite = coordinate;

	auto sprite_x = this->_sprite_offsets.x * this->_current_sprite.x;
	auto sprite_y = this->_sprite_offsets.y * this->_current_sprite.y;

	std::array<Vec2, 4> verticies;
	verticies[0].x = sprite_x;
	verticies[0].y = sprite_y;
	verticies[1].x = sprite_x + this->_sprite_offsets.x;
	verticies[1].y = sprite_y;
	verticies[2].x = sprite_x;
	verticies[2].y = sprite_y + this->_sprite_offsets.y;
	verticies[3].x = sprite_x + this->_sprite_offsets.x;
	verticies[3].y = sprite_y + this->_sprite_offsets.y;

	this->_mesh->updateVertexTextures(verticies.data(), 4);
}

void SpriteNode::calculateSpriteOffsets(const Extent& texture_dimensions)
{
	this->_sprite_offsets.x =
		static_cast<float>(this->_sprite_dimensions.width) / texture_dimensions.width;
	this->_sprite_offsets.y =
		static_cast<float>(this->_sprite_dimensions.height) / texture_dimensions.height;
}

};
