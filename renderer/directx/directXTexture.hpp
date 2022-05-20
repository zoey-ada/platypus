// #include <string>

// #include "../iTexture.hpp"

// class ID3D11Resource;
// class IRenderer;
// class ResourceCache;

// class DirectXTexture: public ITexture
// {
// public:
// 	explicit DirectXTexture(std::shared_ptr<IRenderer> renderer,
// 		std::shared_ptr<ResourceCache> cache, std::string path);
// 	virtual ~DirectXTexture() = default;

// 	[[nodiscard]] bool initialize(const std::shared_ptr<Scene>& scene) override;

// 	[[nodiscard]] uint64_t height() const override { return 5; }
// 	[[nodiscard]] uint64_t width() const override { return 5; }

// private:
// 	void deinitialize();

// 	std::string _path;

// 	std::shared_ptr<ID3D11Resource> _texture {nullptr};
// 	std::shared_ptr<IRenderer> _renderer {nullptr};
// 	std::weak_ptr<ResourceCache> _cache;
// };
