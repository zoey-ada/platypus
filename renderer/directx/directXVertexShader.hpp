#pragma once

#include <string>

#include "../iVertexShader.hpp"

class DirectXRenderer;
class IRenderer;
class ResourceCache;

struct ID3D11Buffer;
struct ID3D11InputLayout;
struct ID3D11VertexShader;

class DirectXVertexShader: public IVertexShader
{
public:
	explicit DirectXVertexShader(std::shared_ptr<IRenderer> renderer,
		std::weak_ptr<ResourceCache> cache, std::string path);
	virtual ~DirectXVertexShader();

	bool initialize(const std::shared_ptr<Scene>& scene) override;
	bool setupRender(const std::shared_ptr<Scene>& scene,
		const std::shared_ptr<SceneNode>& node) override;

private:
	void deinitialize();

	std::string _path {};

	ID3D11VertexShader* _vertex_shader {nullptr};
	ID3D11InputLayout* _input_layout {nullptr};
	std::shared_ptr<DirectXRenderer> _renderer {nullptr};
	std::weak_ptr<ResourceCache> _resource_cache;

	ID3D11Buffer* _matrices {nullptr};
	ID3D11Buffer* _material {nullptr};
	// ID3D11Buffer* _lighting;
};
