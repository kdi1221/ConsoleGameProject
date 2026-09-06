#include "BehaviorBossComponent.h"
#include "Actor/Pawn/NPC/Boss/BossOneEye.h"
#include "Util/Util.h"
#include <cassert>

void BehaviorBossComponent::BeginPlay()
{
	super::BeginPlay();

	ownerBossEye = Cast<BossOneEye>(GetNPCOwner());
	assert(ownerBossEye && "Invalid ownerBossEye");
}

bool BehaviorBossComponent::IsTargetAttackRange(std::shared_ptr<Pawn> targetPawn) const
{
	return GetChaseTarget() == targetPawn;
}

void BehaviorBossComponent::GetAvailableChaseTargetPosition(const Craft::Vector2Int& targetPos, 
															std::vector<Craft::Vector2Int>& availablePosition)
{

}

void BehaviorBossComponent::ClearPreviouseBehaviorState(eBehaviorState prevState)
{
	switch (prevState)
	{
	case eBehaviorState::Idle:
		{
			//OutputDebugStringA("On Idle Exit\n");
		}
		break;

	case eBehaviorState::TargetChase:
		{

		}
		break;

	case eBehaviorState::Attack1:
		{
			
		}
		break;
	}
}

void BehaviorBossComponent::BeginNewBehaviorState()
{
	switch (GetBehaviorState())
	{
	case eBehaviorState::Idle:
		{
			/* 첫 공격패턴 실행이후 Idle 상태로 전환되었으면 다음 공격 타이머 지정 */
			if (bExecuteFirstAttackPattern)
			{
				const float nextAttackPatternDelay = Util::RandomRange(20.f, 40.f);
				TimerNextExecutePatternDelay.SetTargetTime(nextAttackPatternDelay);
				TimerNextExecutePatternDelay.Reset();
				bSetExecuteNextAttackPatternTimer = true;
			}
		}
		break;

	case eBehaviorState::TargetChase:
		{
			
		}
		break;

	case eBehaviorState::Attack1:
		{
			ExecuteAttackPattern1();
		}
		break;
	}
}

void BehaviorBossComponent::OnBehaviorIdle(float deltaTime)
{
	if (!ownerBossEye || !ownerBossEye->IsOpenEyeActivated())
	{
		return;
	}

	if (!bActivatedSummonAbility)
	{
		/* 처음 소환 Ability가 활성화안된 상태에서 활성화한다. */
		ownerBossEye->ActivateSummonAbility();
		bActivatedSummonAbility = true;
	}

	if (!bExecuteFirstAttackPattern)
	{
		/* 최초 공격 실행 타이머 지정 */
		TimerNextExecutePatternDelay.SetTargetTime(10.f);
		TimerNextExecutePatternDelay.Reset();
		bSetExecuteNextAttackPatternTimer = true;

		bExecuteFirstAttackPattern = true;
	}

	if (bSetExecuteNextAttackPatternTimer)
	{
		UpdateNextPatternTimer(deltaTime);
	}
}

void BehaviorBossComponent::OnBehaviorChaseTarget(float deltaTime)
{
	/* 따로 타겟을 추적할 필요는 없으므로 곧바로 Idle로 돌아감 */
	SetIdleState();
}

void BehaviorBossComponent::OnBehaviorAttack(float deltaTime)
{

}

void BehaviorBossComponent::OnEndAbility(const AbilityObject& ability)
{
	//Idle 상태로 전환한다.
	SetIdleState();
}

void BehaviorBossComponent::UpdateNextPatternTimer(float deltaTime)
{
	TimerNextExecutePatternDelay.Tick(deltaTime);
	if (TimerNextExecutePatternDelay.IsTimeOut())
	{
		//Util::RandomRange(1, 1)
		int selectPattern = 0;
		SetAttackState(selectPattern);

		bExecuteFirstAttackPattern = false;
		TimerNextExecutePatternDelay.Reset();
	}
}

void BehaviorBossComponent::ExecuteAttackPattern1()
{
	if (!ownerBossEye || !ownerBossEye->IsOpenEyeActivated())
	{
		return;
	}

	ownerBossEye->ActivateShockWaveAbility();
}

