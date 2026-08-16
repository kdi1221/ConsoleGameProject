#include "TextBlockElapsedTime.h"
#include <sstream>
#include <iomanip>

TextBlockElapsedTime::TextBlockElapsedTime(const Craft::Vector2Int& positionLT)
	:super(positionLT, 15)
	,frequency()
	,startTime()
{
	QueryPerformanceFrequency(&frequency);
}

void TextBlockElapsedTime::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	FormatTimeString();
}

void TextBlockElapsedTime::SetStartTime(LARGE_INTEGER time)
{
	startTime = time;
	FormatTimeString();
}

void TextBlockElapsedTime::FormatTimeString()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime); 

	const double elapsedTime = static_cast<double>(currentTime.QuadPart - startTime.QuadPart) / static_cast<double>(frequency.QuadPart);

	unsigned int totalSeconds = static_cast<unsigned int>(elapsedTime);
	unsigned int hours = totalSeconds / 3600;
	unsigned int minutes = (totalSeconds % 3600) / 60;
	unsigned int seconds = totalSeconds % 60;

	std::wostringstream oss;
	oss << L"Time : "
		<< std::setw(2) << std::setfill(L'0') << hours << L":"
		<< std::setw(2) << std::setfill(L'0') << minutes << L":"
		<< std::setw(2) << std::setfill(L'0') << seconds;

	SetTextValue(oss.str()); 
}
