#pragma once

#include <cstdint>
#include <memory>

#include <utilities/time/utils.hpp>

class IResourceCache;

using ChannelId = uint32_t;
const ChannelId InvalidChannelId = 0;

class IAudioSystem
{
public:
	virtual ~IAudioSystem() = default;

	virtual bool initialize(const std::shared_ptr<IResourceCache>& resource_cache) = 0;
	virtual void deinitialize() = 0;

	virtual void update(const Milliseconds delta) = 0;

	virtual void loadSound() = 0;
	virtual void unloadSound() = 0;

	virtual ChannelId playSound(const char* sound_name, bool loop, int32_t volume) = 0;
	virtual void stopSound(ChannelId id) = 0;
	virtual bool isSoundPlaying(ChannelId id) = 0;

	virtual void stopAllSounds() = 0;
};
