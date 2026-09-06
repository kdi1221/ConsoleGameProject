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

		currentValueBuffer.assign(drawCurrentValueNum, L'█');
		maxValueBuffer.assign(drawMaxValueNum, L'░');

		currentValueDrawPos = GetPosition();
		maxValueDrawPos = currentValueDrawPos + Vector2Int(drawCurrentValueNum, 0);
	}

	void ProgressBar::Draw()
	{
		Renderer& renderer = Renderer::Get();
		const int sortingOrder = GetRenderSortingOrder();

		const Vector2Int& drawPos = GetPosition();

		renderer.SubmitUI(currentValueBuffer, currentValueDrawPos, foregroundColor, sortingOrder);
		renderer.SubmitUI(maxValueBuffer, maxValueDrawPos, backgroundColor, sortingOrder);


		/*int drawXPos = drawPos.x;
		for (int i = 0; i < drawCurrentValueNum; ++i, ++drawXPos)
		{
			renderer.SubmitUI(L"█", Vector2Int(drawXPos, drawPos.y), foregroundColor, sortingOrder);
		}

		for (int i = 0; i < drawMaxValueNum; ++i, ++drawXPos)
		{
			renderer.SubmitUI(L"░", Vector2Int(drawXPos, drawPos.y), backgroundColor, sortingOrder);
		}*/
	}
}