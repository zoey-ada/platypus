#pragma once

#include <string>

#include <resource_cache/resources/textureResource.hpp>

#include "../iPixelShader.hpp"

class DirectXRenderer;
class IRenderer;
class IResourceCache;

struct ID3D11Buffer;
struct ID3D11PixelShader;

class DirectXPixelShader: public IPixelShader
{
public:
	explicit DirectXPixelShader(std::shared_ptr<const IRenderer> renderer,
		std::shared_ptr<IResourceCache> cache, std::string path,
		std::string texture_path = std::string());
	virtual ~DirectXPixelShader();

	bool initialize(const std::shared_ptr<Scene>& scene) override;
	bool setupRender(const std::shared_ptr<Scene>& scene,
		const std::shared_ptr<SceneNode>& node) override;

	std::shared_ptr<TextureResource> getTexture() override;
	bool setTexture(const std::string& texture_path) override;
	bool setTexture(PtTexture texture, PtSamplerState sampler_state);

private:
	void deinitialize();

	std::string _path;
	std::string _texture_path;

	ID3D11PixelShader* _pixel_shader {nullptr};
	std::shared_ptr<const DirectXRenderer> _renderer {nullptr};
	std::weak_ptr<IResourceCache> _resource_cache;

	ID3D11Buffer* _material {nullptr};
};
