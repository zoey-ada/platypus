#include "wasapiChannel.hpp"

#include <audioclient.h>
#include <mmdeviceapi.h>
#include <resource_cache/resources/audioResource.hpp>
#include <utilities/logging/logger.hpp>

#define REFTIMES_PER_SEC 10000000
// #define REFTIMES_PER_MILLISEC 10000

const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

WasapiChannel::~WasapiChannel()
{
	this->deinitialize();
}

bool WasapiChannel::initialize(IMMDevice* device, std::shared_ptr<AudioResource> resource)
{
	this->deinitialize();
	auto hr = device->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&this->_audio_client);

	device = nullptr;

	if (FAILED(hr))
	{
		logError("could not activate audio device", hr, "audio");
		return false;
	}

	this->_resource = std::move(resource);
	this->parseFormat();

	WAVEFORMATEX closest_format;
	WAVEFORMATEX* closest_format_ptr = &closest_format;

	hr = this->_audio_client->IsFormatSupported(AUDCLNT_SHAREMODE::AUDCLNT_SHAREMODE_SHARED,
		&this->_format, &closest_format_ptr);

	if (FAILED(hr))
	{
		// log error (unsupported format)
		logError("audio format is unsupported", hr, "audio");
		return false;
	}

	REFERENCE_TIME requested_duration = REFTIMES_PER_SEC / 10;
	hr = this->_audio_client->Initialize(AUDCLNT_SHAREMODE::AUDCLNT_SHAREMODE_SHARED, 0,
		requested_duration, 0, &this->_format, nullptr);
	if (FAILED(hr))
	{
		logError("could not initialize audio device", hr, "audio");
		logError(this->getErrorMessage(hr));
		return false;
	}

	hr = this->_audio_client->GetService(IID_IAudioRenderClient,
		(void**)&this->_audio_render_client);
	if (FAILED(hr) || this->_audio_render_client == nullptr)
	{
		logError("could not get audio render device", hr, "audio");
		return false;
	}

	hr = this->_audio_client->GetBufferSize(&this->_buffer_frame_count);
	if (FAILED(hr) || this->_buffer_frame_count == 0)
	{
		logError("could not get audio device buffer size", hr, "audio");
		return false;
	}

	this->_should_run = true;

	return true;
}

void WasapiChannel::deinitialize()
{
	if (this->_audio_render_client != nullptr)
		this->_audio_render_client->Release();
	this->_audio_render_client = nullptr;

	if (this->_audio_client != nullptr)
		this->_audio_client->Release();
	this->_audio_client = nullptr;
}

void WasapiChannel::update()
{
	while (this->_should_run)
	{
		if (this->_play_requested)
		{
			_play_requested = false;
			this->performPlay();
		}
		else if (this->_pause_requested)
		{
			_pause_requested = false;
			this->performPause();
		}
		else if (this->_stop_requested)
		{
			_stop_requested = false;
			this->performStop();
		}
		else if (this->_is_playing)
		{
			this->loadNextChunk();
		}

		const Milliseconds frame_time_60fps = 32;
		Sleep(frame_time_60fps);
	}

	this->_can_cleanup = true;
}

void WasapiChannel::performPlay()
{
	this->_is_playing = true;
	logVerbose("begin WasapiChannel::play", "audio");
	BYTE* data = nullptr;
	logVerbose("begin WasapiChannel::play get audio buffer", "audio");
	auto hr = this->_audio_render_client->GetBuffer(this->_buffer_frame_count, &data);
	logVerbose("end WasapiChannel::play get audio buffer", "audio");
	if (FAILED(hr))
	{
		logError("could not get audio buffer", hr, "audio");
		return;
	}

	logVerbose("begin WasapiChannel::play determine data length", "audio");
	auto data_length = this->_buffer_frame_count * this->_format.nBlockAlign;
	if (this->_resource->audio_data_size() < data_length)
		data_length = this->_resource->audio_data_size();
	logVerbose("end WasapiChannel::play determine data length", "audio");

	// load the initial data into the shared buffer
	logVerbose("begin WasapiChannel::play copy audio data", "audio");
	std::memcpy(data, this->_resource->audio_data(), data_length);
	logVerbose("end WasapiChannel::play copy audio data", "audio");
	this->_audio_offset = data_length;

	const DWORD flags = 0;
	logVerbose("begin WasapiChannel::play release audio buffer", "audio");
	hr = this->_audio_render_client->ReleaseBuffer(this->_buffer_frame_count, flags);
	logVerbose("end WasapiChannel::play release audio buffer", "audio");
	if (FAILED(hr))
	{
		logError("could not release audio buffer", hr, "audio");
		return;
	}

	logVerbose("begin WasapiChannel::play play buffer contents", "audio");
	if (FAILED(this->_audio_client->Start()))
	{
		logError("could not play audio buffer contents", "audio");
		return;
	}
	logVerbose("end WasapiChannel::play play buffer contents", "audio");

	logVerbose("end WasapiChannel::play", "audio");
}

