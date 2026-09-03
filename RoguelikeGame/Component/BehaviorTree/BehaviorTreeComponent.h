#pragma once

#include <Util/Timer.h>
#include <Core/Core.h>
#include <Component/Component.h>
#include <vector>

namespace Craft
{
	class Vector2Int;
}

class Pawn;
class NPCBase;
class AbilityObject;

/* 비헤이비어 트리 컴포넌트 */
class BehaviorTreeComponent : public Craft::Component
{
	TYPE_DECLARATIONS(BehaviorTreeComponent, Component)

public:
	/* 비헤이비어 상태 */
	enum class eBehaviorState
	{
		/* 기본 대기 상태 */
		Idle = 0,

		/* 타겟 추적 중 */
		TargetChase,

		/* 공격 중 */
		Attack
	};

public:
	BehaviorTreeComponent();
	virtual ~BehaviorTreeComponent() = default;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

public:
	/* 타겟을 추적 대상으로 삼음 */
	void SetChaseTarget(std::weak_ptr<Pawn> target);

	/* 이동 목적지에 도착한 경우 호출 */
	void OnMoveFinish();

	/* 이동 목적지로 향하던 도중 충돌하여 멈춘 경우 */
	void OnMoveAbort();

	/* 실행한 Ability가 종료되었을때 호출 */
	void OnEndAbility(const AbilityObject& ability);

public:
	std::shared_ptr<Pawn> GetChaseTarget() const;

public:
	inline eBehaviorState GetBehaviorState() const { return behaviorState; }

protected:
	std::shared_ptr<NPCBase> GetNPCOwner() const;

private:
	/* 비헤이비어 상태 설정 */
	void SetBehaviorState(eBehaviorState newState);

	/* 매 프레임 비헤이비어 상태 처리 */
	void OnBehaviorUpdate(float deltaTime);

private:
	/* 공격 실행 */
	void ExecuteAttack();

	/* 타겟을 향한 경로를 구해서 이동 시작 */
	bool BeginPathfindingToTarget();

	/* 이동 중단 */
	void StopMove();

	/* 타겟 추적 중 다음 상태로 전환 */
	bool TransitionNextStateWhileChase();

	/* 타겟 추적 딜레이 설정(무작위) */
	void SetTargetChaseDelayTime();

	/* 다음 프레임에서 타겟 추적 설정 */
	void SetTargetChaseNextTick();

	/* 현재 상태에 대한 정보 표시(디버깅용) */
	void DrawCurrentState();

protected:
	/* 이전 상태 정리 */
	virtual void ClearPreviouseBehaviorState(eBehaviorState prevState);

	/* 새로운 상태 시작 */
	virtual void BeginNewBehaviorState();

	/* Idle 상태에서 호출 */
	virtual void OnBehaviorIdle(float deltaTime);

	/* 타겟 추적 상태에서 호출 */
	virtual void OnBehaviorChaseTarget(float deltaTime);

	/* 공격 상태에서 호출 */
	virtual void OnBehaviorAttack(float deltaTime);

	/* 타겟이 공격 범위 안에 있는지 확인 */
	virtual bool IsTargetAttackRange(std::shared_ptr<Pawn> targetPawn) const = 0;

	/* 타겟 추적 상황에서 목적지 반환 */
	virtual void GetAvailableChaseTargetPosition(const Craft::Vector2Int& targetPos,
												std::vector<Craft::Vector2Int>& availablePosition) = 0;

private:
	//공격 실행 여부
	bool bActivateAttackAbility = false;

	//경로 탐색 딜레이 min Value
	float minFindPathdelay = 0.5f;

	//경로 탐색 딜레이 max Value
	float maxFindPathdelay = 1.5f;

	/* 추격 경로 탐색 딜레이 타이머 */
	Timer timerFindChasePathDelay;

	//현재 추적중인 대상
	std::weak_ptr<Pawn> chaseTarget;

	/* 현재 비헤이비어 상태값 */
	eBehaviorState behaviorState = eBehaviorState::Idle;

	/* Owner NPC */
	std::weak_ptr<NPCBase> ownerNPC;
};



