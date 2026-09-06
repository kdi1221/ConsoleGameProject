#include "BackgroundWidget.h"
#include "Render/Renderer.h"

#include <UI/Widget/Image/ImageWidget.h>

using namespace Craft;

BackgroundWidget::BackgroundWidget(const Vector2Int& positionLT, int widgetWidth, int widgetHeight, Craft::Color color)
	:super(positionLT, widgetWidth, widgetHeight)
	,drawColor(color)
{
	drawImageBuffer.resize(widgetHeight);
	for (int i = 0; i < widgetHeight; ++i)
	{
		drawImageBuffer[i].resize(widgetWidth);
	}

	drawImageBuffer[0][0] = L'┏';
	drawImageBuffer[0][widgetWidth - 2] = L'┓';
	drawImageBuffer[widgetHeight - 1][0] = L'┗';
	drawImageBuffer[widgetHeight - 1][widgetWidth - 2] = L'┛';

	/* 상하단 */
	for (int i = 1; i < widgetWidth - 3; ++i)
	{
		drawImageBuffer[0][i] = L'━';
		drawImageBuffer[widgetHeight - 1][i] = L'━';
	}

	/* 좌우 */
	for (int i = 1; i < widgetHeight - 1; ++i)
	{
		drawImageBuffer[i][0] = L'┃';
		drawImageBuffer[i][widgetWidth - 2] = L'┃';
	}
}

void BackgroundWidget::Draw()
{
	Renderer& renderer = Renderer::Get();
	const int sortingOrder = GetRenderSortingOrder();
	const Vector2Int& positionLT = GetPosition();

	int yPos = 0;
	for (const std::wstring& imageBuffer : drawImageBuffer)
	{
		renderer.SubmitUI(imageBuffer, Vector2Int(positionLT.x, positionLT.y + yPos), Color::Yellow, sortingOrder);
		++yPos;
	}
}
