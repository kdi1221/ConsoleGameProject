#pragma once

#include "Ability/AbilityObject.h"
#include "Util/Timer.h"

/* 보스몬스터 : 쇼크웨이브 */
class AbilityShockWave : public AbilityObject
{
	TYPE_DECLARATIONS(AbilityShockWave, AbilityObject)

public:
	AbilityShockWave(ABILITY_ID_TYPE id, int level);
	virtual ~AbilityShockWave() = default;

private:
	virtual void Tick(float deltaTime) override;

public:
	/* Ability 활성화 */
	virtual void ActivateAbility() override;
	virtual void EndAbility(bool bCancelAbility) override;

private:
	/* ShockWave Actor 생성 */
	void SpawnShockWaveActor();

private:
	/* 다음 쇼크웨이브 생성 딜레이 */
	Timer nextSpawnWaveDelay;

	/* 현재까지 생성된 쇼크웨이브 생성 갯수 */
	int currentSpawnShockWave = 0;

	/* 최대 생성 가능한 쇼크 웨이브 생성 갯수 */
	int maxSpawnShockWave = 3;
};

