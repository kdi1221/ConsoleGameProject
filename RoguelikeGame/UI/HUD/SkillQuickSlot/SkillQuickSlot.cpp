#include "SkillQuickSlot.h"
#include "UI/HUD/QuickSlotIcon/QuickSlotIcon.h"
#include <cassert>

using namespace Craft;

const int SkillQuickSlot::ICON_GAP = 2;

SkillQuickSlot::SkillQuickSlot(const Vector2Int& positionLT, int numIcon)
	:super(positionLT, (QuickSlotIcon::SLOT_ICON_WIDTH * numIcon) + (ICON_GAP * (numIcon - 1)), QuickSlotIcon::SLOT_ICON_HEIGHT)
{
	assert(numIcon > 0 && "Invalid QuickSlot Icon Num");
	skillIcons.resize(numIcon);

	int iconXPos = QuickSlotIcon::SLOT_ICON_WIDTH;
	for (int i = 0; i < numIcon; ++i)
	{
		skillIcons[i] = AddChildWidget<QuickSlotIcon>(Vector2Int(iconXPos, 0));
		iconXPos += numIcon + ICON_GAP;
	}
}

SkillQuickSlot::~SkillQuickSlot() = default;