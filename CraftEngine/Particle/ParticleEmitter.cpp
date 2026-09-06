#include "ParticleEmitter.h"
#include "Util/Util.h"
#include "Math/Define.h"
#include "ParticleSystem.h"

namespace Craft
{
	ParticleEmitter::ParticleEmitter()
	{

	}

	ParticleEmitter::~ParticleEmitter()
	{

	}

	void ParticleEmitter::Tick(float deltaTime)
	{
		/* spawnInterval이 0이면 최초 한번만 생성 */
		if (spawnInterval <= 0.f)
		{
			if (!bSpawnedElement)
			{
				SpawnParticleElements();
				bSpawnedElement = true;
			}

			return;
		}

		/* 그게 아니면 반복 생성 */
		spawnElapsedTime += deltaTime;
		if (spawnElapsedTime >= spawnInterval)
		{
			spawnElapsedTime -= spawnInterval;

			SpawnParticleElements();
		}
	}

	void ParticleEmitter::SetOwnerParticleSystem(std::shared_ptr<ParticleSystem> particleSystem)
	{
		ownerParticleSystem = particleSystem;
	}

	void ParticleEmitter::SetSpawnElementFunction(SpawnElementFunction spawnFunction)
	{
		spawnElementFunction = spawnFunction;
	}

	void ParticleEmitter::SetElementSpawnInterval(float interval)
	{
		spawnInterval = interval;
	}

	void ParticleEmitter::SpawnParticleElements()
	{
		if (!spawnElementFunction)
		{
			return;
		}

		std::shared_ptr<ParticleSystem> particleSystem = ownerParticleSystem.lock();
		if (!particleSystem)
		{
			return;
		}

		spawnElementFunction(*particleSystem);

		/* 테스트 : 360도 원형 발사 */
		/*for (int angle = 0; angle < 360; angle += 15)
		{
			const float angleRad = DEG_TO_RAD(static_cast<float>(angle));

			FParticleElement addParticleElement;
			addParticleElement.moveMode = eParticleElementMoveMode::Velocity;
			addParticleElement.position = Vector2Int::Zero;
			addParticleElement.velocity = Vector2Float(static_cast<float>(cos(angleRad)), static_cast<float>(sin(angleRad))) * 10.f;
			addParticleElement.lifeTime = 5.f;
			addParticleElement.image = L"*";
			addParticleElement.drawColor = Color::LightGreen;
			addParticleElement.renderSortingOrder = static_cast<int>(elementRenderSortOrder);
			particleComponent->AddParticleElement(std::move(addParticleElement));
		}*/

		/*for (int i = 0; i < 3; ++i)
		{
			const float angleRad = DEG_TO_RAD(Util::RandomRange(0.f, 360.f));

			FParticleElement addParticleElement;
			addParticleElement.moveMode = eParticleElementMoveMode::Velocity;
			addParticleElement.position = Vector2Int::Zero;
			addParticleElement.velocity = Vector2Float(static_cast<float>(cos(angleRad)), static_cast<float>(sin(angleRad))) * 10.f;
			addParticleElement.lifeTime = 5.f;
			addParticleElement.image = L"*";
			addParticleElement.drawColor = Color::LightGreen;
			addParticleElement.renderSortingOrder = static_cast<int>(elementRenderSortOrder);
			particleComponent->AddParticleElement(std::move(addParticleElement));
		}*/

		/* 테스트 : 360도 원형 발사인데 살짝 노이즈를 끼얹는거.. */
		/*for (int angle = 0; angle < 360; angle += 15)
		{
			const float angleRad = DEG_TO_RAD(static_cast<float>(angle));

			FParticleElement addParticleElement;
			addParticleElement.moveMode = eParticleElementMoveMode::Velocity;
			addParticleElement.position = Vector2Int::Zero;
			addParticleElement.velocity = Vector2Float(static_cast<float>(cos(angleRad)), static_cast<float>(sin(angleRad))) * 10.f;
			addParticleElement.noisePosition = Vector2Float(static_cast<float>(angle) * 0.17f,
															static_cast<float>(angle) * 0.31f);
			addParticleElement.noiseSpeed = 8.f;
			addParticleElement.noiseStrength = 15.f;
			addParticleElement.lifeTime = 5.f;
			addParticleElement.image = L"*";
			addParticleElement.drawColor = Color::LightGreen;
			addParticleElement.renderSortingOrder = static_cast<int>(elementRenderSortOrder);
			particleComponent->AddParticleElement(std::move(addParticleElement));
		}*/
	}
}