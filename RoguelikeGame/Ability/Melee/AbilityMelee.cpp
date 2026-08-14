#include "AbilityMelee.h"
#include "Actor/Pawn/Pawn.h"
#include <cassert>

using namespace Craft;

AbilityMelee::AbilityMelee(float interval, float damage)
	:super(interval)
	,damageValue(damage)
{
	
}

void AbilityMelee::OnIntervalTrigger()
{
	std::shared_ptr<Pawn> targetPawn = target.lock();
	if (!targetPawn)
	{
		return;
	}

	std::shared_ptr<Pawn> ownerPawn = GetOwnerPawn();
	assert(ownerPawn && "Invlid ownerActor");

	const Vector2Int distance = targetPawn->GetWorldPosition() - ownerPawn->GetWorldPosition();

	/* 타겟이 사정거리안에 있을때만(8방향) 공격 가능 */
	if (abs(distance.x) <= 1 && abs(distance.y) <= 1)
	{
		targetPawn->TakeDamage(damageValue);
	}
}

void AbilityMelee::SetTargetPawn(std::weak_ptr<Pawn> inTarget)
{
	target = inTarget;
}
