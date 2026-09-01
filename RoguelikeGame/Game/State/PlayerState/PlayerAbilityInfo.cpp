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

void PlayerAbilityInfo::SaveCooldownElapsedTime(float elapsedTime)
{
	savedCooldownTime = elapsedTime;
}

void PlayerAbilityInfo::ResetCooldownElapsedTime()
{
	savedCooldownTime = 0.f;
}
