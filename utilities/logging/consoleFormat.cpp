#include "consoleFormat.hpp"

#include <iostream>
#include <map>

ConsoleFormat::ConsoleFormat(const ConsoleColor foreground, const ConsoleColor background)
{
	this->loadColorMaps();
	this->setForegroundColor(foreground);
	this->setBackgroundColor(background);
}

ConsoleFormat::~ConsoleFormat()
{
	try
	{
		this->resetColors();
	}
	catch(std::exception&)
	{
		// unable to reset colors
	}
}

void ConsoleFormat::loadColorMaps()
{
	_foreground_color_map[ConsoleColor::Black] = "\x1B[30m";
	_foreground_color_map[ConsoleColor::Red] = "\x1B[31m";
	_foreground_color_map[ConsoleColor::Green] = "\x1B[32m";
	_foreground_color_map[ConsoleColor::Yellow] = "\x1B[33m";
	_foreground_color_map[ConsoleColor::Blue] = "\x1B[34m";
	_foreground_color_map[ConsoleColor::Magenta] = "\x1B[35m";
	_foreground_color_map[ConsoleColor::Cyan] = "\x1B[36m";
	_foreground_color_map[ConsoleColor::White] = "\x1B[37m";
	_foreground_color_map[ConsoleColor::BrightBlack] = "\x1B[90m";
	_foreground_color_map[ConsoleColor::BrightRed] = "\x1B[91m";
	_foreground_color_map[ConsoleColor::BrightGreen] = "\x1B[92m";
	_foreground_color_map[ConsoleColor::BrightYellow] = "\x1B[93m";
	_foreground_color_map[ConsoleColor::BrightBlue] = "\x1B[94m";
	_foreground_color_map[ConsoleColor::BrightMagenta] = "\x1B[95m";
	_foreground_color_map[ConsoleColor::BrightCyan] = "\x1B[96m";
	_foreground_color_map[ConsoleColor::BrightWhite] = "\x1B[97m";

	_background_color_map[ConsoleColor::Black] = "\x1B[40m";
	_background_color_map[ConsoleColor::Red] = "\x1B[41m";
	_background_color_map[ConsoleColor::Green] = "\x1B[42m";
	_background_color_map[ConsoleColor::Yellow] = "\x1B[43m";
	_background_color_map[ConsoleColor::Blue] = "\x1B[44m";
	_background_color_map[ConsoleColor::Magenta] = "\x1B[45m";
	_background_color_map[ConsoleColor::Cyan] = "\x1B[46m";
	_background_color_map[ConsoleColor::White] = "\x1B[47m";
	_background_color_map[ConsoleColor::BrightBlack] = "\x1B[100m";
	_background_color_map[ConsoleColor::BrightRed] = "\x1B[101m";
	_background_color_map[ConsoleColor::BrightGreen] = "\x1B[102m";
	_background_color_map[ConsoleColor::BrightYellow] = "\x1B[103m";
	_background_color_map[ConsoleColor::BrightBlue] = "\x1B[104m";
	_background_color_map[ConsoleColor::BrightMagenta] = "\x1B[105m";
	_background_color_map[ConsoleColor::BrightCyan] = "\x1B[106m";
	_background_color_map[ConsoleColor::BrightWhite] = "\x1B[107m";
}

void ConsoleFormat::setForegroundColor(const ConsoleColor color) const
{
	std::cout << this->_foreground_color_map.at(color);
}

void ConsoleFormat::setBackgroundColor(const ConsoleColor color) const
{
	std::cout << this->_background_color_map.at(color);
}

void ConsoleFormat::resetColors() const
{
	std::cout << "\x1B[0m";
}
