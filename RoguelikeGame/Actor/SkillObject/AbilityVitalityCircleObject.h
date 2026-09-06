#pragma once

#include "Types/Enums.h"
#include "SkillObjectActor.h"
#include <Util/Timer.h>

namespace Craft
{
	class ParticleComponent;
}

/* 플레이어가 마법진 스킬 사용시 스폰되는 Actor */
class AbilityVitalityCircleObject : public SkillObject
{
	TYPE_DECLARATIONS(AbilityVitalityCircleObject, SkillObject)

public:
	AbilityVitalityCircleObject(const Craft::Vector2Int& position);
	virtual ~AbilityVitalityCircleObject() = default;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	//virtual void Draw() override;

public:
	void SetInstigatorTeamID(eTeamID teamID);
	void SetlimitRange(float range);
	void SetHealthRegeneration(float rate);
	void SetManaRegeneration(float rate);
	void SetLifeSpan(float lifeTime);
	void SetIntervalRegeneration(float interval);
	
private:
	void ExecuteRegeneration();

private:
	/* 시전자의 Team ID */
	eTeamID instigatorTeamID = eTeamID::None;

	/* 힐 가능 범위 */
	float limitRange = 0.f;

	/* 주기당 체력 회복 값 */
	float healthRegeneration = 0.f;

	/* 주기당 마나 회복 값 */
	float manaRegeneration = 0.f;

	/* 수명 타이머 */
	Timer timerLifeSpan;

	/* 회복 주기 */
	Timer timerIntervalRegen;

private:
	/* 마법진 표시 파티클 컴포넌트 */
	std::shared_ptr<Craft::ParticleComponent> particleComponent;

//private:
//	/* 범위 디버깅용 타일 위치 */
//	std::vector<Craft::Vector2Int> circleOuterTileCoord;
	
};

