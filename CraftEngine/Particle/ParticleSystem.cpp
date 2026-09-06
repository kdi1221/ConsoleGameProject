#include "ParticleSystem.h"
#include "Actor/Actor.h"
#include "Particle/ParticleEmitter.h"
#include "Render/Renderer.h"
#include "StaticLibrary/StaticFunctionLibrary.h"

#include "Util/Util.h"

namespace Craft
{
	ParticleSystem::ParticleSystem(const size_t elementsMaxSize)
	{
		particleElements.resize(elementsMaxSize);
	}

	void ParticleSystem::Initialize()
	{

	}

	void ParticleSystem::Tick(float deltaTime)
	{
		/* 파티클 생성기 업데이트 */
		UpdateParticleEmitters(deltaTime);

		/* 파티클 입자 업데이트 */
		UpdateParticleElements(deltaTime);
	}

	void ParticleSystem::Draw()
	{
		/* 파티클 입자 그리기 */
		DrawParticleElements();
	}

	void ParticleSystem::SetCurrentLevel(std::shared_ptr<Level> level)
	{
		currentLevel = level;
	}

	void ParticleSystem::SetCenterPosition(const Vector2Int& position)
	{
		centerPosition = position;
	}

	bool ParticleSystem::AddParticleElement(FParticleElement&& particleElement)
	{
		/* 현재 입자 버퍼갯수가 초과되었으면 더이상 추가할 수 없음 */
		if (activateElementNum >= particleElements.size())
		{
			return false;
		}

		particleElements[activateElementNum++] = std::move(particleElement);

		return true;
	}

	void ParticleSystem::AddParticleEmitter(std::shared_ptr<ParticleEmitter> emitter)
	{
		if (!emitter)
		{
			return;
		}

		/* owner 지정 */
		emitter->SetOwnerParticleSystem(shared_from_this());

		//입자 생성기 추가 
		particleEmitters.emplace_back(emitter);
	}

	bool ParticleSystem::IsDrawParticleElement(const FParticleElement& element, const Vector2Int& drawPosition) const
	{
		return true;
	}

	std::shared_ptr<Level> ParticleSystem::GetCurrentLevel() const
	{
		return currentLevel.lock();
	}

	void ParticleSystem::UpdateParticleEmitters(float deltaTime)
	{
		for (std::shared_ptr<ParticleEmitter>& emitter : particleEmitters)
		{
			emitter->Tick(deltaTime);
		}
	}

	void ParticleSystem::UpdateParticleElements(float deltaTime)
	{
		/* 활성화된 입자들만 업데이트 */
		for (size_t elementIndex = 0; elementIndex < activateElementNum; )
		{
			FParticleElement& particleElement = particleElements[elementIndex];

			//경과시간 계산
			particleElement.duration += deltaTime;

			//lifeTime이 0이하이면 무한수명으로 간주
			if (particleElement.lifeTime > 0.f)
			{
				//생명주기 계산
				if (particleElement.duration >= particleElement.lifeTime)
				{
					/* 마지막 활성화 입자와 위치 교환 */
					std::swap(particleElements[elementIndex], particleElements[--activateElementNum]);

					/* 인덱스 증가 없이 바로 다음으로 진행 */
					continue;
				}
			}

			switch (particleElement.moveMode)
			{
			case eParticleElementMoveMode::Velocity:
				UpdateParticleElementVelocity(particleElement, deltaTime);
				break;

			case eParticleElementMoveMode::Orbit:
				UpdateParticleElementOrbit(particleElement, deltaTime);
				break;
			}

			UpdateParticleElementImage(particleElement, deltaTime);
			UpdateParticleElementColor(particleElement, deltaTime);

			++elementIndex;
		}
	}

