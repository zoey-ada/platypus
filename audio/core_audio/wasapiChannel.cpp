#include "wasapiChannel.hpp"

#include <audioclient.h>
#include <mmdeviceapi.h>
#include <resource_cache/resources/audioResource.hpp>

#define REFTIMES_PER_SEC 10000000
// #define REFTIMES_PER_MILLISEC 10000

const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

WasapiChannel::~WasapiChannel()
{
	if (this->_audio_render_client != nullptr)
		this->_audio_render_client->Release();
	this->_audio_render_client = nullptr;

	if (this->_audio_client != nullptr)
		this->_audio_client->Release();
	this->_audio_client = nullptr;
}

void WasapiChannel::update(const Milliseconds /*delta*/)
{
	if (!this->_is_playing)
	{
		return;
	}

	uint32_t number_frames_padding = 0;
	auto hr = this->_audio_client->GetCurrentPadding(&number_frames_padding);

	uint32_t number_frames_available = this->_buffer_frame_count - number_frames_padding;

	if (this->_audio_offset >= this->_resource->audio_data_size())
		if (number_frames_available == this->_buffer_frame_count)
		{
			this->stop();
			return;
		}
		else
		{
			return;
		}

	BYTE* data = nullptr;
	hr = this->_audio_render_client->GetBuffer(number_frames_available, &data);

	uint32_t number_frames_written = 0;

	// load the next (number_frames_available) of data
	if (this->_audio_offset < this->_resource->audio_data_size())
	{
		auto data_length = number_frames_available * this->_format.nBlockAlign;
		if (this->_resource->audio_data_size() - this->_audio_offset < data_length)
			data_length = this->_resource->audio_data_size() - this->_audio_offset;

		number_frames_written = data_length / this->_format.nBlockAlign;

		std::memcpy(data, this->_resource->audio_data() + this->_audio_offset, data_length);
		this->_audio_offset += data_length;
	}

	hr = this->_audio_render_client->ReleaseBuffer(number_frames_written, 0);
}

bool WasapiChannel::initialize(IMMDevice* device, std::shared_ptr<AudioResource> resource)
{
	this->_resource = resource;

	auto hr = device->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&this->_audio_client);

	device = nullptr;

	if (FAILED(hr))
	{
		// log error
		return false;
	}

	this->parseFormat();

	WAVEFORMATEX closest_format {0};
	WAVEFORMATEX* closest_format_ptr = &closest_format;

	hr = this->_audio_client->IsFormatSupported(AUDCLNT_SHAREMODE::AUDCLNT_SHAREMODE_SHARED,
		&this->_format, &closest_format_ptr);

	if (FAILED(hr))
	{
		// log error (unsupported format)
		return false;
	}

	REFERENCE_TIME requested_duration = REFTIMES_PER_SEC;
	hr = this->_audio_client->Initialize(AUDCLNT_SHAREMODE::AUDCLNT_SHAREMODE_SHARED, 0,
		requested_duration, 0, &this->_format, nullptr);
	if (FAILED(hr))
	{
		// log error
		return false;
	}

	hr = this->_audio_client->GetService(IID_IAudioRenderClient,
		(void**)&this->_audio_render_client);
	if (FAILED(hr))
	{
		// log error
		return false;
	}

	hr = this->_audio_client->GetBufferSize(&this->_buffer_frame_count);
	if (FAILED(hr))
	{
		// log error
		return false;
	}

	return true;
}

void WasapiChannel::play()
{
	BYTE* data = nullptr;
	auto hr = this->_audio_render_client->GetBuffer(this->_buffer_frame_count, &data);
	if (FAILED(hr))
	{
		// log error
		return;
	}

	auto data_length = this->_buffer_frame_count * this->_format.nBlockAlign;
	if (this->_resource->audio_data_size() < data_length)
		data_length = this->_resource->audio_data_size();

	// load the initial data into the shared buffer
	std::memcpy(data, this->_resource->audio_data(), data_length);
	this->_audio_offset = data_length;

	DWORD flags = 0;
	hr = this->_audio_render_client->ReleaseBuffer(this->_buffer_frame_count, flags);
	if (FAILED(hr))
	{
		// log error
		return;
	}

	if (FAILED(this->_audio_client->Start()))
	{
		// log error
		return;
	}

	this->_is_playing = true;
}

void WasapiChannel::pause()
{
	this->_is_playing = false;

	if (FAILED(this->_audio_client->Stop()))
	{
		// log error
		return;
	}
}

void WasapiChannel::stop()
{
	this->_is_playing = false;

	if (FAILED(this->_audio_client->Stop()))
	{
		// log error
	}

	if (FAILED(this->_audio_client->Reset()))
	{
		// log error
	}

	this->_audio_offset = 0;
}

void WasapiChannel::parseFormat()
{
	this->_format.wFormatTag = WAVE_FORMAT_PCM;
	this->_format.cbSize = 0;
	this->_format.nChannels = _resource->channels();
	this->_format.wBitsPerSample = _resource->bits_per_sample();
	this->_format.nSamplesPerSec = _resource->sample_rate();
	this->_format.nBlockAlign = this->_format.nChannels * this->_format.wBitsPerSample / 8;
	this->_format.nAvgBytesPerSec = this->_format.nSamplesPerSec * this->_format.nBlockAlign;
}
