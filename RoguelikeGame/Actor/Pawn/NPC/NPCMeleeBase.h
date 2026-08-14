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
				RoomDefines::UNIQUE_INDEX_TYPE roomIndex,
				float moveDelay,
				float chaseDelay,
				float attackDelay,
				float damageValue);
	virtual ~NPCMeleeBase() = default;

private:
	/* Pawn의 초기 Ability 구성 */
	virtual void InitializeAbility() override;

private:
	/* 타겟을 추적할때 추적 목적지 반환 */
	virtual void GetAvailableChaseTargetPosition(const Craft::Vector2Int& targetPos, std::vector<Craft::Vector2Int>& availablePosition) override;

	/* 타겟이 공격 범위 안에 있는지 확인 */
	virtual bool IsTargetAttackRange(std::shared_ptr<Pawn> targetPawn) const override;

	/* 공격 Ability Trigger On */
	virtual void AttackAbilitiesTriggerON();

	/* 공격 Ability Trigger Off */
	virtual void AttackAbilitiesTriggerOFF();

private:
	/* 부여된 근접 공격 Ability */
	AbilityObject::ABILITY_ID_TYPE grantMeleeAttackID = AbilityObject::INVALID_ABILITY_ID;

	/* 공격 딜레이 */
	float attackDelayInterval = 0.f;

	/* 데미지 수치 */
	float attackDamage = 0.f;
};