	void ParticleSystem::UpdateParticleElementVelocity(FParticleElement& particleElement, float deltaTime)
	{
		Vector2Float noiseVelocity = Vector2Float::Zero;
		if (particleElement.noiseStrength > 0.f)
		{
			//noise 적용
			const float noisePosX = particleElement.noisePosition.x + particleElement.duration * particleElement.noiseSpeed;
			const float noiseValue = StaticFunctionLibrary::perlin(noisePosX, particleElement.noisePosition.y);

			Vector2Float direction = particleElement.velocity;
			direction.Normalize();

			const Vector2Float sideDirection(-direction.y, direction.x);
			noiseVelocity = sideDirection * noiseValue * particleElement.noiseStrength;
		}

		// velocity 적용
		particleElement.moveDeltaValue += (particleElement.velocity + noiseVelocity) * deltaTime;

		const float absMoveDeltaX = abs(particleElement.moveDeltaValue.x);
		if (absMoveDeltaX >= 1.f)
		{
			const int addX = static_cast<int>(absMoveDeltaX) * (particleElement.moveDeltaValue.x > 0.f ? 1 : -1);
			particleElement.position.x += addX;
			particleElement.moveDeltaValue.x -= static_cast<float>(addX);
		}

		const float absMoveDeltaY = abs(particleElement.moveDeltaValue.y);
		if (absMoveDeltaY >= 1.f)
		{
			const int addY = static_cast<int>(absMoveDeltaY) * (particleElement.moveDeltaValue.y > 0.f ? 1 : -1);
			particleElement.position.y += addY;
			particleElement.moveDeltaValue.y -= static_cast<float>(addY);
		}
	}

	void ParticleSystem::UpdateParticleElementOrbit(FParticleElement& particleElement, float deltaTime)
	{
		float radius = particleElement.orbitRadius;

		//noise 적용
		if (particleElement.noiseStrength > 0.f)
		{
			const float noisePosX = particleElement.noisePosition.x + particleElement.duration * particleElement.noiseSpeed;
			const float noiseValue = StaticFunctionLibrary::perlin(noisePosX, particleElement.noisePosition.y);

			radius += (noiseValue * particleElement.noiseStrength);
		}

		//반경 제한
		if (particleElement.orbitMaxRadius > 0.f)
		{
			radius = min(radius, particleElement.orbitMaxRadius);
		}
		
		radius = max(radius, particleElement.orbitMinRadius);

		/* Angle 변경 */
		particleElement.orbitAngle += particleElement.orbitAngularSpeed * deltaTime;

		/* 위치 계산 */
		const Vector2Float currentPos(cos(particleElement.orbitAngle) * radius * particleElement.orbitRadiusScale.x,
			sin(particleElement.orbitAngle) * radius * particleElement.orbitRadiusScale.y);

		//const Vector2Float currentPos = Vector2Float(cos(particleElement.orbitAngle), sin(particleElement.orbitAngle)) * radius;

		particleElement.position.x = static_cast<int>(round(currentPos.x));
		particleElement.position.y = static_cast<int>(round(currentPos.y));
	}

	void ParticleSystem::UpdateParticleElementImage(FParticleElement& particleElement, float deltaTime)
	{
		if (particleElement.drawImages.empty() || 
			particleElement.drawChangeImageInterval <= 0.f)
		{
			return;
		}

		particleElement.drawChangeImageElapsedTime += deltaTime;
		if (particleElement.drawChangeImageElapsedTime >= particleElement.drawChangeImageInterval)
		{
			particleElement.drawChangeImageElapsedTime -= particleElement.drawChangeImageInterval;

			const int selectIndex = Util::RandomRange(0, static_cast<int>(particleElement.drawImages.size()) - 1);
			particleElement.image = particleElement.drawImages[selectIndex];
		}
	}

	void ParticleSystem::UpdateParticleElementColor(FParticleElement& particleElement, float deltaTime)
	{
		if (particleElement.drawColors.empty() ||
			particleElement.drawChangeColorInterval <= 0.f)
		{
			return;
		}

		particleElement.drawChangeColorElapsedTime += deltaTime;
		if (particleElement.drawChangeColorElapsedTime >= particleElement.drawChangeColorInterval)
		{
			particleElement.drawChangeColorElapsedTime -= particleElement.drawChangeColorInterval;

			const int selectIndex = Util::RandomRange(0, static_cast<int>(particleElement.drawColors.size()) - 1);
			particleElement.drawColor = particleElement.drawColors[selectIndex];
		}
	}

	void ParticleSystem::DrawParticleElements()
	{
		Renderer& renderer = Renderer::Get();

		for (size_t elementIndex = 0; elementIndex < activateElementNum; ++elementIndex)
		{
			FParticleElement& particleElement = particleElements[elementIndex];

			const Vector2Int drawPosition = centerPosition + particleElement.position;

			if (!IsDrawParticleElement(particleElement, drawPosition))
			{
				continue;
			}

			renderer.Submit(particleElement.image, drawPosition, particleElement.drawColor, particleElement.renderSortingOrder);
		}
	}
}