#pragma once

#include "Ability/AbilityObject.h"
#include <Util/Timer.h>
#include <unordered_map>

class TilemapLevel;
class NPCBase;

/* 보스몬스터 소환 스킬 */
class AbilitySummon : public AbilityObject
{
	TYPE_DECLARATIONS(AbilitySummon, AbilityObject)
		
public:
	AbilitySummon(ABILITY_ID_TYPE id, int level);
	virtual ~AbilitySummon() = default;

public:
	virtual void Tick(float deltaTime) override;

	/* Ability 활성화 */
	virtual void ActivateAbility() override;

	/* Ability 종료 */
	virtual void EndAbility(bool bCancelAbility) override;

public:
	/* 소환 딜레이 지정 */
	void SetSummonDelay(float deltaTime);

	/* 소환될 몬스터가 추적할 타겟 지정 */
	void SetSummonNPCChaseTarget(std::weak_ptr<Pawn> target);

	/* 소환 최대 마리수 지정 */
	void SetMaxSummonNum(int maxNum);

	/* 딜레이 별 소환 최대 마리수 지정 */
	void SetMaxSummonDelayNum(int maxNum);

private:
	/* 활성화되어있는동안 딜레이 체크하여 소환 실행 */
	void CheckSummonDelayAndExecute(float delayTime);

	/* 실제 소환 실행 */
	void ExecuteSummon();

	/* 소환되어있던 몬스터들 모두 제거 */
	void DestorySummonMonster();

	/* 소환된 몬스터가 사망했을때 이벤트 */
	void OnEventSummonNPCDeath(std::shared_ptr<Pawn> deathPawn);

private:
	/* 소환되어서 관리중인 NPC List*/
	std::unordered_map<PawnUniqueIDType, std::weak_ptr<NPCBase>> summonNPCs;

	/* 소환 최대 마리수 */
	int maxSummonNum = 0;

	/* 딜레이 별 소환 최대 마리수 */
	int maxSummonDelayNum = 0;

	/* 소환 딜레이 타이머 */
	Timer summonDelayTimer;

	/* 소환될 몬스터가 추적할 타겟 */
	std::weak_ptr<Pawn> chaseTarget;
};

