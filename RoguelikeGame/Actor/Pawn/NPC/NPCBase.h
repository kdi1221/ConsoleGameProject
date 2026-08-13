#pragma once

#include "Actor/Pawn/Pawn.h"
#include <Util/Timer.h>
#include "Types/Defines.h"

namespace Craft
{
	class PathMoveComponent;
}

/* 모든 NPC(몬스터)의 상위 클래스 */
class NPCBase : public Pawn
{
	TYPE_DECLARATIONS(NPCBase, Pawn)

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
		int CollisionWidth,
		int initialHealth,
		RoomDefines::UNIQUE_INDEX_TYPE roomIndex,
		float moveDelay,
		float ChaseDelay);

	~NPCBase() = default;

public:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

public:
	/* 타겟을 추적 대상으로 삼음 */
	void SetChaseTarget(std::weak_ptr<Pawn> target);

public:
	inline RoomDefines::UNIQUE_INDEX_TYPE GetSpawnedRoomIndex() const { return spawnedRoomIndex; }

private:
	/* 비헤이비어 상태 설정 */
	void SetBehaviorState(eMonsterBehavior newState);

	/* 타겟을 향한 경로를 구해서 이동 시작 */
	void BeginPathfindingToTarget();

	/* 이동 중단 */
	void StopMove();

private:
	void OnBehaviorIdle(float deltaTime);
	void OnBehaviorChaseTarget(float deltaTime);
	void OnBehaviorAttack(float deltaTime);

private:
	void OnMoveFinish();
	void OnMoveAbort();

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

	//현재 이동중인 경로
	std::vector<Craft::Vector2Int> debugMovePaths;
};

