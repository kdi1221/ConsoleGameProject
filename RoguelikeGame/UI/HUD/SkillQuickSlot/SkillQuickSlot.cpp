#include "SkillQuickSlot.h"
#include "UI/HUD/QuickSlotIcon/QuickSlotIcon.h"
#include "Ability/AbilityDataTable.h"
#include "Game/State/PlayerState/PlayerAbilityInfo.h"
#include "Ability/AbilityObject.h"
#include <Math/Color.h>
#include <vector>
#include <cassert>

using namespace Craft;

const int SkillQuickSlot::ICON_GAP = 2;

const std::vector<std::pair<int, std::wstring>> SKILL_SLOT_ICON_KEYCODES = 
{ 
	{'1', L"1"},
	{'2', L"2"},
	{'3', L"3"},
	{'4', L"4"},
	{VK_RBUTTON, L"RMB"}
};


SkillQuickSlot::SkillQuickSlot(const Vector2Int& positionLT)
	:super(positionLT, 
		QuickSlotIcon::SLOT_ICON_WIDTH * static_cast<int>(SKILL_SLOT_ICON_KEYCODES.size()) + (ICON_GAP * (static_cast<int>(SKILL_SLOT_ICON_KEYCODES.size()) - 1)),
		QuickSlotIcon::SLOT_ICON_HEIGHT)
{
	int iconXPos = QuickSlotIcon::SLOT_ICON_WIDTH;
	for (const auto& slotKeyCode : SKILL_SLOT_ICON_KEYCODES)
	{
		std::shared_ptr<QuickSlotIcon> addedSlotIcon = AddChildWidget<QuickSlotIcon>(Vector2Int(iconXPos, 0));
		skillIcons.insert({ slotKeyCode.first, addedSlotIcon });
		addedSlotIcon->SetIconBottomImage(slotKeyCode.second);
		iconXPos += QuickSlotIcon::SLOT_ICON_WIDTH + ICON_GAP;
	}
}

SkillQuickSlot::~SkillQuickSlot() = default;

void SkillQuickSlot::UpdateAbilityIcon(const PlayerAbilityInfo& abilityInfo)
{
	/* Key Code에 대응되는 Skill QuickSlot 찾음 */
	auto iterFindSkillIcons = skillIcons.find(abilityInfo.GetBindingKeyCode());
	if (iterFindSkillIcons == skillIcons.end())
	{
		return;
	}

	std::shared_ptr<QuickSlotIcon>& slotIcon = iterFindSkillIcons->second;

	const FAbilityData& abilityData = AbilityDataTable::GetAbilityData(abilityInfo.GetAbilityID());
	slotIcon->SetIconImage(abilityData.iconImage);
	slotIcon->SetIconImageColor(abilityData.iconColor);

	// Ability 레벨 표시 
	std::wstring abilityLevelText = L"Lv";
	abilityLevelText += std::to_wstring(abilityInfo.GetAbilityLevel());
	slotIcon->SetIconTopImage(abilityLevelText);
}

void SkillQuickSlot::AbilityCooldownChange(const AbilityObject& ability, const PlayerAbilityInfo& playerAbilityInfo, bool bCooldown)
{
	/* Key Code에 대응되는 Skill QuickSlot 찾음 */
	auto iterFindSkillIcons = skillIcons.find(playerAbilityInfo.GetBindingKeyCode());
	if (iterFindSkillIcons == skillIcons.end())
	{
		return;
	}

	std::shared_ptr<QuickSlotIcon>& slotIcon = iterFindSkillIcons->second;
	if (!slotIcon)
	{
		return;
	}

	if (bCooldown)
	{
		slotIcon->StartCooldown(ability.GetCooldownTime());
		slotIcon->SetCooldownElapsed(ability.GetCooldownElapsedTime());
	}
	else
	{
		slotIcon->StopCooldown();
	}
}

void SkillQuickSlot::SetGamePause(bool bPause)
{
	for (auto& pairSlotIcon : skillIcons)
	{
		if (std::shared_ptr<QuickSlotIcon>& slotIcon = pairSlotIcon.second)
		{
			slotIcon->SetGamePause(bPause);
		}
	}
}