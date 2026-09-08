#include "QuickSlotIcon.h"
#include <Render/Renderer.h>

const int QuickSlotIcon::SLOT_ICON_WIDTH = 9;
const int QuickSlotIcon::SLOT_ICON_HEIGHT = 5;

using namespace Craft;

QuickSlotIcon::QuickSlotIcon(const Craft::Vector2Int& positionLT)
	:super(positionLT, SLOT_ICON_WIDTH, SLOT_ICON_HEIGHT)
{
	//아이콘 출력위치 결정
	const Vector2Int& iconPostionLT = GetPosition();
	const int drawWidth = GetWidth();
	const int drawHeight = GetHeight();
	const int drawRight = iconPostionLT.x + drawWidth - 2;
	const int drawBottom = iconPostionLT.y + drawHeight - 1;

	/* 아이콘 출력위치 결정 */
	iconImageDrawPos = iconPostionLT + Vector2Int(drawWidth >> 1, drawHeight >> 1);

	/* 쿨다운 시작 시 테두리 Draw할 시작 위치 */
	cooldownStartBorderIndex = (drawWidth - 3) >> 1;

	/* 뒷 배경 이미지 */
	backgroundImages.resize(drawHeight + 2);
	for (int y = 0; y <= drawHeight + 1; ++y)
	{
		backgroundImages[y].color = Color::Black;
		backgroundImages[y].drawPos = Vector2Int(iconPostionLT.x, iconPostionLT.y + y - 1);

		backgroundImages[y].image.resize(drawWidth);
		for (int x = 0; x < drawWidth; ++x)
		{
			backgroundImages[y].image[x] = L' ';
		}
	}

	/* 테두리 상단 */
	borderlineImages.emplace_back(FIconImageLine{ Vector2Int(positionLT.x, positionLT.y), outlineColorBase, L"┏" });
	for (int i = positionLT.x + 2; i < drawRight - 1; ++i)
	{
		borderlineImages.emplace_back(FIconImageLine{ Vector2Int(i, positionLT.y), outlineColorBase, L"━" });
	}
	borderlineImages.emplace_back(FIconImageLine{ Vector2Int(drawRight, positionLT.y), outlineColorBase, L"┓" });

	/* 우측 */
	for (int i = positionLT.y + 1; i < drawBottom; ++i)
	{
		borderlineImages.emplace_back(FIconImageLine{ Vector2Int(drawRight, i), outlineColorBase, L"┃" });
	}

	/* 테두리 하단 */
	borderlineImages.emplace_back(FIconImageLine{ Vector2Int(drawRight, drawBottom), outlineColorBase, L"┛" });
	for (int i = drawRight - 2; i > positionLT.x + 1; --i)
	{ 
		borderlineImages.emplace_back(FIconImageLine{ Vector2Int(i, drawBottom), outlineColorBase, L"━" });
	}
	borderlineImages.emplace_back(FIconImageLine{ Vector2Int(positionLT.x, drawBottom), outlineColorBase, L"┗" });

	/* 좌측 */
	for (int i = drawBottom - 1; i >= positionLT.y + 1; --i)
	{
		borderlineImages.emplace_back(FIconImageLine{ Vector2Int(positionLT.x, i), outlineColorBase, L"┃" });
	}
}

