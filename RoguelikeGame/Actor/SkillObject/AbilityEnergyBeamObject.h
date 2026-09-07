#pragma once

#include "SkillObjectActor.h"
#include "Particle/ParticleDefines.h"
#include "Types/Enums.h"
#include <Util/Timer.h>
#include <Math/Vector2Int.h>
#include <Defines/Enums.h>
#include <string>
#include <vector>

namespace Craft
{
	class ParticleComponent;
}

class PSEnergyBeam;

/* 보스 Energy Beam Actor */
class AbilityEnergyBeamObject : public SkillObject
{
	TYPE_DECLARATIONS(AbilityEnergyBeamObject, SkillObject)

public:
	using OnDestroyBeamCallbackType = std::function<void(const AbilityEnergyBeamObject&)>;

public:
	AbilityEnergyBeamObject(const Craft::Vector2Int& position,
							const Craft::Vector2Int& endPos,
							int range);
	virtual ~AbilityEnergyBeamObject() = default;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void PostTick(float deltaTime) override;
	virtual void Draw() override;
	virtual void Destroy() override;

public:
	void SetInstigatorTeamID(eTeamID teamID);
	void SetIndex(int index);
	void SetOnDestroyBeamCallback(OnDestroyBeamCallbackType callback);
	void SetExpandDuration(float duration);
	void SetDamageValue(float amount);
	void SetDamageProcessInterval(float interval);

private:
	/* 빔 영역 증가 */
	void ExpandBeamRect(float deltaTime);

	/* 빔 영역 그리기 */
	void DrawBeamRect();

	/* 빔 시작 위치 업데이트 */
	void UpdateBeamStartPosition();

	/* 현재 빔 모드 변경 */
	void SetCurrentExpandMode(eBeamExpandMode current);

	/* 다음 빔 확장 모드 변경 */
	void SetNextExpandMode(eBeamExpandMode nextMode, float delay);

	/* 영역안의 타겟들에게 데미지 */
	void ApplyDamage();

	/* 이펙트 사운드 재생 */
	void PlayFXSound();

public:
	inline int GetBeamIndex() const { return beamIndex; }

private:
	/* 주기당 입힐 데미지 수치 */
	float damageAmount = 0.f;

	/* 데미지 처리 주기 타이머 */
	Timer timerDamageProcessInterval;

private:
	/* 출력할 빔 이미지 버퍼 */
	std::vector<std::wstring> drawBeamImageBuffer;

	/* 출력할 위치 */
	Craft::Vector2Int drawStartPosition = Craft::Vector2Int::Zero;

	/* 최소 확장 범위 */
	size_t minRange = 1;

	/* 현재 확장된 범위 */
	size_t currentRange = 1;

	/* 최대 확장 범위 */
	size_t maxRange = 0;

	/* 빔의 진행방향 */
	Craft::eDirection progressDirection = Craft::eDirection::None;

	/* 빔이 목적지까지 확장하는데 걸리는 시간 */
	float expandDuration = 0.f;

	/* 현재까지 확장하는데 소요된 시간 */
	float expandElapsedTime = 0.f;

	/* 현재 빔 확장 모드 */
	eBeamExpandMode beamExpandMode = eBeamExpandMode::None;

	/* 다음 빔 확장 모드 */
	eBeamExpandMode nextBeamExpandMode = eBeamExpandMode::None;

	/* 다음 번 빔 확장 모드 변경 딜레이 타이머 */
	Timer timerNextBeamExpandModeDelay;

private:
	/* 시전자의 Team ID */
	eTeamID instigatorTeamID = eTeamID::None;

	/* 도달해야할 끝 위치 */
	Craft::Vector2Int beamEndPosition = Craft::Vector2Int::Zero;

	/* 다른 에너지빔 액터와 구분할 인덱스 */
	int beamIndex = 0;

	/* Destroy될때 호출되는 콜백 */
	OnDestroyBeamCallbackType onDestroyBeamCallback;

	/* 에너지 빔의 수직 범위 */
	int beamVerticalRange = 0;


private:
	/* 파티클 컴포넌트 */
	std::shared_ptr<Craft::ParticleComponent> particleComponent;

	/* 빔 표시 파티클 시스템 */
	std::weak_ptr<PSEnergyBeam> psEnergyBeam;
};

