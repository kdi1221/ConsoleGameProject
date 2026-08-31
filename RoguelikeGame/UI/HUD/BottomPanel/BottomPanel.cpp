#include "BottomPanel.h"
#include "UI/AttributeBar/AttributeBar.h"
#include "UI/HUD/SkillQuickSlot/SkillQuickSlot.h"

using namespace Craft;

BottomPanel::BottomPanel(const Vector2Int& positionLT, int width, int height)
	:super(positionLT, width, height)
{
	const int attributeBarRadius = 8;
	lifeBar = AddChildWidget<AttributeBar>(Vector2Int(5, -8), attributeBarRadius, Color::Red, Color::Yellow);
	manaBar = AddChildWidget<AttributeBar>(Vector2Int(width - 22, -8), attributeBarRadius, Color::Blue, Color::Yellow);

	skillQuickSlot = AddChildWidget<SkillQuickSlot>(Vector2Int((width >> 1) - 12, -5), 5);
}

BottomPanel::~BottomPanel() = default;

void BottomPanel::SetHealthValue(float amount, float maxValue)
{
	if (lifeBar)
	{
		lifeBar->SetValue(amount, maxValue);
	}
}

void BottomPanel::SetManaValue(float amount, float maxValue)
{
	if (manaBar)
	{
		manaBar->SetValue(amount, maxValue);
	}
}
