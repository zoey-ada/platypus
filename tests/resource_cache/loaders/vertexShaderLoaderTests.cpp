#include <catch2/catch.hpp>

#include <resource_cache/loaders/vertexShaderLoader.hpp>
#include <resource_cache/resources/vertexShaderResource.hpp>
#include <serviceProvider.hpp>
#include <tests/renderer/mockRenderer.hpp>
#include <tests/renderer/mockShaderManager.hpp>
#include <tests/resource_cache/mockResourceCache.hpp>
#include <tests/utilities/logging/mockLoggingSystem.hpp>

using namespace platypus;

SCENARIO("vertex shader loader can load a vertex shader", "[res_cache]")
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

	GIVEN("a vertex shader loader with no resource cache")
	{
		VertexShaderLoader loader(nullptr, renderer, logging);

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

	GIVEN("a vertex shader loader with no renderer")
	{
		VertexShaderLoader loader(resource_cache, nullptr, logging);

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

	GIVEN("a vertex shader loader with a resource cache and renderer")
	{
		VertexShaderLoader loader(resource_cache, renderer, logging);

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
			shader_manager->_vertex_shader = nullptr;

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

		AND_GIVEN("a precompiled vertex shader can be created")
		{
			std::string mock_vertex_shader("vertex_shader");
			shader_manager->_vertex_shader = (PtVertexShader)&mock_vertex_shader;

			AND_GIVEN("no input layout can be created")
			{
				renderer->_input_layout = nullptr;

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

			AND_GIVEN("an input layout can be created")
			{
				std::string mock_input_layout("input_layout");
				renderer->_input_layout = (PtInputLayout)&mock_input_layout;

				WHEN("an uncompiled shader is loaded")
				{
					auto resource = loader.load(uncompiled_res_id.c_str(), store_id.c_str(),
						res_data, data_size);

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
						REQUIRE(resource->type() == ResourceType::VertexShader);

						auto vertex_shader_res =
							std::dynamic_pointer_cast<VertexShaderResource>(resource);
						REQUIRE(vertex_shader_res != nullptr);
						REQUIRE(vertex_shader_res->getInputLayout() ==
							(PtInputLayout)&mock_input_layout);
						REQUIRE(
							vertex_shader_res->getShader() == (PtVertexShader)&mock_vertex_shader);
					}
				}
			}
		}

		AND_GIVEN("an uncompiled vertex shader can be created")
		{
			std::string mock_vertex_shader("vertex_shader");
			shader_manager->_compiled_vertex_shader = (PtVertexShader)&mock_vertex_shader;

			AND_GIVEN("no input layout can be created")
			{
				renderer->_input_layout = nullptr;

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

			AND_GIVEN("an input layout can be created")
			{
				std::string mock_input_layout("input_layout");
				renderer->_input_layout = (PtInputLayout)&mock_input_layout;

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
					auto resource = loader.load(uncompiled_res_id.c_str(), store_id.c_str(),
						res_data, data_size);

					THEN("the resouce is correctly created")
					{
						REQUIRE(resource != nullptr);
						REQUIRE(resource->id() == uncompiled_res_id);
						REQUIRE(resource->store_id() == store_id);
						REQUIRE(resource->size() == data_size);
						REQUIRE(resource->type() == ResourceType::VertexShader);

						auto vertex_shader_res =
							std::dynamic_pointer_cast<VertexShaderResource>(resource);
						REQUIRE(vertex_shader_res != nullptr);
						REQUIRE(vertex_shader_res->getInputLayout() ==
							(PtInputLayout)&mock_input_layout);
						REQUIRE(
							vertex_shader_res->getShader() == (PtVertexShader)&mock_vertex_shader);
					}
				}
			}
		}
	}
}
