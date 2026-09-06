#pragma once

#include "Ability/AbilityObject.h"

namespace Craft
{
	class Actor;
}

class AbilityVitalityCircleObject;

//플레이어 스킬 - 회복 마법진
class AbilityVitalityCircle : public AbilityObject
{
	TYPE_DECLARATIONS(AbilityVitalityCircle, AbilityObject)

public:
	AbilityVitalityCircle(ABILITY_ID_TYPE id, int level);
	virtual ~AbilityVitalityCircle() = default;

public:
	/* Ability 활성화 가능 여부 */
	virtual bool CanActivateAbility() const override;

	/* Ability 활성화 */
	virtual void ActivateAbility() override;

	/* Ability 취소 */
	virtual void CancelAbility() override;

private:
	/* 생성되었던 마법진이 파괴될때 호출 */
	void OnSpawnedVitalityCircleActorDestroy(std::weak_ptr<Craft::Actor> destroyActor);

	/* 기존 마법진 Destroy 처리 */
	void SpawnedVitalityCircleDestroy();

private:
	std::weak_ptr<AbilityVitalityCircleObject> spawnedCircleObject;
};

