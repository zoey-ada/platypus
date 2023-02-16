#include "coreAudioSystem.hpp"

#include <audioclient.h>
#include <mmdeviceapi.h>
#include <mmeapi.h>
#include <resource_cache/resourceCache.hpp>
#include <resource_cache/resources/audioResource.hpp>

CoreAudioSystem::CoreAudioSystem()
{}

CoreAudioSystem::~CoreAudioSystem()
{}

// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC 10000000
#define REFTIMES_PER_MILLISEC 10000

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

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

	IMMDevice* device = nullptr;
	hr = device_enumerator->GetDefaultAudioEndpoint(EDataFlow::eRender, ERole::eConsole, &device);

	device_enumerator->Release();
	device_enumerator = nullptr;

	if (FAILED(hr))
	{
		// log error
		return false;
	}

	device->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&this->_output_device);

	device->Release();
	device = nullptr;

	if (FAILED(hr))
	{
		// log error
		return false;
	}

	return true;
}

void CoreAudioSystem::deinitialize()
{
	if (this->_output_render_device != nullptr)
		this->_output_render_device->Release();
	this->_output_render_device = nullptr;

	if (this->_output_device != nullptr)
		this->_output_device->Release();
	this->_output_device = nullptr;
}

void CoreAudioSystem::update(const Milliseconds /*delta*/)
{}

void CoreAudioSystem::loadSound()
{}

void CoreAudioSystem::unloadSound()
{}

ChannelId CoreAudioSystem::playSound(const char* sound_name, bool /*loop*/, int32_t /*volume*/)
{
	auto audio_resource = this->_resource_cache->getAudio(sound_name);

	WAVEFORMATEX format;
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.cbSize = 0;
	format.nChannels = audio_resource->channels();
	format.wBitsPerSample = audio_resource->bits_per_sample();
	format.nSamplesPerSec = audio_resource->sample_rate();
	format.nBlockAlign = format.nChannels * format.wBitsPerSample / 8;
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

	WAVEFORMATEX closest_format;
	WAVEFORMATEX* closest_format_ptr = &closest_format;

	if (FAILED(this->_output_device->IsFormatSupported(AUDCLNT_SHAREMODE::AUDCLNT_SHAREMODE_SHARED,
			&format, &closest_format_ptr)))
	{
		// log error (unsupported format)
		return InvalidChannelId;
	}

	REFERENCE_TIME requested_duration = REFTIMES_PER_SEC;
	auto hr = this->_output_device->Initialize(AUDCLNT_SHAREMODE::AUDCLNT_SHAREMODE_SHARED, 0,
		requested_duration, 0, &format, nullptr);
	if (FAILED(hr))
	{
		// log error
		return InvalidChannelId;
	}

	hr = this->_output_device->GetBufferSize(&this->_output_buffer_frame_count);
	if (FAILED(hr))
	{
		// log error
		return InvalidChannelId;
	}

	hr = this->_output_device->GetService(IID_IAudioRenderClient,
		(void**)&this->_output_render_device);
	if (FAILED(hr))
	{
		// log error
		return InvalidChannelId;
	}

	BYTE* data = nullptr;
	hr = this->_output_render_device->GetBuffer(this->_output_buffer_frame_count, &data);
	if (FAILED(hr))
	{
		// log error
		return InvalidChannelId;
	}

	auto data_length = this->_output_buffer_frame_count * format.nBlockAlign;
	if (audio_resource->audio_data_size() < data_length)
		data_length = audio_resource->audio_data_size();

	// load the initial data into the shared buffer
	std::memcpy(data, audio_resource->audio_data(), data_length);
	uint32_t audio_offset = data_length;

	DWORD flags = 0;
	hr = this->_output_render_device->ReleaseBuffer(this->_output_buffer_frame_count, flags);
	if (FAILED(hr))
	{
		// log error
		return InvalidChannelId;
	}

	REFERENCE_TIME actual_duration = (REFERENCE_TIME)((double)REFTIMES_PER_SEC *
		this->_output_buffer_frame_count / format.nSamplesPerSec);

	hr = this->_output_device->Start();
	if (FAILED(hr))
	{
		// log error
		return InvalidChannelId;
	}

	while (audio_offset < audio_resource->audio_data_size())
	{
		Sleep((DWORD)(actual_duration / REFTIMES_PER_MILLISEC / 2));

		uint32_t number_frames_padding = 0;
		hr = this->_output_device->GetCurrentPadding(&number_frames_padding);

		uint32_t number_frames_available = this->_output_buffer_frame_count - number_frames_padding;

		hr = this->_output_render_device->GetBuffer(number_frames_available, &data);

		uint32_t number_frames_written = 0;

		// load the next (number_frames_available) of data
		if (audio_offset < audio_resource->audio_data_size())
		{
			data_length = number_frames_available * format.nBlockAlign;
			if (audio_resource->audio_data_size() - audio_offset < data_length)
				data_length = audio_resource->audio_data_size() - audio_offset;

			number_frames_written = data_length / format.nBlockAlign;

			std::memcpy(data, audio_resource->audio_data() + audio_offset, data_length);
			audio_offset += data_length;
		}

		hr = this->_output_render_device->ReleaseBuffer(number_frames_written, flags);
	}

	// wait for sound to end
	Sleep((DWORD)(actual_duration / REFTIMES_PER_MILLISEC / 2));

	hr = this->_output_device->Stop();
	if (FAILED(hr))
	{
		// log error
		return InvalidChannelId;
	}

	return InvalidChannelId;
}

void CoreAudioSystem::stopSound(ChannelId /*id*/)
{}

bool CoreAudioSystem::isSoundPlaying(ChannelId /*id*/)
{
	return false;
}

void CoreAudioSystem::stopAllSounds()
{}
