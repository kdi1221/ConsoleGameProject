#pragma once

#include "Ability/AbilityObject.h"
#include <Util/Timer.h>

//NPC 원거리 공격 Ability
class AbilityRange : public AbilityObject
{
	TYPE_DECLARATIONS(AbilityRange, AbilityObject)

public:
	AbilityRange(ABILITY_ID_TYPE id, int level);
	virtual ~AbilityRange() = default;

public:
	virtual void Tick(float deltaTime) override;

	/* Ability 활성화 */
	virtual void ActivateAbility() override;

	/* Ability 종료 */
	virtual void EndAbility(bool bCancelAbility) override;

public:
	/* 공격 타이머 및 프레임 지정 */
	void SetAttackFrame(float attackDuration, float frameTime);

private:
	/* 틱 별 호출되는 Attack 함수 */
	void UpdateAttack(float deltaTime);

	/* 공격 타이밍 프레임에 한번 실행되는 Attack 함수*/
	void ExecuteAttackFrame();


private:
	/* 활성화 이후 공격이 실행되었는지 여부 */
	bool activateAttacked = false;

	/* 공격 타이밍 프레임 */
	float attackFrameTime = 0.f;

	/* 공격 진행 타이머 */
	Timer attackDurationTimer;
};