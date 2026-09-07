#pragma once

#include "Particle/ParticleSystemOnTilemap.h"
#include "ParticleDefines.h"
#include <Defines/Enums.h>
#include <Math/Vector2Int.h>

/* 에너지빔에 대한 파티클 시스템 */
class PSEnergyBeam : public ParticleSystemOnTilemap
{
	TYPE_DECLARATIONS(PSEnergyBeam, ParticleSystemOnTilemap)

public:
	PSEnergyBeam();
	virtual ~PSEnergyBeam() = default;

public:
	virtual void Initialize() override;

public:
	void SetCurrentBeamExpandMode(eBeamExpandMode expandMode);
	void SetBeamProgressDirection(Craft::eDirection direction);
	void SetBeamRange(int range);
	void SetBeamVerticalRange(int range);

private:
	/* 현재 빔의 확장 상태 */
	eBeamExpandMode currentExpandMode = eBeamExpandMode::None;

	/* 빔의 진행방향 */
	Craft::eDirection progressDirection = Craft::eDirection::None;

	/* 빔의 범위 */
	int currentRange = 0;

	/* 빔의 수직방향의 범위 */
	int verticalRange = 0;

};

