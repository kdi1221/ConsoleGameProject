#pragma once

#include "Particle/ParticleSystem.h"

namespace Craft
{
	struct FParticleElement;
	class Vector2Int;
}

/* 타일맵 위에 표시되는 모든 파티클 시스템의 베이스 클래스  */
class ParticleSystemOnTilemap : public Craft::ParticleSystem
{
	TYPE_DECLARATIONS(ParticleSystemOnTilemap, ParticleSystem)

public:
	ParticleSystemOnTilemap(const size_t elementMaxSize);
	virtual ~ParticleSystemOnTilemap() = default;

private:
	virtual bool IsDrawParticleElement(const Craft::FParticleElement& element, const Craft::Vector2Int& drawPosition) const;
};

