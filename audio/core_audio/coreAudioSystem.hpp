#pragma once

#include <map>

#include "../iAudioSystem.hpp"

class Channel;
class WasapiChannel;
struct IMMDevice;

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
	IMMDevice* _output_device {nullptr};
	std::map<ChannelId, std::shared_ptr<WasapiChannel>> _channels;
	std::shared_ptr<ResourceCache> _resource_cache {nullptr};

	ChannelId _last_channel_id {InvalidChannelId};

	inline ChannelId getNextChannelId()
	{
		++this->_last_channel_id;
		return this->_last_channel_id;
	}
};
