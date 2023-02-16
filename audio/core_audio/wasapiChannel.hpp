#pragma once

#include <cstdint>
#include <memory>

#include <Audioclient.h>
#include <mmeapi.h>
#include <utilities/time/utils.hpp>

class AudioResource;
struct IAudioClient;
struct IAudioRenderClient;
struct IMMDevice;

class WasapiChannel
{
public:
	explicit WasapiChannel() = default;
	virtual ~WasapiChannel();

	void update(const Milliseconds delta);

	bool initialize(IMMDevice* device, std::shared_ptr<AudioResource> resource);

	void play();
	void pause();
	void stop();

	inline bool isPlaying() const { return this->_is_playing; }

private:
	IAudioClient* _audio_client {nullptr};
	IAudioRenderClient* _audio_render_client {nullptr};
	std::shared_ptr<AudioResource> _resource {nullptr};
	uint32_t _buffer_frame_count {0};
	uint8_t _frame_size {0};
	WAVEFORMATEX _format {0};

	uint32_t _audio_offset {0};
	bool _is_playing {false};

	void parseFormat();
};
