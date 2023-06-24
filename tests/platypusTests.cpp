#include <catch2/catch.hpp>

#include <platypus.hpp>
#include <tests/mockEngine.hpp>
#include <tests/mockGameLogic.hpp>
#include <tests/utilities/time/mockClock.hpp>

SCENARIO("engine can start up and shutdown", "[engine, slow]")
{
	GIVEN("the engine exists")
	{
		auto game_logic = std::make_shared<MockGameLogic>();
		auto clock = std::make_shared<MockClock>();
		MockEngine engine(clock, game_logic);

		WHEN("the engine starts up")
		{
			bool exception_encountered = false;
			bool startup_success = false;
			try
			{
				startup_success = engine.startup();
			}
			catch (const std::exception& e)
			{
				exception_encountered = true;
			}

			THEN("no exceptions occur")
			{
				REQUIRE(startup_success == true);
				REQUIRE(exception_encountered == false);

				AND_WHEN("the engine shuts down")
				{
					exception_encountered = false;
					try
					{
						engine.shutdown();
					}
					catch (const std::exception& e)
					{
						exception_encountered = true;
					}

					THEN("no exceptions occur")
					{
						REQUIRE(exception_encountered == false);
					}
				}
			}
		}
	}
}
