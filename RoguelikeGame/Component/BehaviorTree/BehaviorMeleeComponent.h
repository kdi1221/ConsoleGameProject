#pragma once

#include "BehaviorTreeComponent.h"

/* 근접 패턴 비헤이비어 트리 */
class BehaviorMeleeComponent : public BehaviorTreeComponent
{
	TYPE_DECLARATIONS(BehaviorMeleeComponent, BehaviorTreeComponent)

private:
	/* 타겟이 공격 범위 안에 있는지 확인 */
	virtual bool IsTargetAttackRange(std::shared_ptr<Pawn> targetPawn) const override;

	/* 타겟 추적 상황에서 목적지 반환 */
	virtual void GetAvailableChaseTargetPosition(const Craft::Vector2Int& targetPos,
												std::vector<Craft::Vector2Int>& availablePosition) override;
};

