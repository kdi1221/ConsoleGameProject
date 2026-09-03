#include "AbilityMelee.h"
#include "Actor/Pawn/NPC/NPCBase.h"
#include <cassert>

using namespace Craft;

AbilityMelee::AbilityMelee(ABILITY_ID_TYPE id, int level)
	:super(id, level)
{

}

void AbilityMelee::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (IsActivate())
	{
		UpdateAttack(deltaTime);
	}
}

void AbilityMelee::ActivateAbility()
{
	super::ActivateAbility();

	attackDurationTimer.Reset();
	activateAttacked = false;
}

void AbilityMelee::EndAbility(bool bCancelAbility)
{
	super::EndAbility(bCancelAbility);

	attackDurationTimer.Reset();
	activateAttacked = false;
}

void AbilityMelee::SetAttackFrame(float attackDuration, float frameTime)
{
	attackDurationTimer.SetTargetTime(attackDuration);
	attackFrameTime = frameTime;
}

void AbilityMelee::UpdateAttack(float deltaTime)
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

void AbilityMelee::ExecuteAttackFrame()
{
	std::shared_ptr<NPCBase> ownerNPCBase = Cast<NPCBase>(GetOwnerPawn());
	if (!ownerNPCBase)
	{
		return;
	}

	ownerNPCBase->OnNotifyAttackFrame(*this);

	activateAttacked = true;
}