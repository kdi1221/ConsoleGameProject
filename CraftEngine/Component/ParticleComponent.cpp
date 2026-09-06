#include "ParticleComponent.h"
#include "Actor/Actor.h"
#include "Particle/ParticleSystem.h"

namespace Craft
{
	ParticleComponent::ParticleComponent()
		:super()
	{
		
	}

	void ParticleComponent::Tick(float deltaTime)
	{
		super::Tick(deltaTime);

		/* 파티클 시스템 업데이트 */
		UpdateParticleSystem(deltaTime);
	}

	void ParticleComponent::PostTick(float deltaTime)
	{
		super::PostTick(deltaTime);

		/* 파티클 시스템에 현재 월드 위치 알림 */
		UpdateParticleSystemPosition();
	}

	void ParticleComponent::Draw()
	{
		super::Draw();

		/* 파티클 시스템 그리기 */
		DrawParticleSystem();
	}

	void ParticleComponent::AddParticleSystem(std::shared_ptr<ParticleSystem> particleSystem)
	{
		if (!particleSystem)
		{
			return;
		}

		particleSystem->SetCurrentLevel(GetOwnerLevel());
		particleSystems.emplace_back(particleSystem);
	}

	void ParticleComponent::UpdateParticleSystem(float deltaTime)
	{
		for (std::shared_ptr<ParticleSystem>& particleSystem : particleSystems)
		{
			particleSystem->Tick(deltaTime);
		}
	}

	void ParticleComponent::UpdateParticleSystemPosition()
	{
		std::shared_ptr<Actor> ownerActor = GetOwner();
		if (!ownerActor)
		{
			return;
		}

		const Vector2Int& centerPostion = ownerActor->GetWorldPosition();

		for (std::shared_ptr<ParticleSystem>& particleSystem : particleSystems)
		{
			particleSystem->SetCenterPosition(centerPostion);
		}
	}

	void ParticleComponent::DrawParticleSystem()
	{
		for (std::shared_ptr<ParticleSystem>& particleSystem : particleSystems)
		{
			particleSystem->Draw();
		}
	}

	std::shared_ptr<Level> ParticleComponent::GetOwnerLevel() const
	{
		std::shared_ptr<Actor> ownerActor = GetOwner();

		return ownerActor ? ownerActor->GetOwner() : nullptr;
	}
}