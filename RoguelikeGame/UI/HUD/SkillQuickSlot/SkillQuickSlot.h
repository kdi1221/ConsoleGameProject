#pragma once

#include <UI/Widget/Widget.h>
#include <unordered_map>

class QuickSlotIcon;
class PlayerAbilityInfo;
class AbilityObject;

//하단 스킬 퀵슬롯 
class SkillQuickSlot : public Craft::Widget
{
	TYPE_DECLARATIONS(SkillQuickSlot, Widget)

private:
	static const int ICON_GAP;

public:
	SkillQuickSlot(const Craft::Vector2Int& positionLT);
	virtual ~SkillQuickSlot();

public:
	void UpdateAbilityIcon(const PlayerAbilityInfo& abilityInfo);
	void AbilityCooldownChange(const AbilityObject& ability, const PlayerAbilityInfo& playerAbilityInfo, bool bCooldown);
	void SetGamePause(bool bPause);

private:
	/* Key Code에 대응되는 Skil Slot Icon */
	std::unordered_map<int, std::shared_ptr<QuickSlotIcon>> skillIcons;
};

