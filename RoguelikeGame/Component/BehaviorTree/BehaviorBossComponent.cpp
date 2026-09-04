#include "BehaviorBossComponent.h"

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

	case eBehaviorState::Attack:
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
			//OutputDebugStringA("On Idle Begin\n");
		}
		break;

	case eBehaviorState::TargetChase:
		{
			
		}
		break;

	case eBehaviorState::Attack:
		{
			
		}
		break;
	}
}

void BehaviorBossComponent::OnBehaviorIdle(float deltaTime)
{

}

void BehaviorBossComponent::OnBehaviorChaseTarget(float deltaTime)
{

}

void BehaviorBossComponent::OnBehaviorAttack(float deltaTime)
{

}

