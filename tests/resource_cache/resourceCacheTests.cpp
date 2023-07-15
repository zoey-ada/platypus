#include <catch2/catch.hpp>

#include <platypus_proto/settings.hpp>

#include <resource_cache/resourceCache.hpp>
#include <tests/resource_cache/loaders/mockResourceLoader.hpp>
#include <tests/resource_cache/resources/mockResource.hpp>
#include <tests/resource_cache/stores/mockResourceStore.hpp>
#include <tests/utilities/logging/mockLoggingSystem.hpp>

using namespace platypus;

SCENARIO("resource cache can load a resource", "[res_cache]")
{
	auto logging = std::make_shared<MockLoggingSystem>();
	auto store = std::make_shared<MockResourceStore>();
	auto loader = std::make_shared<MockResourceLoader>();
	auto resource = std::make_shared<MockResource>();

	ResourceCache cache(1, logging);

	GIVEN("no loaders are configured")
	{
		WHEN("attempting to get a resource")
		{
			auto res = cache.getResource(ResourceType::General, "test.zip/res.txt");

			THEN("no resource should be found")
			{
				REQUIRE(res == nullptr);
			}

			AND_THEN("a warning should be logged")
			{
				REQUIRE(logging->_warning_logs.size() == 1);
			}
		}
	}

	GIVEN("loader exists, but has wrong type")
	{
		loader->_resource_type = ResourceType::Audio;
		cache.registerLoader(loader);

		WHEN("attempting to get a resource")
		{
			auto res = cache.getResource(ResourceType::General, "test.zip/res.txt");

			THEN("no resource should be found")
			{
				REQUIRE(res == nullptr);
			}

			AND_THEN("a warning should be logged")
			{
				REQUIRE(logging->_warning_logs.size() == 1);
			}
		}
	}

	GIVEN("a loader is correctly configured")
	{
		loader->_resource_type = ResourceType::General;
		cache.registerLoader(loader);

		AND_GIVEN("no stores are configured")
		{
			REQUIRE(cache.initialize({}));

			WHEN("attempting to get a resource")
			{
				auto res = cache.getResource(ResourceType::General, "test.zip/res.txt");

				THEN("no resource should be found")
				{
					REQUIRE(res == nullptr);
				}

				AND_THEN("a warning should be logged")
				{
					REQUIRE(logging->_warning_logs.size() == 1);
				}
			}
		}

		AND_GIVEN("store exists, but has wrong identifier")
		{
			store->_identifier = "bad.zip";
			REQUIRE(cache.initialize({store}));

			WHEN("attempting to get a resource")
			{
				auto res = cache.getResource(ResourceType::General, "test.zip/res.txt");

				THEN("no resource should be found")
				{
					REQUIRE(res == nullptr);
				}

				AND_THEN("a warning should be logged")
				{
					REQUIRE(logging->_warning_logs.size() == 1);
				}
			}
		}

		AND_GIVEN("store exists, but cannot be opened")
		{
			store->_identifier = "test.zip";
			store->_can_open = false;
			REQUIRE(cache.initialize({store}));

			WHEN("asked to get resource")
			{
				auto res = cache.getResource(ResourceType::General, "test.zip/res.txt");

				THEN("no resource should be found")
				{
					REQUIRE(res == nullptr);
				}

				AND_THEN("a warning should be logged")
				{
					REQUIRE(logging->_warning_logs.size() == 1);
				}
			}
		}

		AND_GIVEN("a store is correctly configured")
		{
			store->_identifier = "test.zip";
			store->_can_open = true;
			REQUIRE(cache.initialize({store}));

			AND_GIVEN("the resource doesn't exist in the store")
			{
				store->_can_get_resource = false;

				WHEN("asked to get resource")
				{
					auto res = cache.getResource(ResourceType::General, "test.zip/res.txt");

					THEN("no resource should be found")
					{
						REQUIRE(res == nullptr);
					}

					AND_THEN("a warning should be logged")
					{
						REQUIRE(logging->_warning_logs.size() == 1);
					}
				}
			}

			AND_GIVEN("the resource does exist in the store")
			{
				store->_can_get_resource = true;

				AND_GIVEN("the resource cannot be loaded")
				{
					loader->_resource = nullptr;

					WHEN("asked to get resource")
					{
						auto res = cache.getResource(ResourceType::General, "test.zip/res.txt");

						THEN("no resource should be found")
						{
							REQUIRE(res == nullptr);
						}

						AND_THEN("a warning should be logged")
						{
							REQUIRE(logging->_warning_logs.size() == 1);
						}
					}
				}

				AND_GIVEN("the resource can be loaded")
				{
					loader->_resource = resource;

					WHEN("asked to get resource")
					{
						auto res = cache.getResource(ResourceType::General, "test.zip/res.txt");

						THEN("resource is loaded as expected")
						{
							REQUIRE(res != nullptr);
							REQUIRE(res == resource);
						}
					}
				}
			}
		}
	}
}

