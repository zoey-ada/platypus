// #include "directXTexture.hpp"

// #include <resource_cache/resourceCache.hpp>
// #include <utilities/logging/logger.hpp>

// #include "directXRenderer.hpp"

// DirectXTexture::DirectXTexture(std::shared_ptr<IRenderer> renderer,
// 	std::shared_ptr<ResourceCache> cache, std::string path)
// 	: _path(std::move(path)), _resource_cache(cache)
// {
// 	if (this->_path.empty())
// 		this->_path = "assets.zip/simple_pixel.hlsl";

// 	_renderer = std::dynamic_pointer_cast<DirectXRenderer>(renderer);
// 	if (_renderer == nullptr)
// 		logWarning(
// 			"attempting to create DirectX pixel shader when not using DirectX "
// 			"renderer",
// 			"renderer_init");
// }

// bool DirectXTexture::initialize(const std::shared_ptr<Scene>& scene)
// {}
