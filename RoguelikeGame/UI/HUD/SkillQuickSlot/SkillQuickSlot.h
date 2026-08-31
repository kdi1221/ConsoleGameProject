#pragma once

#include <UI/Widget/Widget.h>
#include <vector>

class QuickSlotIcon;

//하단 스킬 퀵슬롯 
class SkillQuickSlot : public Craft::Widget
{
	TYPE_DECLARATIONS(SkillQuickSlot, Widget)

private:
	static const int ICON_GAP;

public:
	SkillQuickSlot(const Craft::Vector2Int& positionLT, int numIcon);
	virtual ~SkillQuickSlot();

private:
	std::vector<std::shared_ptr<QuickSlotIcon>> skillIcons;
};

