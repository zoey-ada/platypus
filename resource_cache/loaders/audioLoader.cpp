#include "audioLoader.hpp"

#include <cstring>

#include "../resourceCache.hpp"
#include "../resources/audioResource.hpp"

AudioLoader::AudioLoader(std::shared_ptr<ResourceCache> cache): _cache(cache)
{}

struct RiffChunk
{
	uint32_t riff_tag;
	uint32_t file_size;
	uint32_t file_type;
};

struct WaveChunk
{
	uint32_t fmt_tag;
	uint32_t size;
	uint16_t format_type;
	uint16_t channels;
	uint32_t sample_rate;
	uint32_t average_bytes_per_sec;
	uint16_t block_align;
	uint16_t bits_per_sample;
};

struct DataChunk
{
	uint32_t data_tag;
	uint32_t size;
	std::byte data;
};

std::shared_ptr<Resource> AudioLoader::load(const char* resource_id, const char* store_id,
	std::byte* resource_data, const uint64_t /*data_size*/)
{
	if (strlen(resource_id) == 0 || _cache == nullptr || resource_data == nullptr)
	{
		return nullptr;
	}

	auto riff_chunk = reinterpret_cast<RiffChunk*>(resource_data);
	if (riff_chunk->file_type != 'EVAW')
	{
		// log error
		return nullptr;
	}

	auto wave_chunk = reinterpret_cast<WaveChunk*>(resource_data + sizeof(RiffChunk));
	auto data_chunk =
		reinterpret_cast<DataChunk*>(resource_data + sizeof(RiffChunk) + sizeof(WaveChunk));

	PtAudioData audio_data;
	audio_data.audio_data_size = data_chunk->size;
	audio_data.bits_per_sample = wave_chunk->bits_per_sample;
	audio_data.channels = static_cast<uint8_t>(wave_chunk->channels);
	audio_data.sample_rate = wave_chunk->sample_rate;
	audio_data.resource_id = resource_id;
	audio_data.store_id = store_id;

	audio_data.audio_data = new std::byte[audio_data.audio_data_size];
	std::memcpy(audio_data.audio_data, &data_chunk->data, audio_data.audio_data_size);

	return std::make_shared<AudioResource>(&audio_data);
}

uint8_t* AudioLoader::allocate(unsigned int size)
{
	return this->_cache->allocate(size);
}