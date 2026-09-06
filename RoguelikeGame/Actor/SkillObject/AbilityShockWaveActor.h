#pragma once

#include "Types/Enums.h"
#include "Types/Defines.h"
#include "SkillObjectActor.h"
#include <Math/Vector2Float.h>
#include <unordered_set>

/* 보스몬스터 쇼크웨이브 스킬 사용시 스폰되는 Actor */
class AbilityShockWaveActor : public SkillObject
{
	TYPE_DECLARATIONS(AbilityShockWaveActor, SkillObject)

public:
	AbilityShockWaveActor(const Craft::Vector2Int& position, 
							eTeamID TeamID, 
							float extendSpeed, 
							float beginRange, 
							float limitRange, 
							float duration,
							float damage);

	virtual ~AbilityShockWaveActor() = default;

public:
	virtual void Tick(float deltaTime) override;
	virtual void PostTick(float deltaTime) override;
	virtual void Draw() override;

private:
	/* 시간에 따라 범위 확장 */
	void UpdateRangeCircle(float deltaTime);

	/* 범위 계산을 통한 데미지 처리 */
	void ApplyRangeDamage();

private:
	/* 노바 외곽선 표시 이미지 */
	std::wstring outlineImage = L" ";

	/* 노바 외곽선 이미지 색상 */
	Craft::Color outlineColor = Craft::Color::BG_LightPurple;

	/* 시전자의 Team ID */
	eTeamID instigatorTeamID = eTeamID::None;

	/* 이전 범위 */
	float prevRange = 0.f;

	/* 현재 범위 */
	float currentRange = 0.f;

	/* 확장 속도 */
	float extendSpeed = 0.f;

	/* 최대 범위 */
	float limitRange = 0.f;

	/* 데미지 */
	float damageAmount = 0.f;

	/* 경과 시간 */
	float elapsedTime = 0.f;

	/* 지속 시간 */
	float duration = 0.f;

	/* 타원형을 그리기위한 y radius 배율 */
	float yRadiusRate = 0.3f;

	/* 데미지를 준 Pawn 정보 저장(중복 데미지 처리 방지) */
	std::unordered_set<PawnUniqueIDType> setDamagedPawns;

	/* 현재 범위 원 외곽 타일 위치 */
	std::vector<Craft::Vector2Int> circleOuterTileCoord;

	/* 현재 범위 타일 최소 값 */
	Craft::Vector2Int minTileCoord;

	/* 현재 범위 타일 최대 값 */
	Craft::Vector2Int maxTileCoord;
};

