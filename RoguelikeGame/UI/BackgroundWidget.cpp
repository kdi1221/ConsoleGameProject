#include "BackgroundWidget.h"
#include "Render/Renderer.h"

using namespace Craft;

BackgroundWidget::BackgroundWidget(const Vector2Int& positionLT, int widgetWidth, int widgetHeight, Craft::Color color)
	:super(positionLT, widgetWidth, widgetHeight)
	,drawColor(color)
{
}

void BackgroundWidget::Draw()
{
	const int drawWidth = GetWidth();
	const int drawHeight = GetHeight();
	const Vector2Int& positionLT = GetPosition();
	const int drawRight = positionLT.x + drawWidth - 2;
	const int drawBottom = positionLT.y + drawHeight;

	/* 뒷 배경 */
	for (int y = 1; y <= drawHeight - 1; ++y)
	{
		for (int x = 1; x < drawWidth-1; ++x)
		{
			Renderer::Get().SubmitUI(L" ", Vector2Int(positionLT.x + x, positionLT.y + y), Color::Black, GetRenderSortingOrder());
		}
	}

	/* 모서리 */
	Renderer::Get().SubmitUI(L"┏", Vector2Int(positionLT.x, positionLT.y), drawColor, GetRenderSortingOrder());
	Renderer::Get().SubmitUI(L"┓", Vector2Int(drawRight, positionLT.y), drawColor, GetRenderSortingOrder());
	Renderer::Get().SubmitUI(L"┗", Vector2Int(positionLT.x, drawBottom), drawColor, GetRenderSortingOrder());
	Renderer::Get().SubmitUI(L"┛", Vector2Int(drawRight, drawBottom), drawColor, GetRenderSortingOrder());

	/* 상하단*/
	for (int i = 1; i < drawWidth-2; ++i)
	{
		Renderer::Get().SubmitUI(L"━", Vector2Int(positionLT.x + i, positionLT.y), drawColor, GetRenderSortingOrder());
		Renderer::Get().SubmitUI(L"━", Vector2Int(positionLT.x + i, drawBottom), drawColor, GetRenderSortingOrder());
	}

	/* 좌우측 */
	for (int i = 1; i < drawHeight; ++i)
	{
		Renderer::Get().SubmitUI(L"┃", Vector2Int(positionLT.x, positionLT.y + i), drawColor, GetRenderSortingOrder());
		Renderer::Get().SubmitUI(L"┃", Vector2Int(drawRight, positionLT.y + i), drawColor, GetRenderSortingOrder());
	}
}
