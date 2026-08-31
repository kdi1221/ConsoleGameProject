#pragma once

#include <UI/Widget/Widget.h>

class AttributeBar;
class SkillQuickSlot;
class PlayerAbilityInfo;
class AbilityObject;

//하단 HUD 패널 
class BottomPanel : public Craft::Widget
{
	TYPE_DECLARATIONS(BottomPanel, Widget)

public:
	BottomPanel(const Craft::Vector2Int& positionLT, int width, int height);
	virtual ~BottomPanel();

private:
	/* HP 바 */
	std::shared_ptr<AttributeBar> lifeBar;

	/* Mana 바 */
	std::shared_ptr<AttributeBar> manaBar;

	/* Skill QuickSlot */
	std::shared_ptr<SkillQuickSlot> skillQuickSlot;


public:
	void SetHealthValue(float amount, float maxValue);
	void SetManaValue(float amount, float maxValue);
	void UpdateAbilityIcon(const PlayerAbilityInfo& abilityInfo);
	void AbilityCooldownChange(const AbilityObject& ability, const PlayerAbilityInfo& playerAbilityInfo, bool bCooldown);
	void SetGamePause(bool bPause);
};

