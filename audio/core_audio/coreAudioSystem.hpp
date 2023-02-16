#pragma once

#include <map>

#include "../iAudioSystem.hpp"

class Channel;
struct IAudioClient;
struct IAudioRenderClient;

class CoreAudioSystem: public IAudioSystem
{
public:
	CoreAudioSystem();
	virtual ~CoreAudioSystem();

	bool initialize(const std::shared_ptr<ResourceCache>& resource_cache) override;
	void deinitialize() override;

	void update(const Milliseconds delta) override;

	void loadSound() override;
	void unloadSound() override;

	ChannelId playSound(const char* sound_name, bool loop, int32_t volume) override;
	void stopSound(ChannelId id) override;
	bool isSoundPlaying(ChannelId id) override;

	void stopAllSounds() override;

private:
	IAudioClient* _output_device {nullptr};
	IAudioRenderClient* _output_render_device {nullptr};
	std::map<ChannelId, Channel*> _channels;
	std::shared_ptr<ResourceCache> _resource_cache {nullptr};
	uint32_t _output_buffer_frame_count {0};
};
