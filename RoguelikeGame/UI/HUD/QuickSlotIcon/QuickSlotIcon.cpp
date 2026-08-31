#include "QuickSlotIcon.h"
#include <Render/Renderer.h>

const int QuickSlotIcon::SLOT_ICON_WIDTH = 9;
const int QuickSlotIcon::SLOT_ICON_HEIGHT = 5;

using namespace Craft;

QuickSlotIcon::QuickSlotIcon(const Craft::Vector2Int& positionLT)
	:super(positionLT, SLOT_ICON_WIDTH, SLOT_ICON_HEIGHT)
{
	//아이콘 출력위치 결정
	iconImaeDrawPos = GetPosition() + Vector2Int(GetWidth() >> 1, GetHeight() >> 1);
}

void QuickSlotIcon::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	//TODO : 게임 일시정지가 아닌동안 쿨타임 타이머 돌려서 갱신
}

void QuickSlotIcon::Draw()
{
	super::Draw();

	Renderer& renderer = Renderer::Get();
	const int renderSortingOrder = GetRenderSortingOrder();

	const Vector2Int& positionLT = GetPosition();
	const int drawWidth = GetWidth();
	const int drawHeight = GetHeight();
	const int drawRight = positionLT.x + drawWidth - 2;
	const int drawBottom = positionLT.y + drawHeight - 1;

	/* 뒷 배경 */
	for (int y = 0; y < drawHeight; ++y)
	{
		for (int x = 0; x < drawWidth; ++x)
		{
			renderer.SubmitUI(L" ", Vector2Int(positionLT.x + x, positionLT.y + y), Color::Black, renderSortingOrder);
		}
	}

	const Color outlineColor = bCooldownState ? outlineColorCooldown : outlineColorBase;

	/* 모서리 */
	renderer.SubmitUI(L"┏", Vector2Int(positionLT.x, positionLT.y), outlineColor, renderSortingOrder);
	renderer.SubmitUI(L"┓", Vector2Int(drawRight, positionLT.y), outlineColor, renderSortingOrder);
	renderer.SubmitUI(L"┗", Vector2Int(positionLT.x, drawBottom), outlineColor, renderSortingOrder);
	renderer.SubmitUI(L"┛", Vector2Int(drawRight, drawBottom), outlineColor, renderSortingOrder);

	/* 상하단*/
	for (int i = positionLT.x + 1; i < drawRight; ++i)
	{
		renderer.SubmitUI(L"━", Vector2Int(i, positionLT.y), outlineColor, renderSortingOrder);
		renderer.SubmitUI(L"━", Vector2Int(i, drawBottom), outlineColor, renderSortingOrder);
	}

	/* 좌우측 */
	for (int i = positionLT.y + 1; i < drawBottom; ++i)
	{
		renderer.SubmitUI(L"┃", Vector2Int(positionLT.x, i), outlineColor, renderSortingOrder);
		renderer.SubmitUI(L"┃", Vector2Int(drawRight, i), outlineColor, renderSortingOrder);
	}

	//가운데 아이콘 표시
	const Color iconImageColor = bCooldownState ? iconImageColorCooldown : iconImageColorBase;
	renderer.SubmitUI(iconImage, iconImaeDrawPos, iconImageColor, renderSortingOrder);

	//TODO : 쿨타임 표시
}

void QuickSlotIcon::SetIconImage(const std::wstring& image)
{
	iconImage = image;
}

void QuickSlotIcon::SetIconImageColor(const Craft::Color& color)
{
	iconImageColorBase = color;
}

void QuickSlotIcon::StartCooldown(const float cooldownTime)
{
	bCooldownState = true;
}

void QuickSlotIcon::StopCooldown()
{
	bCooldownState = false;
}

void QuickSlotIcon::SetGamePause(bool bPause)
{
	bGamePause = bPause;
}