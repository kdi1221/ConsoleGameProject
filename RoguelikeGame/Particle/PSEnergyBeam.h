#pragma once

#include "Particle/ParticleSystemOnTilemap.h"
#include <Defines/Enums.h>
#include <Math/Vector2Int.h>

/* 에너지빔에 대한 파티클 시스템 */
class PSEnergyBeam : public ParticleSystemOnTilemap
{
	TYPE_DECLARATIONS(PSEnergyBeam, ParticleSystemOnTilemap)

public:
	PSEnergyBeam(const Craft::Vector2Int& endPos, int vertical);
	virtual ~PSEnergyBeam() = default;

public:
	virtual void Initialize() override;

private:
	/* 빔의 진행방향 */
	Craft::eDirection progressDirection = Craft::eDirection::None;

	/* 빔의 수직방향 */
	Craft::eDirection verticalDirection = Craft::eDirection::None;

	/* 빔이 도달할 위치 */
	Craft::Vector2Int endPosition = Craft::Vector2Int::Zero;

	/* 빔의 진행방향 범위 */
	int directionRange = 0;

	/* 빔의 수직방향 범위 */
	int verticalRange = 0;

	
};

