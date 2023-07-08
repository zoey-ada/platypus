#pragma once

#include "resource.hpp"

namespace platypus
{

struct PtAudioData: public PtResourceData
{
	uint8_t channels;
	uint32_t sample_rate;
	uint16_t bits_per_sample;
	std::byte* audio_data;
	uint32_t audio_data_size;
};

class AudioResource: public Resource
{
public:
	explicit AudioResource(PtAudioData* resource_data);
	virtual ~AudioResource();

	[[nodiscard]] ResourceType type() const override { return ResourceType::Audio; }

	[[nodiscard]] uint8_t channels() const { return this->_channels; }
	[[nodiscard]] uint32_t sample_rate() const { return this->_sample_rate; }
	[[nodiscard]] uint16_t bits_per_sample() const { return this->_bits_per_sample; }
	[[nodiscard]] std::byte* audio_data() const { return this->_audio_data; }
	[[nodiscard]] uint32_t audio_data_size() const { return this->_audio_data_size; }

private:
	uint8_t _channels;
	uint32_t _sample_rate;
	uint16_t _bits_per_sample;
	std::byte* _audio_data {nullptr};
	uint32_t _audio_data_size;
};

};
