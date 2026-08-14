#pragma once

#include "Ability/AbilityPeriodicDelay.h"

class Pawn;
class AbilityMelee : public AbilityPeriodicDelay
{
	TYPE_DECLARATIONS(AbilityMelee, AbilityPeriodicDelay)

public:
	AbilityMelee(float interval, float damage);
	virtual ~AbilityMelee() = default;

private:
	/* 딜레이 타이머 동작 */
	virtual void OnIntervalTrigger();

public:
	void SetTargetPawn(std::weak_ptr<Pawn> inTarget);

private:
	/* 타겟 */
	std::weak_ptr<Pawn> target;

	/* 데미지 */
	float damageValue = 0.f;
};

