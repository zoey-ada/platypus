#pragma once

#include <platypus.hpp>
#include <tests/mockGameLogic.hpp>

class MockEngine: public Platypus
{
public:
	explicit MockEngine(
		std::shared_ptr<BaseGameLogic> game_logic = std::make_shared<MockGameLogic>())
		: Platypus("test"), _game_logic(std::move(game_logic))
	{}

	MockEngine(std::shared_ptr<IClock> clock, std::shared_ptr<BaseGameLogic> game_logic)
		: Platypus("test", std::move(clock)), _game_logic(std::move(game_logic))
	{}

	virtual ~MockEngine() = default;

	bool startup() { return Platypus::initialize(); }
	int run() { return Platypus::run(); }
	void shutdown() { return Platypus::deinitialize(); }

	std::shared_ptr<BaseGameLogic> createLogicAndView() override { return this->_game_logic; }

	std::shared_ptr<BaseGameLogic> _game_logic;
};
