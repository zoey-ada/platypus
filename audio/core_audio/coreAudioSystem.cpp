#include "coreAudioSystem.hpp"

#include <list>

#include <mmdeviceapi.h>
#include <resource_cache/iResourceCache.hpp>
#include <resource_cache/resources/audioResource.hpp>
#include <utilities/logging/logger.hpp>

#include "wasapiChannel.hpp"

CoreAudioSystem::CoreAudioSystem()
{}

CoreAudioSystem::~CoreAudioSystem()
{}

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

bool CoreAudioSystem::initialize(const std::shared_ptr<IResourceCache>& resource_cache)
{
	if (resource_cache == nullptr)
	{
		logError("did not receive resource cache", "audio");
		return false;
	}

	this->_resource_cache = resource_cache;

	IMMDeviceEnumerator* device_enumerator = nullptr;
	HRESULT hr = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL,
		IID_IMMDeviceEnumerator, (void**)&device_enumerator);
	if (FAILED(hr))
	{
		logError("could not create audio device enumerator", hr, "audio");
		return false;
	}

	hr = device_enumerator->GetDefaultAudioEndpoint(EDataFlow::eRender, ERole::eConsole,
		&this->_output_device);

	device_enumerator->Release();
	device_enumerator = nullptr;

	if (FAILED(hr))
	{
		logError("could not get default audio endpoint", hr, "audio");
		return false;
	}

	for (uint8_t channel_num = 0; channel_num < this->_number_of_channels; ++channel_num)
	{
		auto channel = std::make_shared<WasapiChannel>();
		channel->startUpdateLoop();

		this->_channels[channel_num] = channel;

		auto update_thread = std::make_shared<std::thread>(&WasapiChannel::update, channel.get());
		this->_threads[channel_num] = update_thread;
	}

	return true;
}

void CoreAudioSystem::deinitialize()
{
	for (auto& channel : this->_channels)
	{
		channel.second->stopUpdateLoop();
	}

	while (!this->_channels.empty())
	{
		std::list<ChannelId> finished_channels;

		for (auto& channel : this->_channels)
		{
			if (channel.second->canCleanup())
			{
				finished_channels.push_back(channel.first);
			}
		}

		for (auto id : finished_channels)
		{
			this->_threads[id]->join();
			this->_threads.erase(id);
			this->_channels.erase(id);
		}
	}

	if (this->_output_device != nullptr)
		this->_output_device->Release();
	this->_output_device = nullptr;
}

void CoreAudioSystem::update(const Milliseconds /*delta*/)
{
	// std::list<ChannelId> finished_channels;

	// for (auto [id, channel] : this->_channels)
	// {
	// 	// channel->update(delta);
	// 	if (!channel->isPlaying())
	// 		channel->deinitialize();

	// 	else if (channel->canCleanup())
	// 		finished_channels.push_back(id);
	// }

	// for (auto id : finished_channels)
	// {
	// 	this->_threads[id]->join();
	// 	this->_threads.erase(id);
	// 	this->_channels.erase(id);
	// }
}

void CoreAudioSystem::loadSound()
{}

void CoreAudioSystem::unloadSound()
{}

ChannelId CoreAudioSystem::playSound(const char* sound_name, bool /*loop*/, int32_t /*volume*/)
{
	logVerbose("begin playSound", "audio");
	auto audio_resource = this->_resource_cache->getAudio(sound_name);

	const auto channel_id = this->getNextChannelId();

	auto channel = this->_channels[channel_id];
	if (!channel->initialize(this->_output_device, audio_resource))
	{
		assert(false);
		return InvalidChannelId;
	}

	channel->play();

	logVerbose("end playSound", "audio");
	return this->_last_channel_id;
}

void CoreAudioSystem::stopSound(ChannelId id)
{
	if (this->_channels.contains(id))
	{
		this->_channels[id]->stop();
		this->_channels.erase(id);
	}
}

bool CoreAudioSystem::isSoundPlaying(ChannelId id)
{
	if (this->_channels.contains(id))
		this->_channels[id]->isPlaying();

	return false;
}

void CoreAudioSystem::stopAllSounds()
{
	for (auto [id, channel] : this->_channels)
	{
		channel->stop();
	}

	this->_channels.clear();
}
