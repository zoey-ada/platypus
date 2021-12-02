#include "consoleFormat.hpp"

#include <iostream>

ConsoleFormat::ConsoleFormat(const ConsoleColor foreground, const ConsoleColor background)
{
	this->setForegroundColor(foreground);
	this->setBackgroundColor(background);
}

ConsoleFormat::~ConsoleFormat()
{
	this->resetColors();
}

void ConsoleFormat::setForegroundColor(const ConsoleColor color) const
{
	switch (color)
	{
	case ConsoleColor::Black:
		std::cout << "\x1B[30m";
		break;
	case ConsoleColor::Red:
		std::cout << "\x1B[31m";
		break;
	case ConsoleColor::Green:
		std::cout << "\x1B[32m";
		break;
	case ConsoleColor::Yellow:
		std::cout << "\x1B[33m";
		break;
	case ConsoleColor::Blue:
		std::cout << "\x1B[34m";
		break;
	case ConsoleColor::Magenta:
		std::cout << "\x1B[35m";
		break;
	case ConsoleColor::Cyan:
		std::cout << "\x1B[36m";
		break;
	case ConsoleColor::White:
		std::cout << "\x1B[37m";
		break;
	case ConsoleColor::BrightBlack:
		std::cout << "\x1B[90m";
		break;
	case ConsoleColor::BrightRed:
		std::cout << "\x1B[91m";
		break;
	case ConsoleColor::BrightGreen:
		std::cout << "\x1B[92m";
		break;
	case ConsoleColor::BrightYellow:
		std::cout << "\x1B[93m";
		break;
	case ConsoleColor::BrightBlue:
		std::cout << "\x1B[94m";
		break;
	case ConsoleColor::BrightMagenta:
		std::cout << "\x1B[95m";
		break;
	case ConsoleColor::BrightCyan:
		std::cout << "\x1B[96m";
		break;
	case ConsoleColor::BrightWhite:
		std::cout << "\x1B[97m";
		break;
	}
}

void ConsoleFormat::setBackgroundColor(const ConsoleColor color) const
{
	switch (color)
	{
	case ConsoleColor::Black:
		std::cout << "\x1B[40m";
		break;
	case ConsoleColor::Red:
		std::cout << "\x1B[41m";
		break;
	case ConsoleColor::Green:
		std::cout << "\x1B[42m";
		break;
	case ConsoleColor::Yellow:
		std::cout << "\x1B[43m";
		break;
	case ConsoleColor::Blue:
		std::cout << "\x1B[44m";
		break;
	case ConsoleColor::Magenta:
		std::cout << "\x1B[45m";
		break;
	case ConsoleColor::Cyan:
		std::cout << "\x1B[46m";
		break;
	case ConsoleColor::White:
		std::cout << "\x1B[47m";
		break;
	case ConsoleColor::BrightBlack:
		std::cout << "\x1B[100m";
		break;
	case ConsoleColor::BrightRed:
		std::cout << "\x1B[101m";
		break;
	case ConsoleColor::BrightGreen:
		std::cout << "\x1B[102m";
		break;
	case ConsoleColor::BrightYellow:
		std::cout << "\x1B[103m";
		break;
	case ConsoleColor::BrightBlue:
		std::cout << "\x1B[104m";
		break;
	case ConsoleColor::BrightMagenta:
		std::cout << "\x1B[105m";
		break;
	case ConsoleColor::BrightCyan:
		std::cout << "\x1B[106m";
		break;
	case ConsoleColor::BrightWhite:
		std::cout << "\x1B[107m";
		break;
	}
}

void ConsoleFormat::resetColors() const
{
	std::cout << "\x1B[39;49m";
}
