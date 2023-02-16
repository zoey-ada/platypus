#include "coreAudioSystem.hpp"

#include <list>

#include <mmdeviceapi.h>
#include <resource_cache/resourceCache.hpp>
#include <resource_cache/resources/audioResource.hpp>

#include "wasapiChannel.hpp"

CoreAudioSystem::CoreAudioSystem()
{}

CoreAudioSystem::~CoreAudioSystem()
{}

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

bool CoreAudioSystem::initialize(const std::shared_ptr<ResourceCache>& resource_cache)
{
	if (resource_cache == nullptr)
	{
		// error
		return false;
	}

	this->_resource_cache = resource_cache;

	IMMDeviceEnumerator* device_enumerator = nullptr;
	HRESULT hr = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL,
		IID_IMMDeviceEnumerator, (void**)&device_enumerator);
	if (FAILED(hr))
	{
		// log error
		return false;
	}

	hr = device_enumerator->GetDefaultAudioEndpoint(EDataFlow::eRender, ERole::eConsole,
		&this->_output_device);

	device_enumerator->Release();
	device_enumerator = nullptr;

	if (FAILED(hr))
	{
		// log error
		return false;
	}

	return true;
}

void CoreAudioSystem::deinitialize()
{
	if (this->_output_device != nullptr)
		this->_output_device->Release();
	this->_output_device = nullptr;
}

void CoreAudioSystem::update(const Milliseconds delta)
{
	std::list<ChannelId> finished_channels;

	for (auto [id, channel] : this->_channels)
	{
		channel->update(delta);
		if (!channel->isPlaying())
			finished_channels.push_back(id);
	}

	for (auto id : finished_channels)
		this->_channels.erase(id);
}

void CoreAudioSystem::loadSound()
{}

void CoreAudioSystem::unloadSound()
{}

ChannelId CoreAudioSystem::playSound(const char* sound_name, bool /*loop*/, int32_t /*volume*/)
{
	auto audio_resource = this->_resource_cache->getAudio(sound_name);

	auto channel = std::make_shared<WasapiChannel>();
	if (!channel->initialize(this->_output_device, audio_resource))
	{
		return InvalidChannelId;
	}

	this->_channels[this->getNextChannelId()] = channel;
	channel->play();

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
