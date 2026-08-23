#include "TextBlockMousePos.h"
#include <Core/Input.h>

using namespace Craft;

TextBlockMousePos::TextBlockMousePos(const Vector2Int& positionLT, int widgetWidth)
	:super(positionLT, widgetWidth)
{
}

void TextBlockMousePos::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (!IsActive())
	{
		return;
	}

	FormatMousePosString();
}

void TextBlockMousePos::FormatMousePosString()
{
	const Input& input = Input::Get();

	wchar_t szMousePosFormat[256] = { 0 };
	const Vector2Int& mousePos = input.GetMousePosition();
	swprintf_s(szMousePosFormat, L"MousePos: <x:%3d, y:%3d>", mousePos.x, mousePos.y);

	SetTextValue(szMousePosFormat);
}

	
