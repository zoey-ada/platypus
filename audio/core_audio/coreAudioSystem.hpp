#pragma once

#include <map>
#include <thread>

#include "../iAudioSystem.hpp"

class Channel;
class WasapiChannel;
struct IMMDevice;

namespace platypus
{
class IResourceCache;
};

class CoreAudioSystem: public IAudioSystem
{
public:
	CoreAudioSystem();
	virtual ~CoreAudioSystem();

	bool initialize(const std::shared_ptr<platypus::IResourceCache>& resource_cache) override;
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
	std::shared_ptr<platypus::IResourceCache> _resource_cache {nullptr};

	uint8_t _number_of_channels {3};
	std::map<ChannelId, std::shared_ptr<WasapiChannel>> _channels;
	std::map<ChannelId, std::shared_ptr<std::thread>> _threads;

	ChannelId _last_channel_id {InvalidChannelId};

	inline ChannelId getNextChannelId()
	{
		++this->_last_channel_id;
		return this->_last_channel_id % this->_number_of_channels;
	}
};
