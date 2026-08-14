#pragma once

#include "NPCBase.h"
#include <string>
#include <Math/Color.h>

class NPCRangeBase : public NPCBase
{
	TYPE_DECLARATIONS(NPCRangeBase, NPCBase)

public:
	NPCRangeBase(const Craft::Vector2Int& position,
		const std::wstring& image,
		Craft::Color color,
		float initialHealth,
		RoomDefines::UNIQUE_INDEX_TYPE roomIndex,
		float moveDelay,
		float chaseDelay);
	virtual ~NPCRangeBase() = default;

private:
	/* Pawn의 초기 Ability 구성 */
	virtual void InitializeAbility() override;

private:
	/* 타겟을 추적할때 추적 목적지 반환 */
	virtual void GetAvailableChaseTargetPosition(const Craft::Vector2Int& targetPos, std::vector<Craft::Vector2Int>& availablePosition) override;

	/* 타겟이 공격 범위 안에 있는지 확인 */
	virtual bool IsTargetAttackRange(std::shared_ptr<Pawn> targetPawn) const override;

	/* 공격 Ability Trigger On */
	virtual void AttackAbilitiesTriggerON() override;

	/* 공격 Ability Trigger Off */
	virtual void AttackAbilitiesTriggerOFF() override;

	/* 공격 도중 호출 */
	virtual void OnBehaviorAttack(float deltaTime) override;

private:
	void AdjustAimDirection();

protected:
	float projectileFireDelay = 0.f;
	std::wstring projectileImage = L"";
	Craft::Color projectileColor = Craft::Color::White;
	float projectileMoveDelayMin = 0.f;
	float projectileMoveDelayMax = 0.f;
	float projectileDamageValue = 0.f;
	float attackRange = 0.f; 

private:
	/* 부여된 원거리 공격 Ability */
	AbilityObject::ABILITY_ID_TYPE grantRangeAttackID = AbilityObject::INVALID_ABILITY_ID;
};

