#pragma once

#include "NPCBase.h"
#include <string>
#include <Math/Color.h>

//원거리 공격 몬스터 베이스 
class NPCRangeBase : public NPCBase
{
	TYPE_DECLARATIONS(NPCRangeBase, NPCBase)

public:
	NPCRangeBase(const Craft::Vector2Int& position,
		const std::wstring& image,
		Craft::Color color,
		float initialHealth,
		float moveSpeed,
		float attackDuration,
		float attackFrameTime,
		float enableFireRange,
		RoomDefines::UNIQUE_INDEX_TYPE roomIndex);

	virtual ~NPCRangeBase() = default;

protected:
	virtual void SpawnProjectile(const Craft::Vector2Int& spawnPosition, const Craft::Vector2Float& aimingDireciton) = 0;

private:
	/* Pawn의 초기 Ability 구성 */
	virtual void InitializeAbility() override;

	/* 공격 실행 */
	virtual void ExecuteAttack() override;

	/* 공격 프레임에서 호출되는 함수 */
	virtual void OnNotifyAttackFrame(const AbilityObject& ability) override;

private:
	/* 원거리 공격 실행 시간(총 길이) */
	float attackDuration = 0.f;

	/* 원거리 공격 중 실제 공격 프레임 시간 */
	float attackFrameTime = 0.f;

	/* 부여된 원거리 공격 Ability */
	ABILITY_ID_TYPE grantRangeAttackAbilityID = INVALID_ABILITY_ID;
};