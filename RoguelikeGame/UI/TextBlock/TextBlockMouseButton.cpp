#include "TextBlockMouseButton.h"
#include <Core/Input.h>

using namespace Craft;

TextBlockMouseButton::TextBlockMouseButton(const Vector2Int& positionLT, int widgetWidth)
	:super(positionLT, widgetWidth)
{

}

void TextBlockMouseButton::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (!IsActive())
	{
		return;
	}

	FormatMouseButton();
}

void TextBlockMouseButton::FormatMouseButton()
{
	const Input& input = Input::Get();

	std::wstring mouseButtonDown;
	if (input.GetKey(VK_LBUTTON))
	{
		if (!mouseButtonDown.empty())
		{
			mouseButtonDown += L"|";
		}
		mouseButtonDown += L"Left";
	}

	if (input.GetKey(VK_MBUTTON))
	{
		if (!mouseButtonDown.empty())
		{
			mouseButtonDown += L"|";
		}
		mouseButtonDown += L"Wheel";
	}

	if (input.GetKey(VK_RBUTTON))
	{
		if (!mouseButtonDown.empty())
		{
			mouseButtonDown += L"|";
		}
		mouseButtonDown += L"Right";
	}

	SetTextValue(L"MouseBtn: " + mouseButtonDown);
}