#include "ProgressBar.h"
#include "Render/Renderer.h"

namespace Craft
{
	ProgressBar::ProgressBar(const Vector2Int& positionLT, 
							const int drawWidth, 
							const Color background, 
							const Color foreground)
		:super(positionLT, drawWidth, 1)
		,backgroundColor(background)
		,foregroundColor(foreground)
	{

	}

	void ProgressBar::SetValue(float inCurrentValue, float inMaxValue)
	{
		maxValue = inMaxValue;
		currentValue = min(inCurrentValue, maxValue);
		
		const int drawWidth = GetWidth();

		const float percentage = currentValue / maxValue;
		drawCurrentValueNum = static_cast<int>(static_cast<float>(drawWidth) * percentage);
		drawCurrentValueNum = min(max(0, drawCurrentValueNum), drawWidth);
		drawMaxValueNum = drawWidth - drawCurrentValueNum;
	}

	void ProgressBar::Draw()
	{
		const Vector2Int& drawPos = GetPosition();
		int drawXPos = drawPos.x;

		for (int i = 0; i < drawCurrentValueNum; ++i, ++drawXPos)
		{
			Renderer::Get().SubmitUI(L"█", Vector2Int(drawXPos, drawPos.y), foregroundColor);
		}

		for (int i = 0; i < drawMaxValueNum; ++i, ++drawXPos)
		{
			Renderer::Get().SubmitUI(L"░", Vector2Int(drawXPos, drawPos.y), backgroundColor);
		}
	}
}