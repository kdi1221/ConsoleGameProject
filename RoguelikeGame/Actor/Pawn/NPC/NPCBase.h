#pragma once

#include "Actor/Pawn/Pawn.h"
#include <Util/Timer.h>
#include "Types/Defines.h"
#include "Component/AbilitySystemComponent.h"

namespace Craft
{
	class NavMovementComponent;
}

class AbilityObject;
class BehaviorTreeComponent;

/* 모든 NPC(몬스터)의 상위 클래스 */
class NPCBase : public Pawn
{
	TYPE_DECLARATIONS(NPCBase, Pawn)

public:
	/* 몬스터 패턴 지정 */
	enum class eMonsterPattern
	{
		/* 근접 */
		Melee = 0,

		/* 원거리 */
		Range,	
	};

public:
	NPCBase(const Craft::Vector2Int& position,
			const std::wstring& image,
			Craft::Color color,
			float initialHealth,
			float moveSpeed,
			eMonsterPattern pattern,
			RoomDefines::UNIQUE_INDEX_TYPE roomIndex);

	~NPCBase() = default;

public:
	virtual void Initialize() override;
	virtual void Draw() override;
	virtual void Destroy() override;

public:
	/* 타겟을 추적 대상으로 삼음 */
	void SetChaseTarget(std::weak_ptr<Pawn> target);

public:
	/* 타겟 위치를 향해 경로 탐색 후 이동 */
	bool BeginPathfindingToTargetMove(const Craft::Vector2Int& targetPosition);

	/* 이동 중단 */
	void StopMove();

	/* 공격 실행 */
	virtual void ExecuteAttack();

	/* 공격 프레임에서 호출되는 함수 */
	virtual void OnNotifyAttackFrame(const AbilityObject& ability);

public:
	inline RoomDefines::UNIQUE_INDEX_TYPE GetSpawnedRoomIndex() const { return spawnedRoomIndex; }

protected:
	std::shared_ptr<Pawn> GetChaseTarget() const;
	std::shared_ptr<BehaviorTreeComponent> GetBehaviorTreeComponent() const;

private:
	/* 현재 이동중인 경로 표시 */
	void DrawMovePaths();

private:
	/* NavMovementComponent에서 이동 완료되었을때 호출 */
	void OnMoveFinish();

	/* NavMovementComponent에서 이동 중단 되었을때 호출 */
	void OnMoveAbort();

	/* AbilitySystemComponent에서 Ability 활성화 되었을때 호출 */
	void OnActivateAbility(const AbilityObject& ability, bool bActivate);

private:
	/* 네비게이션 기반 이동 컴포넌트 */
	std::shared_ptr<Craft::NavMovementComponent> navMovementComponent;

	/* 비헤이비어 트리 컴포넌트 */
	std::shared_ptr<BehaviorTreeComponent> behaviorTreeComponent;

private:
	/* 초기 Health Value */
	float initialiHealthValue = 0.f;

	//생성된 방의 인덱스
	RoomDefines::UNIQUE_INDEX_TYPE spawnedRoomIndex = RoomDefines::ROOM_INDEX_INVALID;
};

