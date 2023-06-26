#include <catch2/catch.hpp>

#include <events/eventManager.hpp>
#include <tests/utilities/logging/mockLoggingSystem.hpp>

#include "mockEvent.hpp"

SCENARIO("event manager can trigger singular events", "[event_manager]")
{
	auto logging = std::make_shared<MockLoggingSystem>();
	EventManager event_manager(logging);

	GIVEN("an event sink is registered")
	{
		bool delegate_was_called = false;
		const EventDelegate delegate = [&delegate_was_called](
										   std::shared_ptr<IEvent>) { delegate_was_called = true; };
		const std::string event_type = "mock-event";
		event_manager._registerEventSink(event_type, delegate);

		WHEN("a monitored event is triggered")
		{
			const auto event = std::make_shared<MockEvent>();
			event_manager.triggerEvent(event);

			THEN("The delegate should be called")
			{
				REQUIRE(delegate_was_called == true);
			}
		}

		WHEN("a non-monitored event is triggered")
		{
			const auto event = std::make_shared<OtherMockEvent>();
			event_manager.triggerEvent(event);

			THEN("The delegate should not be called")
			{
				REQUIRE(delegate_was_called == false);
			}
		}
	}
}

SCENARIO("event manager can remove an event sink", "[event_manager]")
{
	auto logging = std::make_shared<MockLoggingSystem>();
	EventManager event_manager(logging);

	GIVEN("an event sink is registered")
	{
		bool delegate_was_called = false;
		const EventDelegate delegate = [&](std::shared_ptr<IEvent>) { delegate_was_called = true; };
		const std::string event_type = "mock-event";
		auto delegate_id = event_manager._registerEventSink(event_type, delegate);

		AND_GIVEN("the event sink is removed")
		{
			event_manager.removeEventSink(event_type, delegate_id);

			WHEN("triggering an event")
			{
				auto event = std::make_shared<MockEvent>();
				event_manager.triggerEvent(event);

				THEN("The delegate should not have be called")
				{
					REQUIRE(delegate_was_called == false);
				}
			}
		}
	}
}

