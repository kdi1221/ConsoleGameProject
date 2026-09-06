#pragma once

#include "Core/CraftObject.h"
#include <functional>

namespace Craft
{
	class ParticleSystem;

	/* 파티클 입자 생성기 */
	class CRAFT_API ParticleEmitter : public CraftObject
	{
		TYPE_DECLARATIONS(ParticleEmitter, CraftObject)

	public:
		using SpawnElementFunction = std::function<void(ParticleSystem& particleSystem)>;

	public:
		ParticleEmitter();
		virtual ~ParticleEmitter();

	public:
		void Tick(float deltaTime);

	public:
		/* Owner 지정 */
		void SetOwnerParticleSystem(std::shared_ptr<ParticleSystem> particleSystem);

		/* 입자 생성 방법 지정 */
		void SetSpawnElementFunction(SpawnElementFunction spawnFunction);

		/* 입자 생성 간격 지정 */
		void SetElementSpawnInterval(float interval);

	private:
		/* 주기별 파티클 입자 생성 */
		void SpawnParticleElements();

	private:
		/* spawnInterval이 0일때 한번만 생성 */
		bool bSpawnedElement = false;

		/* 입자 생성 간격 */
		float spawnInterval = 0.f;

		/* 입자 생성 타이머 */
		float spawnElapsedTime = 0.f;

	private:
		/* Particle 입자 생성 방법 정의 함수 */
		SpawnElementFunction spawnElementFunction;

		/* Owner Particle System */
		std::weak_ptr<ParticleSystem> ownerParticleSystem;
	};
}


