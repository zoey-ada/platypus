#include "rectangleNode.hpp"

#include <sstream>

// #include "../../application_layer/ApplicationFactory.h"
// #include "../../application_layer/IApplicationLayer.h"
// #include "../../event_system/Events.h"
// #include "../../event_system/IEventManager.h"
// #include "<resource_cache/loaders/DirectXMeshLoader.hpp>
// #include "<resource_cache/resources/MeshResource.hpp>
#include <resource_cache/ResourceCache.hpp>
// #include <resource_cache/resources/SpriteResource.hpp>
#include <resource_cache/resources/TextureResource.hpp>

#include "../directx/directXPixelShader.hpp"
#include "../directx/directXRenderer.hpp"
#include "../directx/directXVertexShader.hpp"

RectangleNode::RectangleNode(const ActorId actorId, const std::string name, const Color color,
	RenderPass renderPass, const uint32_t height, const uint32_t width, const float x,
	const float y, const std::string pixelShaderFilename, const std::string vertexShaderFilename)
	: SceneNode(actorId, name, renderPass, Color::green, Mat4x4::Identity()),
	  _color(color),
	  _height(height),
	  _width(width),
	  _x(x),
	  _y(y),
	  _pixelShaderFilename(pixelShaderFilename),
	  _vertexShaderFilename(vertexShaderFilename)
{
	this->_pixelShader = std::make_shared<DirectXPixelShader>(pixelShaderFilename, "");

	this->_vertexShader = std::make_shared<DirectXVertexShader>(vertexShaderFilename);

	this->SetPosition(Vec2(x, -y));

	Material material;
	material.SetDiffuse(color);
	this->SetMaterial(material);
	this->SetAlpha(color.a);
}

bool RectangleNode::onRestore(std::shared_ptr<Scene> scene)
{
	if (!SceneNode::OnRestore(scene) || !this->_pixelShader->OnRestore(scene) ||
		!this->_vertexShader->OnRestore(scene))
	{
		return false;
	}

	// force the sprite to reload
	auto appLayer = std::shared_ptr<ApplicationLayerFactory>()->GetApplicationLayer();
	auto resCache = appLayer->_resourceCache;
	if (!resCache->Exists(ResourceType::Mesh, this->GenerateMeshName()))
	{
		DirectXMeshLoader meshLoader;
		bool added = resCache->AddResource(meshLoader.CreateRectangle(resCache,
			this->GenerateMeshName(), this->_height, this->_width));

		if (!added)
		{
			// log error about creating sprite resource
		}
	}

	this->SetRadius(1000);

	return true;
}

bool RectangleNode::render(std::shared_ptr<Scene> scene)
{
	if (!this->_pixelShader->SetupRender(scene, this->shared_from_this()) ||
		!this->_vertexShader->SetupRender(scene, this->shared_from_this()))
	{
		return false;
	}

	auto screenHeight = 720;

	// IA setup
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	auto appLayer = std::shared_ptr<ApplicationLayerFactory>()->GetApplicationLayer();
	auto resCache = appLayer->_resourceCache;
	auto meshResource = std::dynamic_pointer_cast<MeshResource>(
		resCache->GetResource(ResourceType::Mesh, this->GenerateMeshName()));
	auto vertexBuffer = meshResource->GetVertexBuffer().get();

	auto renderer = std::dynamic_pointer_cast<DirectXRenderer>(appLayer->_renderer);
	renderer->context()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	renderer->context()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	renderer->context()->Draw(4, 0);

	return true;
}

// void RectangleNode::ActorMovedDelegate(std::shared_ptr<IEvent> data)
// {
// 	auto d = std::dynamic_pointer_cast<ActorMovedEvent>(data);

// 	if (d->GetId() == this->_properties->actor_id())
// 	{
// 		this->_x = d->GetX();
// 		this->_y = d->GetY();
// 		this->SetPosition(Vec2(this->_x, -this->_y));
// 	}
// }

std::string RectangleNode::GenerateMeshName()
{
	std::stringstream ss;
	ss << "generated__rectangle_" << this->_height << "_" << this->_width;
	return ss.str();
}
