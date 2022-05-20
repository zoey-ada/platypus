#pragma once

#include <string>

#include <resource_cache/resources/textureResource.hpp>

#include "../iPixelShader.hpp"

class DirectXRenderer;
class IRenderer;
class ResourceCache;

struct ID3D11Buffer;
struct ID3D11PixelShader;

class DirectXPixelShader: public IPixelShader
{
public:
	explicit DirectXPixelShader(std::shared_ptr<IRenderer> renderer,
		std::shared_ptr<ResourceCache> cache, std::string path,
		std::string texture_path = std::string());
	virtual ~DirectXPixelShader();

	bool initialize(const std::shared_ptr<Scene>& scene) override;
	bool setupRender(const std::shared_ptr<Scene>& scene,
		const std::shared_ptr<SceneNode>& node) override;

	bool setTexture(const std::string& texture_path) override;
	bool setTexture(std::shared_ptr<graphics::Texture> texture,
		std::shared_ptr<graphics::SamplerState> sampler_state);

private:
	void deinitialize();

	std::string _path;
	std::string _texture_path;

	std::shared_ptr<ID3D11PixelShader> _pixel_shader {nullptr};
	std::shared_ptr<DirectXRenderer> _renderer {nullptr};
	std::weak_ptr<ResourceCache> _resource_cache;

	ID3D11Buffer* _material {nullptr};
};
