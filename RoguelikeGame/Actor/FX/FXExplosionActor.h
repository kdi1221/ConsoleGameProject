#pragma once

#include <Util/Timer.h>
#include <Actor/Actor.h>

namespace Craft
{
	class ParticleComponent;
}

class FXExplosionActor : public Craft::Actor
{
	TYPE_DECLARATIONS(FXExplosionActor, Actor)

public:
	FXExplosionActor(const Craft::Vector2Int& inPosition, float range, float limitTime);
	virtual ~FXExplosionActor();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

private:
	float explosionRange = 0.f;
	Timer timerLifeSpan;

private:
	/* 파티클 컴포넌트 */
	std::shared_ptr<Craft::ParticleComponent> particleComponent;
};

