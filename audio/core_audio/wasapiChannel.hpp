#pragma once

#include <cstdint>
#include <memory>
#include <mutex>

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

	bool initialize(IMMDevice* device, std::shared_ptr<AudioResource> resource);
	void deinitialize();

	void startUpdateLoop() { this->_should_run = true; }
	void stopUpdateLoop() { this->_should_run = false; }
	bool canCleanup() const { return this->_can_cleanup; }

	void update();

	void play() { this->_play_requested = true; }
	void pause() { this->_pause_requested = true; }
	void stop() { this->_stop_requested = true; }

	inline bool isPlaying() { return this->_is_playing; }

private:
	IAudioClient* _audio_client {nullptr};
	IAudioRenderClient* _audio_render_client {nullptr};
	std::shared_ptr<AudioResource> _resource {nullptr};
	uint32_t _buffer_frame_count {0};
	uint8_t _frame_size {0};
	WAVEFORMATEX _format;

	uint32_t _audio_offset {0};
	bool _is_playing {false};
	bool _should_run {false};
	bool _can_cleanup {false};

	bool _play_requested {false};
	bool _stop_requested {false};
	bool _pause_requested {false};

	void parseFormat();

	void performPlay();
	void loadNextChunk();
	void performStop();
	void performPause();

	std::string getErrorMessage(HRESULT hr);
};
