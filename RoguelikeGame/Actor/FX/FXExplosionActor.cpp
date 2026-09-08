#include "FXExplosionActor.h"
#include "Particle/PSFireBallExplosion.h"
#include <Component/ParticleComponent.h>
#include <Engine/Engine.h>
#include <cassert>
#include <Util/Util.h>

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
	spawnedParticleSystem->Initialize();
	particleComponent->AddParticleSystem(spawnedParticleSystem);

	/* 폭발 사운드 재생 */
	const int randomHitSoundIndex = Util::RandomRange(1, 3);
	std::string hitSoundName = "Effect/boom" + std::to_string(randomHitSoundIndex) + ".wav";
	Engine::Get().PlayOneShot(hitSoundName);
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
