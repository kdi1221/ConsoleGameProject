#pragma once

#include "Ability/AbilityObject.h"

//플레이어 스킬 텔레포트 
class AbilityTeleport : public AbilityObject
{
	TYPE_DECLARATIONS(AbilityTeleport, AbilityObject)

public:
	AbilityTeleport(ABILITY_ID_TYPE id, int level);
	virtual ~AbilityTeleport() = default;

public:
	/* Ability 활성화 */
	virtual void ActivateAbility() override;

private:
	/* 텔레포트 최대 이동 거리 */
	float maxRange = 30.f;
};