void QuickSlotIcon::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (!bGamePause)
	{
		// 게임 일시정지가 아닌동안 쿨타임 타이머 돌려서 갱신
		UpdateCooldownTimer(deltaTime);
	}

	if (!bGamePause && bCooldownState)
	{
		
		if (!cooldownTimer.IsTimeOut())
		{
			cooldownTimer.Tick(deltaTime);

			const float cooldownTargetTime = cooldownTimer.GetTargetTime();
			const float cooldownElapsed = cooldownTimer.GetElapsedTime();
			const float cooldownRate = cooldownElapsed / cooldownTargetTime;

			cooldownBorderDrawNum = static_cast<int>(borderlineImages.size() * cooldownRate);
		}
	}
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
	for (const FIconImageLine& backgroundLine : backgroundImages)
	{
		renderer.SubmitUI(backgroundLine.image, backgroundLine.drawPos, backgroundLine.color, renderSortingOrder);
	}

	/* 테두리(쿨타임 표시) */
	int cooldownBorderStart = 0;
	int cooldownBorderEnd = cooldownBorderStart + static_cast<int>(borderlineImages.size());
	if (bCooldownState)
	{
		cooldownBorderStart = cooldownStartBorderIndex;
		cooldownBorderEnd = cooldownBorderStart + cooldownBorderDrawNum;
	}

	const Color outlineColor = bCooldownState ? outlineColorCooldown : outlineColorBase;

	for (int i = cooldownBorderStart;
		i < cooldownBorderEnd;
		++i)
	{
		/* 실제 출력할 테두리 인덱스 */
		const int borderIndex = i % static_cast<int>(borderlineImages.size());

		const FIconImageLine& borderLine = borderlineImages[borderIndex];
		renderer.SubmitUI(borderLine.image, borderLine.drawPos, outlineColor, renderSortingOrder);
	}

	//가운데 아이콘 표시
	const Color iconImageColor = bCooldownState ? iconImageColorCooldown : iconImageColorBase;
	renderer.SubmitUI(iconImage, iconImageDrawPos, iconImageColor, renderSortingOrder);

	//상단 이미지 표시(Lv)
	if (!topInfoImage.empty())
	{
		renderer.SubmitUI(topInfoImage, topInfoImageDrawPos, Color::BrightWhite, renderSortingOrder);
	}

	//하단 이미지 표시(Key)
	if (!bottomInfoImage.empty())
	{
		renderer.SubmitUI(bottomInfoImage, bottomInfoImageDrawPos, Color::BrightWhite, renderSortingOrder);
	}
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
	cooldownTimer.SetTargetTime(cooldownTime);
}

void QuickSlotIcon::SetCooldownElapsed(const float elapsedTime)
{
	if (!bCooldownState || elapsedTime <= 0.f)
	{
		return;
	}

	cooldownTimer.SetElapsedTime(elapsedTime);

	UpdateCooldownBorderDrawNum();
}

void QuickSlotIcon::StopCooldown()
{
	bCooldownState = false;
	cooldownTimer.Reset();
}

void QuickSlotIcon::SetGamePause(bool bPause)
{
	bGamePause = bPause;
}

void QuickSlotIcon::SetIconBottomImage(const std::wstring& bottomImage)
{
	bottomInfoImage = bottomImage;

	const int imageLength = static_cast<int>(bottomInfoImage.length());
	if (imageLength <= 0)
	{
		return;
	}

	const int drawWidth = GetWidth();
	const int drawHeight = GetHeight();
	const Vector2Int& iconPostionLT = GetPosition();
	bottomInfoImageDrawPos = iconPostionLT + Vector2Int((drawWidth - imageLength) >> 1, drawHeight);
}

void QuickSlotIcon::SetIconTopImage(const std::wstring & topImage)
{
	topInfoImage = topImage;

	const int imageLength = static_cast<int>(topInfoImage.length());
	if (imageLength <= 0)
	{
		return;
	}

	const int drawWidth = GetWidth();
	const Vector2Int& iconPostionLT = GetPosition();
	topInfoImageDrawPos = iconPostionLT + Vector2Int((drawWidth - imageLength) >> 1, -1);
}

void QuickSlotIcon::UpdateCooldownTimer(const float deltaTime)
{
	if (!bCooldownState || !cooldownTimer.IsTimeOut())
	{
		return;
	}

	cooldownTimer.Tick(deltaTime);

	UpdateCooldownBorderDrawNum();
}

void QuickSlotIcon::UpdateCooldownBorderDrawNum()
{
	const float cooldownTargetTime = cooldownTimer.GetTargetTime();
	const float cooldownElapsed = cooldownTimer.GetElapsedTime();
	const float cooldownRate = cooldownElapsed / cooldownTargetTime;

	cooldownBorderDrawNum = static_cast<int>(borderlineImages.size() * cooldownRate);
}
