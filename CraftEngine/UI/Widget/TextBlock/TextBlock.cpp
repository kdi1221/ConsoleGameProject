#include "TextBlock.h"
#include "Render/Renderer.h"

namespace Craft
{
	TextBlock::TextBlock(const Vector2Int& positionLT, int widgetWidth)
		:super(positionLT, widgetWidth, 1)
	{

	}

	void TextBlock::Draw()
	{
		//비활성화 상태라면 처리 안함.
		if (!IsActive())
		{
			return;
		}

		if (!textValue.empty())
		{
			Renderer::Get().SubmitUI(textValue, GetPosition(), GetDrawColor(), GetRenderSortingOrder());
		}
	}

	void TextBlock::SetTextValue(const std::wstring& text)
	{
		textValue = text;
	}

	void TextBlock::SetDrawColor(const Color newColor)
	{
		drawColor = newColor;
	}
}