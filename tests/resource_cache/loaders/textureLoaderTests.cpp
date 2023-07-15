#include <catch2/catch.hpp>

#include <resource_cache/loaders/textureLoader.hpp>
#include <resource_cache/resources/textureResource.hpp>
#include <serviceProvider.hpp>
#include <tests/renderer/mockRenderer.hpp>
#include <tests/resource_cache/mockResourceCache.hpp>
#include <tests/utilities/logging/mockLoggingSystem.hpp>

using namespace platypus;

SCENARIO("texture loader can load a texture", "[res_cache]")
{
	auto logging = std::make_shared<MockLoggingSystem>();
	auto resource_cache = std::make_shared<MockResourceCache>();
	auto renderer = std::make_shared<MockRenderer>();
	ServiceProvider::registerRenderer(renderer);

	std::string store_id("assets.zip");
	uint64_t data_size = 0;
	std::byte res_data[] {std::byte {0xff}};
	std::string png_res_id("image.png");

	GIVEN("a texture loader with no resource cache")
	{
		TextureLoader loader(nullptr, renderer, logging);

		WHEN("a png image is loaded")
		{
			auto resource = loader.load(png_res_id.c_str(), store_id.c_str(), res_data, data_size);

			THEN("the resouce fails to be created")
			{
				REQUIRE(resource == nullptr);
				REQUIRE(logging->_warning_logs.size() == 1);
			}
		}
	}

	GIVEN("a texture loader with no renderer")
	{
		TextureLoader loader(resource_cache, nullptr, logging);

		WHEN("a png image is loaded")
		{
			auto resource = loader.load(png_res_id.c_str(), store_id.c_str(), res_data, data_size);

			THEN("the resouce fails to be created")
			{
				REQUIRE(resource == nullptr);
				REQUIRE(logging->_warning_logs.size() == 1);
			}
		}
	}

	GIVEN("a texture loader with a resource cache and renderer")
	{
		TextureLoader loader(resource_cache, renderer, logging);

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

		AND_GIVEN("a texture can not be created")
		{
			renderer->_texture = nullptr;

			WHEN("a png image is loaded")
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

		AND_GIVEN("a texture can be created")
		{
			std::string mock_texture("texture");
			renderer->_texture = (platypus::graphics::TextureResource)&mock_texture;

			AND_GIVEN("a sampler state can not be created")
			{
				renderer->_sampler_state = nullptr;

				WHEN("a png image is loaded")
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

			AND_GIVEN("a sampler state can be created")
			{
				std::string mock_sampler_state("sampler_state");
				renderer->_sampler_state = (platypus::graphics::SamplerState)&mock_sampler_state;

				WHEN("a texture is loaded")
				{
					auto resource =
						loader.load(png_res_id.c_str(), store_id.c_str(), res_data, data_size);

					THEN("the resouce is correctly created")
					{
						REQUIRE(resource != nullptr);
						REQUIRE(resource->id() == png_res_id);
						REQUIRE(resource->store_id() == store_id);
						REQUIRE(resource->size() == data_size);
						REQUIRE(resource->type() == ResourceType::Texture);

						auto texture_res = std::dynamic_pointer_cast<TextureResource>(resource);
						REQUIRE(texture_res != nullptr);
						REQUIRE(texture_res->getTexture() ==
							(platypus::graphics::TextureResource)&mock_texture);
						REQUIRE(texture_res->getSamplerState() ==
							(platypus::graphics::SamplerState)&mock_sampler_state);
					}
				}
			}
		}
	}
}