SCENARIO("event manager removes the correct event sink", "[event_manager]")
{
	auto logging = std::make_shared<MockLoggingSystem>();
	EventManager event_manager(logging);

	GIVEN("multiple delegates are registered")
	{
		const std::string event_type = "mock-event";

		bool delegate1_was_called = false;
		const EventDelegate delegate1 = [&](std::shared_ptr<IEvent>) {
			delegate1_was_called = true;
		};
		auto delegate1_id = event_manager._registerEventSink(event_type, delegate1);

		bool delegate2_was_called = false;
		const EventDelegate delegate2 = [&](std::shared_ptr<IEvent>) {
			delegate2_was_called = true;
		};
		auto delegate2_id = event_manager._registerEventSink(event_type, delegate2);

		bool delegate3_was_called = false;
		const EventDelegate delegate3 = [&](std::shared_ptr<IEvent>) {
			delegate3_was_called = true;
		};
		auto delegate3_id = event_manager._registerEventSink(event_type, delegate3);

		bool delegate4_was_called = false;
		const EventDelegate delegate4 = [&](std::shared_ptr<IEvent>) {
			delegate4_was_called = true;
		};
		auto delegate4_id = event_manager._registerEventSink(event_type, delegate4);

		bool delegate5_was_called = false;
		const EventDelegate delegate5 = [&](std::shared_ptr<IEvent>) {
			delegate5_was_called = true;
		};
		auto delegate5_id = event_manager._registerEventSink(event_type, delegate5);

		AND_GIVEN("the first delegate is removed")
		{
			event_manager.removeEventSink(event_type, delegate1_id);

			WHEN("triggering an event")
			{
				auto event = std::make_shared<MockEvent>();
				event_manager.triggerEvent(event);

				THEN("The delegate 1 should not be called")
				{
					REQUIRE(delegate1_was_called == false);
					REQUIRE(delegate2_was_called == true);
					REQUIRE(delegate3_was_called == true);
					REQUIRE(delegate4_was_called == true);
					REQUIRE(delegate5_was_called == true);

					AND_GIVEN("the last delegate is removed")
					{
						event_manager.removeEventSink(event_type, delegate5_id);

						WHEN("triggering an event")
						{
							delegate1_was_called = false;
							delegate2_was_called = false;
							delegate3_was_called = false;
							delegate4_was_called = false;
							delegate5_was_called = false;

							event_manager.triggerEvent(event);

							THEN("The delegates 1 and 5 should not be called")
							{
								REQUIRE(delegate1_was_called == false);
								REQUIRE(delegate2_was_called == true);
								REQUIRE(delegate3_was_called == true);
								REQUIRE(delegate4_was_called == true);
								REQUIRE(delegate5_was_called == false);

								AND_GIVEN("the middle delegate is removed")
								{
									event_manager.removeEventSink(event_type, delegate3_id);

									WHEN("triggering an event")
									{
										delegate1_was_called = false;
										delegate2_was_called = false;
										delegate3_was_called = false;
										delegate4_was_called = false;
										delegate5_was_called = false;

										event_manager.triggerEvent(event);

										THEN("The delegates 1, 3, and 5 should not be called")
										{
											REQUIRE(delegate1_was_called == false);
											REQUIRE(delegate2_was_called == true);
											REQUIRE(delegate3_was_called == false);
											REQUIRE(delegate4_was_called == true);
											REQUIRE(delegate5_was_called == false);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

SCENARIO("event manager can handle events in delegates", "[event_manager]")
{
	auto logging = std::make_shared<MockLoggingSystem>();
	EventManager event_manager(logging);

	GIVEN("a delegate that triggers an event is registered")
	{
		bool delegate1_was_called = false;
		auto event1 = std::make_shared<OtherMockEvent>();
		const EventDelegate delegate1 = [&](std::shared_ptr<IEvent>) {
			delegate1_was_called = true;
			event_manager.triggerEvent(event1);
		};

		const std::string event_type1 = "mock-event";
		event_manager._registerEventSink(event_type1, delegate1);

		AND_GIVEN("a delegate for the secondary event is registered")
		{
			bool delegate2_was_called = false;
			const EventDelegate delegate2 = [&](std::shared_ptr<IEvent>) {
				delegate2_was_called = true;
			};

			const std::string event_type2 = "other-mock-event";
			event_manager._registerEventSink(event_type2, delegate2);

			WHEN("an event is triggered")
			{
				const auto event2 = std::make_shared<MockEvent>();
				event_manager.triggerEvent(event2);

				THEN("the first delegate should be called")
				{
					REQUIRE(delegate1_was_called == true);

					AND_THEN("the second delegate should not be called")
					{
						REQUIRE(delegate2_was_called == false);

						AND_WHEN("the event manager runs an update cycle")
						{
							event_manager.update();

							THEN("the second delegate should be called")
							{
								REQUIRE(delegate2_was_called == true);
							}
						}
					}
				}
			}
		}
	}
}

SCENARIO("event manager can handle delegate removal in delegates", "[event_manager]")
{
	auto logging = std::make_shared<MockLoggingSystem>();
	EventManager event_manager(logging);

	GIVEN("a delegate that unregisters itself is registered")
	{
		bool delegate_was_called = false;
		DelegateId id = InvalidDelegateId;
		const std::string event_type = "mock-event";
		const EventDelegate delegate = [&](std::shared_ptr<IEvent>) {
			delegate_was_called = true;
			event_manager.removeEventSink(event_type, id);
		};

		id = event_manager._registerEventSink(event_type, delegate);

		WHEN("an event is triggered")
		{
			const auto event = std::make_shared<MockEvent>();
			event_manager.triggerEvent(event);

			THEN("the delegate should be called")
			{
				REQUIRE(delegate_was_called == true);

				AND_WHEN("an event is triggered")
				{
					delegate_was_called = false;
					event_manager.triggerEvent(event);

					THEN("the delegate should be called")
					{
						REQUIRE(delegate_was_called == true);
					}
				}

				AND_WHEN("the event manager runs an update cycle")
				{
					event_manager.update();

					AND_WHEN("an event is triggered")
					{
						delegate_was_called = false;
						event_manager.triggerEvent(event);

						THEN("the delegate should not be called")
						{
							REQUIRE(delegate_was_called == false);
						}
					}
				}
			}
		}
	}
}
