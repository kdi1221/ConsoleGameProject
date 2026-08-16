#pragma once

#include "AbilityObject.h"
#include "Types/Enums.h"
#include <Math/Vector2Int.h>
#include <Math/Vector2Float.h>
#include <Math/Color.h>
#include <unordered_set>
#include <Util/Timer.h>

class AbilityBeam : public AbilityObject
{
	TYPE_DECLARATIONS(AbilityBeam, AbilityObject)

private:
	enum class eBeamMode : unsigned int
	{
		Growth,
		Keep,
		lull,
		Max
	};

public:
	AbilityBeam(int level, eTeamID teamID);

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	/* Ability 트리거 켜기 */
	virtual void TriggerOn() override;

	/* Ability 트리거 끄기 */
	virtual void TriggerOff() override;

private:
	void BuildBeamPaths(const Craft::Vector2Int& startPos);
	void ChangeDisplayColor();
	void DamageProcess();


private:
	/* 최대 사정거리 */
	float beamMaxRange = 6.f;

	/* 현재 사정거리 */
	float beamCurrentRange = 0.f;

	/* 행위자의 Team ID*/
	eTeamID instigatorTeamID = eTeamID::None;

	/* 현재 빔의 경로 */
	std::unordered_set<Craft::Vector2Int> beamPaths;

	/* 직전 빔의 경로를 생성했을 때 정보들 */
	Craft::Vector2Int prevBeamPathStartPos = Craft::Vector2Int(-1, -1);

	/* 색상 타이머 */
	Timer timerColorUpdate;

	/* 길이 증감 관련 타이머들 */
	Timer timerRangeUpdate[static_cast<unsigned int>(eBeamMode::Max)];

	/* 현재 표시 색상 인덱스 */
	int displayColorIndex = 0;

	/* 현재 Beam Mode */
	eBeamMode beamMode = eBeamMode::Growth;

	/* 데미지 수치 */
	float damageValue = 6.f;

	/* 마지막 입력 방향 */
	Craft::Vector2Float lastInputDirection = Craft::Vector2Float::Zero;
};

