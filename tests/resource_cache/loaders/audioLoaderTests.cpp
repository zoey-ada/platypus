#include <array>

#include <catch2/catch.hpp>

#include <resource_cache/loaders/audioLoader.hpp>
#include <resource_cache/resources/audioResource.hpp>
#include <tests/resource_cache/mockResourceCache.hpp>
#include <tests/utilities/logging/mockLoggingSystem.hpp>

using namespace platypus;

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

#pragma pack(1)
struct DataChunk
{
	uint32_t data_tag;
	uint32_t size;
	std::byte data;
};

SCENARIO("audio loader can load a wav file", "[res_cache]")
{
	auto logging = std::make_shared<MockLoggingSystem>();
	auto resource_cache = std::make_shared<MockResourceCache>();

	std::string store_id("assets.zip");
	uint64_t data_size = 0;
	std::byte res_data[] {std::byte {0xff}};
	std::string png_res_id("image.png");

	GIVEN("an audio loader with no resource cache")
	{
		AudioLoader loader(nullptr, logging);

		WHEN("a wav is loaded")
		{
			auto resource = loader.load(png_res_id.c_str(), store_id.c_str(), res_data, data_size);

			THEN("the resouce fails to be created")
			{
				REQUIRE(resource == nullptr);
				REQUIRE(logging->_warning_logs.size() == 1);
			}
		}
	}

	GIVEN("an audio loader with a resource cache")
	{
		AudioLoader loader(resource_cache, logging);

		WHEN("invalid resource data is used to load")
		{
			auto resource = loader.load(png_res_id.c_str(), store_id.c_str(), nullptr, data_size);

			THEN("the resouce fails to be created")
			{
				REQUIRE(resource == nullptr);
				REQUIRE(logging->_warning_logs.size() == 1);
			}
		}

		WHEN("no resource id is used to load")
		{
			auto resource = loader.load("", store_id.c_str(), res_data, data_size);

			THEN("the resouce fails to be created")
			{
				REQUIRE(resource == nullptr);
				REQUIRE(logging->_warning_logs.size() == 1);
			}
		}

		AND_GIVEN("the audio data is not a wav file")
		{
			std::byte res_data[] {std::byte {0xff}};

			WHEN("a wav file is loaded")
			{
				auto resource =
					loader.load(png_res_id.c_str(), store_id.c_str(), res_data, data_size);

				THEN("the resouce fails to be created")
				{
					REQUIRE(resource == nullptr);
					REQUIRE(logging->_warning_logs.size() == 1);
				}
			}
		}

		AND_GIVEN("the audio data is a valid wav file")
		{
			std::byte audio_data[] {std::byte {0xff}, std::byte {0x44}, std::byte {0x64},
				std::byte {0x15}, std::byte {0x77}};

			// contains the first element of the audio_data
			DataChunk data_data {'atad', sizeof(audio_data), std::byte {0xff}};
			WaveChunk wave_data {' tmf', sizeof(wave_data) - 8, 1, 2, 48000, 768000, 16, 8};
			RiffChunk riff_data {'FFIR', 4 + wave_data.size + 8 + data_data.size + 8, 'EVAW'};

			uint32_t wave_offset = sizeof(RiffChunk);
			uint32_t data_offset = wave_offset + sizeof(WaveChunk);
			uint32_t audio_data_offset = data_offset + sizeof(DataChunk) - 1;

			const uint64_t data_size =
				sizeof(RiffChunk) + sizeof(WaveChunk) + sizeof(DataChunk) + sizeof(audio_data) - 1;

			std::array<std::byte, data_size> audio_file;
			std::memcpy(audio_file._Elems, &riff_data, sizeof(RiffChunk));
			std::memcpy(audio_file._Elems + wave_offset, &wave_data, sizeof(WaveChunk));
			std::memcpy(audio_file._Elems + data_offset, &data_data, sizeof(DataChunk));
			std::memcpy(audio_file._Elems + audio_data_offset, &audio_data, sizeof(audio_data));

			WHEN("the wave file is loaded")
			{
				auto resource =
					loader.load(png_res_id.c_str(), store_id.c_str(), audio_file.data(), data_size);

				THEN("the resouce is correctly created")
				{
					REQUIRE(resource != nullptr);
					REQUIRE(resource->id() == png_res_id);
					REQUIRE(resource->store_id() == store_id);
					REQUIRE(resource->size() == data_size);
					REQUIRE(resource->type() == ResourceType::Audio);

					auto audio_res = std::dynamic_pointer_cast<AudioResource>(resource);
					REQUIRE(audio_res != nullptr);
					REQUIRE(audio_res->bits_per_sample() == wave_data.bits_per_sample);
					REQUIRE(audio_res->channels() == wave_data.channels);
					REQUIRE(audio_res->sample_rate() == wave_data.sample_rate);
					REQUIRE(audio_res->audio_data_size() == sizeof(audio_data));

					bool audio_data_matches = true;
					for (int i = 0; i < audio_res->audio_data_size(); ++i)
					{
						if (audio_data[i] != audio_res->audio_data()[i])
						{
							audio_data_matches = false;
							break;
						}
					}

					REQUIRE(audio_data_matches);
				}
			}
		}
	}
}
