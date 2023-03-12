#include <catch2/catch.hpp>

#include <resource_cache/loaders/pixelShaderLoader.hpp>
#include <resource_cache/resources/pixelShaderResource.hpp>
#include <serviceProvider.hpp>
#include <tests/renderer/mockRenderer.hpp>
#include <tests/renderer/mockShaderManager.hpp>
#include <tests/resource_cache/mockResourceCache.hpp>
#include <tests/utilities/logging/mockLoggingSystem.hpp>

SCENARIO("pixel shader loader can load a pixel shader", "[res_cache]")
{
	auto logging = std::make_shared<MockLoggingSystem>();
	auto resource_cache = std::make_shared<MockResourceCache>();
	auto renderer = std::make_shared<MockRenderer>();
	ServiceProvider::registerRenderer(renderer);

	auto shader_manager = std::make_shared<MockShaderManager>();
	renderer->_shader_manager = shader_manager;

	std::string store_id("assets.zip");
	uint64_t data_size = 0;
	std::byte res_data[] {std::byte {0xff}};
	std::string uncompiled_res_id("shader.hlsl");
	std::string compiled_res_id("shader.cso");

	GIVEN("a pixel shader loader with no resource cache")
	{
		PixelShaderLoader loader(nullptr, renderer, logging);

		WHEN("a precompiled shader is loaded")
		{
			auto resource =
				loader.load(uncompiled_res_id.c_str(), store_id.c_str(), res_data, data_size);

			THEN("the resouce fails to be created")
			{
				REQUIRE(resource == nullptr);
				REQUIRE(logging->_warning_logs.size() == 1);
			}
		}
	}

	GIVEN("a pixel shader loader with no renderer")
	{
		PixelShaderLoader loader(resource_cache, nullptr, logging);

		WHEN("a precompiled shader is loaded")
		{
			auto resource =
				loader.load(uncompiled_res_id.c_str(), store_id.c_str(), res_data, data_size);

			THEN("the resouce fails to be created")
			{
				REQUIRE(resource == nullptr);
				REQUIRE(logging->_warning_logs.size() == 1);
			}
		}
	}

	GIVEN("a pixel shader loader with a resource cache and renderer")
	{
		PixelShaderLoader loader(resource_cache, renderer, logging);

		WHEN("invalid resource data is used to load")
		{
			auto resource =
				loader.load(compiled_res_id.c_str(), store_id.c_str(), nullptr, data_size);

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

		AND_GIVEN("no shader can be created")
		{
			shader_manager->_pixel_shader = nullptr;

			WHEN("a precompiled shader is loaded")
			{
				auto resource =
					loader.load(compiled_res_id.c_str(), store_id.c_str(), res_data, data_size);

				THEN("the resouce fails to be created")
				{
					REQUIRE(resource == nullptr);
					REQUIRE(logging->_warning_logs.size() == 1);
				}
			}
		}

		AND_GIVEN("a precompiled pixel shader can be created")
		{
			std::string mock_pixel_shader("pixel_shader");
			shader_manager->_pixel_shader = (PtPixelShader)&mock_pixel_shader;

			WHEN("an uncompiled shader is loaded")
			{
				auto resource =
					loader.load(uncompiled_res_id.c_str(), store_id.c_str(), res_data, data_size);

				THEN("the resouce fails to be created")
				{
					REQUIRE(resource == nullptr);
					REQUIRE(logging->_warning_logs.size() == 1);
				}
			}

			WHEN("a precompiled shader is loaded")
			{
				auto resource =
					loader.load(compiled_res_id.c_str(), store_id.c_str(), res_data, data_size);

				THEN("the resouce is correctly created")
				{
					REQUIRE(resource != nullptr);
					REQUIRE(resource->id() == compiled_res_id);
					REQUIRE(resource->store_id() == store_id);
					REQUIRE(resource->size() == data_size);
					REQUIRE(resource->type() == ResourceType::PixelShader);

					auto pixel_shader_res =
						std::dynamic_pointer_cast<PixelShaderResource>(resource);
					REQUIRE(pixel_shader_res != nullptr);
					REQUIRE(pixel_shader_res->getShader() == (PtPixelShader)&mock_pixel_shader);
				}
			}
		}

		AND_GIVEN("an uncompiled pixel shader can be created")
		{
			std::string mock_pixel_shader("pixel_shader");
			shader_manager->_compiled_pixel_shader = (PtPixelShader)&mock_pixel_shader;

			WHEN("a precompiled shader is loaded")
			{
				auto resource =
					loader.load(compiled_res_id.c_str(), store_id.c_str(), res_data, data_size);

				THEN("the resouce fails to be created")
				{
					REQUIRE(resource == nullptr);
					REQUIRE(logging->_warning_logs.size() == 1);
				}
			}

			WHEN("an uncompiled shader is loaded")
			{
				auto resource =
					loader.load(uncompiled_res_id.c_str(), store_id.c_str(), res_data, data_size);

				THEN("the resouce is correctly created")
				{
					REQUIRE(resource != nullptr);
					REQUIRE(resource->id() == uncompiled_res_id);
					REQUIRE(resource->store_id() == store_id);
					REQUIRE(resource->size() == data_size);
					REQUIRE(resource->type() == ResourceType::PixelShader);

					auto pixel_shader_res =
						std::dynamic_pointer_cast<PixelShaderResource>(resource);
					REQUIRE(pixel_shader_res != nullptr);
					REQUIRE(pixel_shader_res->getShader() == (PtPixelShader)&mock_pixel_shader);
				}
			}
		}
	}
}