SCENARIO("resource cache correctly tells if resource is loaded", "[res_cache]")
{
	auto logging = std::make_shared<MockLoggingSystem>();
	auto store = std::make_shared<MockResourceStore>();
	auto loader = std::make_shared<MockResourceLoader>();
	auto resource = std::make_shared<MockResource>();

	ResourceCache cache(1, logging);

	GIVEN(
		"a resource loader is correctly configured, "
		"a resource store is correctly configured, and "
		"a resource exists that can be loaded")
	{
		loader->_resource_type = ResourceType::General;
		cache.registerLoader(loader);

		store->_identifier = "test.zip";
		store->_can_open = true;
		store->_can_get_resource = true;
		REQUIRE(cache.initialize({store}));

		loader->_resource = resource;

		AND_GIVEN("the resource has not yet been loaded")
		{
			WHEN("checking if the resource exists")
			{
				const bool exists = cache.exists(ResourceType::General, "test.zip/res.txt");

				THEN("the resource does not yet exist in the cache")
				{
					REQUIRE_FALSE(exists);
				}
			}
		}

		AND_GIVEN("the resource has been loaded")
		{
			cache.getResource(ResourceType::General, "test.zip/res.txt");

			WHEN("checking if the resource exists")
			{
				const bool exists = cache.exists(ResourceType::General, "test.zip/res.txt");

				THEN("the resource does exist in the cache")
				{
					REQUIRE(exists);
				}
			}

			WHEN("checking if a different resource exists")
			{
				const bool exists = cache.exists(ResourceType::General, "test.zip/file.txt");

				THEN("the resource does not yet exist in the cache")
				{
					REQUIRE_FALSE(exists);
				}
			}
		}
	}
}

SCENARIO("resource cache can share an already loaded resource", "[res_cache]")
{
	auto logging = std::make_shared<MockLoggingSystem>();
	auto store = std::make_shared<MockResourceStore>();
	auto loader = std::make_shared<MockResourceLoader>();
	auto resource = std::make_shared<MockResource>();

	ResourceCache cache(1, logging);

	GIVEN(
		"a resource loader is correctly configured, "
		"a resource store is correctly configured, and "
		"a resource exists that has be loaded")
	{
		loader->_resource_type = ResourceType::General;
		cache.registerLoader(loader);

		store->_identifier = "test.zip";
		store->_can_open = true;
		store->_can_get_resource = true;
		REQUIRE(cache.initialize({store}));

		loader->_resource = resource;
		auto res1 = cache.getResource(ResourceType::General, "test.zip/res.txt");
		REQUIRE(res1 == resource);

		WHEN("trying to load the resource a second time")
		{
			auto res2 = cache.getResource(ResourceType::General, "test.zip/res.txt");

			THEN("resource is shared as expected")
			{
				REQUIRE(res2 != nullptr);
				REQUIRE(res2 == resource);
				REQUIRE(res1 == resource);
			}
		}
	}
}

SCENARIO("resource cache can add a resource", "[res_cache]")
{
	auto logging = std::make_shared<MockLoggingSystem>();

	ResourceCache cache(1, logging);

	GIVEN("a resource exists")
	{
		auto resource = std::make_shared<MockResource>();
		resource->setResourceId("test.zip/res.txt");
		resource->setStoreId("test.zip");
		resource->_resource_type = ResourceType::General;

		WHEN("adding a resource to the cache")
		{
			cache.addResource(resource);

			THEN("the resource does exist in the cache")
			{
				const bool exists = cache.exists(ResourceType::General, "test.zip/res.txt");
				REQUIRE(exists);
			}
		}
	}
}
