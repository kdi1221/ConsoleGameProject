#pragma once

#include "BehaviorTreeComponent.h"
#include <Util/Timer.h>

class BossOneEye;

class BehaviorBossComponent : public BehaviorTreeComponent
{
	TYPE_DECLARATIONS(BehaviorBossComponent, BehaviorTreeComponent)

private:
	virtual void BeginPlay() override;

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

	/* 실행한 Ability가 종료되었을때 호출 */
	virtual void OnEndAbility(const AbilityObject& ability) override;

private:
	/* 다음 공격 패턴을 실행할 타이머 동작*/
	void UpdateNextPatternTimer(float deltaTime);

	/* 첫번째 공격 패턴 실행 */
	void ExecuteAttackPattern1();

private:
	/* Idle 상태에서 다음 패턴을 실행할 Delay 타이머 */
	Timer TimerNextExecutePatternDelay;

	/* 소환 능력 활성화 여부 */
	bool bActivatedSummonAbility = false;

	/* 최초 다음 패턴 타이머 설정 여부 */
	bool bExecuteFirstAttackPattern = false;

	/* 다음 공격 패턴 타이머 설정 여부*/
	bool bSetExecuteNextAttackPatternTimer = false;

	/* owner boss */
	std::shared_ptr<BossOneEye> ownerBossEye;
};

