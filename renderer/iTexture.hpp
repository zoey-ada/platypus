#include <cstdint>
#include <memory>

class Scene;

class ITexture
{
public:
	virtual ~ITexture() = default;

	[[nodiscard]] virtual bool initialize(const std::shared_ptr<Scene>& scene) = 0;

	[[nodiscard]] virtual uint64_t height() const = 0;
	[[nodiscard]] virtual uint64_t width() const = 0;
};
