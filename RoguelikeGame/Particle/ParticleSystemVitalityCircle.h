#pragma once

#include "Particle/ParticleSystemOnTilemap.h"

/* 원기회복마법진 효과 */
class ParticleSystemVitalityCircle : public ParticleSystemOnTilemap
{
	TYPE_DECLARATIONS(ParticleSystemVitalityCircle, ParticleSystemOnTilemap)

public:
	ParticleSystemVitalityCircle();
	virtual ~ParticleSystemVitalityCircle() = default;

public:
	virtual void Initialize() override;
};

