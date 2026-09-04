#pragma once

#include "BehaviorTreeComponent.h"

class BehaviorBossComponent : public BehaviorTreeComponent
{
	TYPE_DECLARATIONS(BehaviorBossComponent, BehaviorTreeComponent)

private:
	/* 타겟이 공격 범위 안에 있는지 확인 */
	virtual bool IsTargetAttackRange(std::shared_ptr<Pawn> targetPawn) const override;

	/* 타겟 추적 상황에서 목적지 반환 */
	virtual void GetAvailableChaseTargetPosition(const Craft::Vector2Int& targetPos,
												std::vector<Craft::Vector2Int>& availablePosition) override;

	/* 이전 상태 정리 */
	virtual void ClearPreviouseBehaviorState(eBehaviorState prevState) override;

	/* 새로운 상태 시작 */
	virtual void BeginNewBehaviorState() override;

	/* Idle 상태에서 호출 */
	virtual void OnBehaviorIdle(float deltaTime) override;

	/* 타겟 추적 상태에서 호출 */
	virtual void OnBehaviorChaseTarget(float deltaTime) override;

	/* 공격 상태에서 호출 */
	virtual void OnBehaviorAttack(float deltaTime) override;
};

