#pragma once

enum class ConsoleColor
{
	Black,
	Red,
	Green,
	Yellow,
	Blue,
	Magenta,
	Cyan,
	White,
	BrightBlack,
	BrightRed,
	BrightGreen,
	BrightYellow,
	BrightBlue,
	BrightMagenta,
	BrightCyan,
	BrightWhite
};

class ConsoleFormat
{
public:
	explicit ConsoleFormat(const ConsoleColor foreground, const ConsoleColor background = ConsoleColor::Black);
	virtual ~ConsoleFormat();

private:
	void setForegroundColor(const ConsoleColor color) const;
	void setBackgroundColor(const ConsoleColor color) const;
	void resetColors() const;
};
