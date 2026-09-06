#include "BossStatusPanel.h"
#include "UI/BackgroundWidget.h"
#include <UI/Widget/TextBlock/TextBlock.h>
#include <UI/Widget/ProgressBar/ProgressBar.h>

using namespace Craft;

BossStatusPanel::BossStatusPanel(const Vector2Int& positionLT, int width, int height)
	:super(positionLT, width, height)
{
	AddChildWidget<BackgroundWidget>(Vector2Int(0, 0), width, height, Color::Yellow);

	const int renderSortingOrder = GetRenderSortingOrder();

	textBlockBossName = AddChildWidget<TextBlock>(Vector2Int((width >> 1) - 8, 1), 17);

	healthBar = AddChildWidget<ProgressBar>(Vector2Int(5, 3), width - 10, Color::DarkGray, Color::Red);
	healthBar->SetValue(1.f, 1.f);
}

BossStatusPanel::~BossStatusPanel()
{
}

void BossStatusPanel::SetHealthValue(float currentValue, float maxValue)
{
	if (healthBar)
	{
		healthBar->SetValue(currentValue, maxValue);
	}
}

void BossStatusPanel::SetBossNameText(const std::wstring& bossNameText)
{
	if (textBlockBossName)
	{
		textBlockBossName->SetTextValue(bossNameText);
	}
}