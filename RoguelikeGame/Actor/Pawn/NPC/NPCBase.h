#pragma once

#include "Actor/Pawn/Pawn.h"
#include <Util/Timer.h>
#include "Types/Defines.h"
#include "Component/AbilitySystemComponent.h"

namespace Craft
{
	//class PathMoveComponent;
	class NavMovementComponent;
}

/* 모든 NPC(몬스터)의 상위 클래스 */
class NPCBase : public Pawn
{
	TYPE_DECLARATIONS(NPCBase, Pawn)

private:
	static const Craft::Vector2Int INVALID_LAST_POS;

public:
	enum class eMonsterBehavior
	{
		Idle = 0,
		TargetChase,
		Attack
	};

public:
	NPCBase(const Craft::Vector2Int& position,
		const std::wstring& image,
		Craft::Color color,
		float initialHealth,
		RoomDefines::UNIQUE_INDEX_TYPE roomIndex,
		float moveDelay,
		float chaseDelay);

	~NPCBase() = default;

public:
	virtual void Initialize() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

public:
	/* 타겟을 추적 대상으로 삼음 */
	void SetChaseTarget(std::weak_ptr<Pawn> target);

public:
	inline std::weak_ptr<Pawn> GetChaseTarget() const { return chaseTarget; }
	inline RoomDefines::UNIQUE_INDEX_TYPE GetSpawnedRoomIndex() const { return spawnedRoomIndex; }

protected:
	inline eMonsterBehavior GetBehaviorState() const { return behaviorState; }

private:
	/* 비헤이비어 상태 설정 */
	void SetBehaviorState(eMonsterBehavior newState);

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

	/* 현재 이동중인 경로 표시 */
	void DrawMovePaths();

protected:
	virtual void OnBehaviorIdle(float deltaTime);
	virtual void OnBehaviorChaseTarget(float deltaTime);
	virtual void OnBehaviorAttack(float deltaTime);

private:
	void OnMoveFinish();
	void OnMoveAbort();

protected:
	/* 타겟을 추적할때 추적 목적지 반환 */
	virtual void GetAvailableChaseTargetPosition(const Craft::Vector2Int& targetPos, 
												std::vector<Craft::Vector2Int>& availablePosition) = 0;

	/* 타겟이 공격 범위 안에 있는지 확인 */
	virtual bool IsTargetAttackRange(std::shared_ptr<Pawn> targetPawn) const = 0;

	/* 공격 Ability Trigger On */
	virtual void AttackAbilitiesTriggerON() = 0;

	/* 공격 Ability Trigger Off */
	virtual void AttackAbilitiesTriggerOFF() = 0;

private:
	/* 네비게이션 기반 이동 컴포넌트 */
	std::shared_ptr<Craft::NavMovementComponent> navMovementComponent;

private:
	/* 초기 Health Value */
	float initialiHealthValue = 0.f;

	//생성된 방의 인덱스
	RoomDefines::UNIQUE_INDEX_TYPE spawnedRoomIndex = RoomDefines::ROOM_INDEX_INVALID;

	/* 현재 몬스터 비헤이비어 상태값 */
	eMonsterBehavior behaviorState = eMonsterBehavior::Idle;

	/* 추격 경로 탐색 딜레이 타이머 */
	Timer timerFindChasePathDelay;

	//현재 추적중인 대상
	std::weak_ptr<Pawn> chaseTarget;

	//직전에 경로 추적 실패 시 다음 번 경로 강제 업데이트 여부
	bool bForceNextPathUpdate = false;

	//경로 탐색 딜레이 min Value
	float minFindPathdelay = 0.5f;

	//경로 탐색 딜레이 max Value
	float maxFindPathdelay = 1.5f;
};

