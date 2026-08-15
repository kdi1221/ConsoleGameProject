#pragma once

#include "Actor/Pawn/Pawn.h"
#include <Util/Timer.h>
#include "Types/Defines.h"
#include "Component/AbilitySystemComponent.h"

namespace Craft
{
	class PathMoveComponent;
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
	void BeginPathfindingToTarget();

	/* 이동 중단 */
	void StopMove();

	/* 타겟 추적 중 타겟과의 거리 및 상태 확인 */
	void CheckTargetWhileChase(bool bForcePathUpdate);

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
	/* 이동 컴포넌트 */
	std::shared_ptr<Craft::PathMoveComponent> pathMoveComponent;

private:
	//생성된 방의 인덱스
	RoomDefines::UNIQUE_INDEX_TYPE spawnedRoomIndex = RoomDefines::ROOM_INDEX_INVALID;

	/* 현재 몬스터 비헤이비어 상태값 */
	eMonsterBehavior behaviorState = eMonsterBehavior::Idle;

	/* 추격 경로 탐색 딜레이 타이머 */
	Timer timerFindChasePathDelay;

	//현재 추적중인 대상
	std::weak_ptr<Pawn> chaseTarget;

	//마지막에 추적한 대상의 위치
	Craft::Vector2Int lastChaseTargetPos = INVALID_LAST_POS;
};

