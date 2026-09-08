#include "FXExplosionActor.h"
#include "Particle/PSFireBallExplosion.h"
#include <Component/ParticleComponent.h>
#include <cassert>

using namespace Craft;

FXExplosionActor::FXExplosionActor(const Vector2Int& inPosition, float range, float limitTime)
	:super(inPosition)
	,explosionRange(range)
{
	timerLifeSpan.SetTargetTime(limitTime);

	/* 파티클 컴포넌트 */
	particleComponent = AddComponent<ParticleComponent>();
}

FXExplosionActor::~FXExplosionActor()
{

}

void FXExplosionActor::BeginPlay()
{
	super::BeginPlay();

	/* 파티클 시스템 : 폭발 효과 표시 */
	assert(particleComponent && "Invalid particleComponent");
	std::shared_ptr<PSFireBallExplosion> spawnedParticleSystem = std::make_shared<PSFireBallExplosion>();
	//spawnedParticleSystem->SetBeamRange(static_cast<int>(currentRange));
	//spawnedParticleSystem->SetCurrentBeamExpandMode(beamExpandMode);
	//spawnedParticleSystem->SetBeamProgressDirection(progressDirection);
	//spawnedParticleSystem->SetBeamVerticalRange(beamVerticalRange);
	spawnedParticleSystem->Initialize();
	particleComponent->AddParticleSystem(spawnedParticleSystem);
}

void FXExplosionActor::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	timerLifeSpan.Tick(deltaTime);
	if (timerLifeSpan.IsTimeOut())
	{
		/* 수명이 다하면 Destroy */
		Destroy();
	}
}
