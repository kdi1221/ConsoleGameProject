#pragma once

#include "Types/Enums.h"
#include "Types/Defines.h"
#include "SkillObjectActor.h"
#include <Math/Color.h>
#include <Math/Vector2Int.h>
#include <Util/Timer.h>
#include <unordered_set>
#include <vector>
#include <string>

class TilemapLevel;

/* 플레이어 노바 스킬 사용시 스폰되는 Actor */
class AbilityNovaActor : public SkillObject
{
	TYPE_DECLARATIONS(AbilityNovaActor, SkillObject)

public:
	AbilityNovaActor(const Craft::Vector2Int& position, eTeamID TeamID, float extendSpeed, float beginRange, float limitRange, float damage);
	virtual ~AbilityNovaActor() = default;

public:
	virtual void Tick(float deltaTime) override;
	virtual void PostTick(float deltaTime) override;
	virtual void Draw() override;

private:
	/* 시간에 따라 노바의 범위 확장 */
	void UpdateRangeCircle(float deltaTime);

	/* 해당 위치의 타일이 노바 범위 내 유효한지 체크 */
	bool IsValidTileCoordInCircle(const TilemapLevel& level, const Craft::Vector2Int& tileCoord) const;

private:
	/* 노바 외곽선 표시 이미지 */
	std::wstring outlineImage = L" ";

	/* 노바 외곽선 이미지 색상 */
	Craft::Color outlineColor = Craft::Color::BG_LightYellow;

	/* 현재 노바 범위 원 외곽 타일 위치 */
	std::vector<Craft::Vector2Int> circleOuterTileCoord;

	/* 현재 노바 범위 원 내부 타일 위치 */
	std::vector<Craft::Vector2Int> circleInnerTileCoord;

	/* 시전자의 Team ID */
	eTeamID instigatorTeamID = eTeamID::None;

	/* 현재 노바 범위 */
	float currentRange = 0.f;

	/* 노바 확장 속도 */
	float extendSpeed = 0.f;

	/* 노바 최대 범위 */
	float limitRange = 0.f;

	/* 노바 데미지 */
	float damageAmount = 0.f;

	/* 데미지를 준 Pawn 정보 저장(중복 데미지 처리 방지) */
	std::unordered_set<PawnUniqueIDType> setDamagedPawns;
};

