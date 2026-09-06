#pragma once

#include "Core/CraftObject.h"
#include "Particle/ParticleElement.h"
#include <vector>

namespace Craft
{
	class Level;
	class ParticleEmitter;

	/* 파티클 시스템, 입자생성기 및 입자를 관리한다. */
	class CRAFT_API ParticleSystem : public CraftObject
									, public std::enable_shared_from_this<ParticleSystem>
	{
		TYPE_DECLARATIONS(ParticleSystem, CraftObject)

	public:
		ParticleSystem(const size_t elementsMaxSize = 64);
		virtual ~ParticleSystem() = default;

	public:
		virtual void Initialize();
		virtual void Tick(float deltaTime);
		virtual void Draw();

	public:
		/* 현재 위치한 레벨 지정 */
		void SetCurrentLevel(std::shared_ptr<Level> level);

		/* 기준 위치 지정 */
		void SetCenterPosition(const Vector2Int& position);

		/* 파티클 입자 정보 추가 */
		bool AddParticleElement(FParticleElement&& particleElement);

		/* 입자 생성기 추가 */
		void AddParticleEmitter(std::shared_ptr<ParticleEmitter> emitter);

	protected:
		virtual bool IsDrawParticleElement(const FParticleElement& element, const Vector2Int& drawPosition) const;

	protected:
		std::shared_ptr<Level> GetCurrentLevel() const;

	private:
		/* 파티클 입자 생성기 업데이트 */
		void UpdateParticleEmitters(float deltaTime);

		/* 파티클 입자 업데이트 */
		void UpdateParticleElements(float deltaTime);

		/* 파티클 입자 업데이트 - Velocity 기반 */
		void UpdateParticleElementVelocity(FParticleElement& particleElement, float deltaTime);

		/* 파티클 입자 업데이트 - Orbit 기반 */
		void UpdateParticleElementOrbit(FParticleElement& particleElement, float deltaTime);

		/* 파티클 입자 업데이트 - Draw Image */
		void UpdateParticleElementImage(FParticleElement& particleElement, float deltaTime);

		/* 파티클 입자 업데이트 - Draw Color */
		void UpdateParticleElementColor(FParticleElement& particleElement, float deltaTime);

		/* 파티클 입자 그리기 */
		void DrawParticleElements();

	private:
		/* 기준 위치 */
		Vector2Int centerPosition = Vector2Int::Zero;

		/* 파티클 입자 생성기 리스트 */
		std::vector<std::shared_ptr<ParticleEmitter>> particleEmitters;

		/* 파티클 입자 리스트 */
		std::vector<FParticleElement> particleElements;

		/* 파티클 현재 활성화 갯수 */
		size_t activateElementNum = 0;

		/* 현재 위치한 레벨 */
		std::weak_ptr<Level> currentLevel;
	};
}


