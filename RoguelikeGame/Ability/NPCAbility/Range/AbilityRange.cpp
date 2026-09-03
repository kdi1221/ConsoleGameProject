#include "AbilityRange.h"
#include "Actor/Pawn/NPC/NPCBase.h"
#include <cassert>

using namespace Craft;

AbilityRange::AbilityRange(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{

}

void AbilityRange::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (IsActivate())
	{
		UpdateAttack(deltaTime);
	}
}

void AbilityRange::ActivateAbility()
{
	super::ActivateAbility();

	attackDurationTimer.Reset();
	activateAttacked = false;
}

void AbilityRange::EndAbility(bool bCancelAbility)
{
	super::EndAbility(bCancelAbility);

	attackDurationTimer.Reset();
	activateAttacked = false;
}

void AbilityRange::SetAttackFrame(float attackDuration, float frameTime)
{
	attackDurationTimer.SetTargetTime(attackDuration);
	attackFrameTime = frameTime;
}

void AbilityRange::UpdateAttack(float deltaTime)
{
	attackDurationTimer.Tick(deltaTime);

	if (!activateAttacked && attackDurationTimer.GetElapsedTime() >= attackFrameTime)
	{
		ExecuteAttackFrame();
	}

	if (attackDurationTimer.IsTimeOut())
	{
		EndAbility(false);
	}
}

void AbilityRange::ExecuteAttackFrame()
{
	std::shared_ptr<NPCBase> ownerNPCBase = Cast<NPCBase>(GetOwnerPawn());
	if (!ownerNPCBase)
	{
		return;
	}

	ownerNPCBase->OnNotifyAttackFrame(*this);

	activateAttacked = true;
}