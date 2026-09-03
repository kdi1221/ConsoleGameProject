#pragma once

#include "NPCBase.h"

/* 근접 공격 몬스터 베이스 */
class NPCMeleeBase : public NPCBase
{
	TYPE_DECLARATIONS(NPCMeleeBase, NPCBase)

public:
	NPCMeleeBase(const Craft::Vector2Int& position,
		const std::wstring& image,
		Craft::Color color,
		float initialHealth,
		float moveSpeed,
		float damageValue,
		float attackDuration,
		float attackFrameTime,
		RoomDefines::UNIQUE_INDEX_TYPE roomIndex);

	virtual ~NPCMeleeBase() = default;

private:
	/* Pawn의 초기 Ability 구성 */
	virtual void InitializeAbility() override;

	/* 공격 실행 */
	virtual void ExecuteAttack() override;

	/* 공격 프레임에서 호출되는 함수 */
	virtual void OnNotifyAttackFrame(const AbilityObject& ability) override;

private:
	/* 근접공격 실행시 적용할 데미지 수치 */
	float damageAmount = 0.f;

	/* 근접 공격 실행 시간(총 길이) */
	float attackDuration = 0.f;

	/* 근접 공격 중 공격 프레임 시간 */
	float attackFrameTime = 0.f;

	/* 부여된 근접공격 Ability ID */
	ABILITY_ID_TYPE grantedAttackAbilityID = INVALID_ABILITY_ID;
};

