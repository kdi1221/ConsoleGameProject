#pragma once

#include "Particle/ParticleSystemOnTilemap.h"

namespace Craft
{
	struct FParticleElement;
	class Vector2Int;
}

/* 파이어볼 폭발 시 표시되는 파티클 시스템 */
class PSFireBallExplosion : public ParticleSystemOnTilemap
{
	TYPE_DECLARATIONS(PSFireBallExplosion, ParticleSystemOnTilemap)

public:
	PSFireBallExplosion();
	virtual ~PSFireBallExplosion() = default;

public:
	virtual void Initialize() override;
	virtual bool ShouldRemoveParticleElement(const Craft::FParticleElement& element, const Craft::Vector2Int& worldPosition) const;
};

