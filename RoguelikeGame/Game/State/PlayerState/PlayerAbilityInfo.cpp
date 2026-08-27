#include "PlayerAbilityInfo.h"

PlayerAbilityInfo::PlayerAbilityInfo(int id, int level, int keyCode)
	:abilityID(id)
	,currentLevel(level)
	,bindingKeyCode(keyCode)
{

}

void PlayerAbilityInfo::SetAbilityLevel(int newLevel)
{
	currentLevel = newLevel;
}

void PlayerAbilityInfo::SetBindingKeyCode(int newKeyCode)
{
	bindingKeyCode = newKeyCode;
}
