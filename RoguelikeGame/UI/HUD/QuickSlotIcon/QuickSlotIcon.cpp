#include "QuickSlotIcon.h"
#include <Render/Renderer.h>

const int QuickSlotIcon::SLOT_ICON_WIDTH = 10;
const int QuickSlotIcon::SLOT_ICON_HEIGHT = 5;

using namespace Craft;

QuickSlotIcon::QuickSlotIcon(const Craft::Vector2Int& positionLT)
	:super(positionLT, SLOT_ICON_WIDTH, SLOT_ICON_HEIGHT)
{
}

void QuickSlotIcon::Draw()
{
	super::Draw();

	Renderer& renderer = Renderer::Get();
	const int renderSortingOrder = GetRenderSortingOrder();

	const Vector2Int& positionLT = GetPosition();
	const int drawWidth = GetWidth();
	const int drawHeight = GetHeight();
	const int drawRight = positionLT.x + drawWidth - 1;
	const int drawBottom = positionLT.y + drawHeight - 1;

	/* 뒷 배경 */
	for (int y = 1; y < drawHeight - 1; ++y)
	{
		for (int x = 1; x < drawWidth - 1; ++x)
		{
			renderer.SubmitUI(L" ", Vector2Int(positionLT.x + x, positionLT.y + y), Color::Black, GetRenderSortingOrder());
		}
	}

	/* 모서리 */
	renderer.SubmitUI(L"┏", Vector2Int(positionLT.x, positionLT.y), outlineColor, GetRenderSortingOrder());
	renderer.SubmitUI(L"┓", Vector2Int(drawRight, positionLT.y), outlineColor, GetRenderSortingOrder());
	renderer.SubmitUI(L"┗", Vector2Int(positionLT.x, drawBottom), outlineColor, GetRenderSortingOrder());
	renderer.SubmitUI(L"┛", Vector2Int(drawRight, drawBottom), outlineColor, GetRenderSortingOrder());

	/* 상하단*/
	for (int i = positionLT.x + 1; i < drawRight; ++i)
	{
		renderer.SubmitUI(L"━", Vector2Int(i, positionLT.y), outlineColor, GetRenderSortingOrder());
		renderer.SubmitUI(L"━", Vector2Int(i, drawBottom), outlineColor, GetRenderSortingOrder());
	}

	/* 좌우측 */
	for (int i = positionLT.y + 1; i < drawBottom; ++i)
	{
		renderer.SubmitUI(L"┃", Vector2Int(positionLT.x, i), outlineColor, GetRenderSortingOrder());
		renderer.SubmitUI(L"┃", Vector2Int(drawRight, i), outlineColor, GetRenderSortingOrder());
	}

	//가운데 아이콘 표시
}
