#pragma once

#include "Component.h"
#include <vector>

namespace Craft
{
	class Level;
	class ParticleSystem;

	/* 파티클 시스템 관리 컴포넌트 */
	class CRAFT_API ParticleComponent : public Component
	{
		TYPE_DECLARATIONS(ParticleComponent, Component)

	public:
		ParticleComponent();
		virtual ~ParticleComponent() = default;

	private:
		virtual void BeginPlay() override;
		virtual void Tick(float deltaTime) override;
		virtual void PostTick(float deltaTime) override;
		virtual void Draw() override;

	public:
		/* 파티클 시스템 추가 */
		void AddParticleSystem(std::shared_ptr<ParticleSystem> particleSystem);

	private:
		/* 파티클 시스템 업데이트 */
		void UpdateParticleSystem(float delaTime);

		/* 파티클 시스템 기준 위치 갱신 */
		void UpdateParticleSystemPosition();

		/* 파티클 시스템 그리기 */
		void DrawParticleSystem();

	private:
		/* owner의 level 반환 */
		std::shared_ptr<Level> GetOwnerLevel() const;

	private:
		/* 파티클 시스템 리스트 */
		std::vector<std::shared_ptr<ParticleSystem>> particleSystems;
	};
}

