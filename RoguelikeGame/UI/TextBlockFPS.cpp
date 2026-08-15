#include "TextBlockFPS.h"

using namespace Craft;

TextBlockFPS::TextBlockFPS(const Vector2Int& positionLT, int widgetWidth)
	:super(positionLT, widgetWidth)
{

}

void TextBlockFPS::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (!IsActive())
	{
		return;
	}

	FormatFPSString(deltaTime);
}

void TextBlockFPS::FormatFPSString(const float deltaTime)
{
	wchar_t szFormat[256] = { 0 };
	const float fps = 1.f / deltaTime;
	swprintf_s(szFormat, L"FPS : %.2f", fps);
	SetTextValue(szFormat);
}