void WasapiChannel::loadNextChunk()
{
	uint32_t number_frames_padding = 0;
	/*auto hr = */ this->_audio_client->GetCurrentPadding(&number_frames_padding);

	const uint32_t number_frames_available = this->_buffer_frame_count - number_frames_padding;

	if (this->_audio_offset >= this->_resource->audio_data_size())
	{
		if (number_frames_available == this->_buffer_frame_count)
		{
			this->stop();
		}

		return;
	}

	BYTE* data = nullptr;
	/*hr = */ this->_audio_render_client->GetBuffer(number_frames_available, &data);

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

	/*hr = */ this->_audio_render_client->ReleaseBuffer(number_frames_written, 0);
}

void WasapiChannel::performPause()
{
	this->_is_playing = false;

	if (FAILED(this->_audio_client->Stop()))
	{
		logError("could not stop audio device", "audio");
		return;
	}
}

void WasapiChannel::performStop()
{
	this->_is_playing = false;

	if (FAILED(this->_audio_client->Stop()))
	{
		logError("could not stop audio device", "audio");
	}

	if (FAILED(this->_audio_client->Reset()))
	{
		logError("could not reset audio device", "audio");
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

std::string WasapiChannel::getErrorMessage(HRESULT hr)
{
	switch (hr)
	{
	case AUDCLNT_E_ALREADY_INITIALIZED:
		return "The IAudioClient object is already initialized.";
	case AUDCLNT_E_WRONG_ENDPOINT_TYPE:
		return "The AUDCLNT_STREAMFLAGS_LOOPBACK flag is set but the endpoint device is a capture "
			   "device, not a rendering device.";
	case AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED:
		return "The requested buffer size is not aligned. This code can be returned for a render "
			   "or a capture device if the caller specified AUDCLNT_SHAREMODE_EXCLUSIVE and the "
			   "AUDCLNT_STREAMFLAGS_EVENTCALLBACK flags. The caller must call Initialize again "
			   "with the aligned buffer size. For more information, see Remarks.";
	case AUDCLNT_E_BUFFER_SIZE_ERROR:
		return "Indicates that the buffer duration value requested by an exclusive-mode client is "
			   "out of range. The requested duration value for pull mode must not be greater than "
			   "5000 milliseconds; for push mode the duration value must not be greater than 2 "
			   "seconds.";
	case AUDCLNT_E_CPUUSAGE_EXCEEDED:
		return "Indicates that the process-pass duration exceeded the maximum CPU usage. The audio "
			   "engine keeps track of CPU usage by maintaining the number of times the "
			   "process-pass duration exceeds the maximum CPU usage. The maximum CPU usage is "
			   "calculated as a percent of the engine's periodicity. The percentage value is the "
			   "system's CPU throttle value (within the range of 10% and 90%). If this value is "
			   "not found, then the default value of 40% is used to calculate the maximum CPU "
			   "usage.";
	case AUDCLNT_E_DEVICE_INVALIDATED:
		return "The audio endpoint device has been unplugged, or the audio hardware or associated "
			   "hardware resources have been reconfigured, disabled, removed, or otherwise made "
			   "unavailable for use.";
	case AUDCLNT_E_DEVICE_IN_USE:
		return "The endpoint device is already in use. Either the device is being used in "
			   "exclusive mode, or the device is being used in shared mode and the caller asked to "
			   "use the device in exclusive mode.";
	case AUDCLNT_E_ENDPOINT_CREATE_FAILED:
		return "The method failed to create the audio endpoint for the render or the capture "
			   "device. This can occur if the audio endpoint device has been unplugged, or the "
			   "audio hardware or associated hardware resources have been reconfigured, disabled, "
			   "removed, or otherwise made unavailable for use.";
	case AUDCLNT_E_INVALID_DEVICE_PERIOD:
		return "Indicates that the device period requested by an exclusive-mode client is greater "
			   "than 5000 milliseconds.";
	case AUDCLNT_E_UNSUPPORTED_FORMAT:
		return "The audio engine (shared mode) or audio endpoint device (exclusive mode) does not "
			   "support the specified format.";
	case AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED:
		return "The caller is requesting exclusive-mode use of the endpoint device, but the user "
			   "has disabled exclusive-mode use of the device.";
	case AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL:
		return "The AUDCLNT_STREAMFLAGS_EVENTCALLBACK flag is set but parameters hnsBufferDuration "
			   "and hnsPeriodicity are not equal.";
	case AUDCLNT_E_SERVICE_NOT_RUNNING:
		return "The Windows audio service is not running.";
	case E_POINTER:
		return "Parameter pFormat is NULL.";
	case E_INVALIDARG:
		return "Parameter pFormat points to an invalid format description; or the "
			   "AUDCLNT_STREAMFLAGS_LOOPBACK flag is set but ShareMode is not equal to "
			   "AUDCLNT_SHAREMODE_SHARED; or the AUDCLNT_STREAMFLAGS_CROSSPROCESS flag is set but "
			   "ShareMode is equal to AUDCLNT_SHAREMODE_EXCLUSIVE. A prior call to "
			   "SetClientProperties was "
			   "made with an invalid category for audio/render streams.";
	case E_OUTOFMEMORY:
		return "Out of memory.";
	default:
		return "Unknown error";
	}
}
