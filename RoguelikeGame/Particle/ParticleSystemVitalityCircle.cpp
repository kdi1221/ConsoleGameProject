#include "ParticleSystemVitalityCircle.h"
#include "Particle/ParticleEmitter.h"
#include "Util/Util.h"
#include "Types/Enums.h"
#include <Math/Define.h>

using namespace Craft;

ParticleSystemVitalityCircle::ParticleSystemVitalityCircle()
	:super(48)
{
	
}

void ParticleSystemVitalityCircle::Initialize()
{
	std::shared_ptr<ParticleEmitter> outerRingEmitter = std::make_shared<ParticleEmitter>();

	/* 이미터에 입자 생성방법을 넘겨줌(Ring 형태) */
	auto SpawnOuterRingElement = [](ParticleSystem& particleSystem)
		{
			/* 극 좌표 기준으로 회전하는 효과 */
			//static const std::wstring elementImages[] = { L"*", L"+", L"#", L"$" };

			for (int angle = 0; angle < 360; angle += 15)
			{
				const float angleRad = DEG_TO_RAD(static_cast<float>(angle));

				FParticleElement addParticleElement;
				addParticleElement.moveMode = eParticleElementMoveMode::Orbit;

				addParticleElement.orbitAngle = angleRad;
				addParticleElement.orbitRadius = 12.f;
				addParticleElement.orbitAngularSpeed = DEG_TO_RAD(25.f);
				addParticleElement.orbitMinRadius = 12.f;
				addParticleElement.orbitMaxRadius = 25.f;
				addParticleElement.orbitRadiusScale = Vector2Float(1.f, 0.6f);

				const Vector2Float initializePos(cos(addParticleElement.orbitAngle) * addParticleElement.orbitRadius * addParticleElement.orbitRadiusScale.x,
												sin(addParticleElement.orbitAngle) * addParticleElement.orbitRadius * addParticleElement.orbitRadiusScale.y);

				addParticleElement.position.x = static_cast<int>(round(initializePos.x));
				addParticleElement.position.y = static_cast<int>(round(initializePos.y));
			
				addParticleElement.noisePosition = Vector2Float::Zero;
				addParticleElement.noiseSpeed = 0.5f;
				addParticleElement.noiseStrength = 15.f;

				addParticleElement.lifeTime = 0.f;

				addParticleElement.drawImages = { L"*", L"+", L"#", L"$" };
				const int selectElementImageIndex = Util::RandomRange(0, static_cast<int>(addParticleElement.drawImages.size()) - 1);
				addParticleElement.image = addParticleElement.drawImages[selectElementImageIndex];
				addParticleElement.drawChangeImageInterval = Util::RandomRange(0.2f, 0.5f);

				addParticleElement.drawColors = { Color::LightGreen, Color::LightGreen, Color::Green, Color::Green, Color::BrightWhite };
				const int selectElementParticleIndex = Util::RandomRange(0, static_cast<int>(addParticleElement.drawColors.size()) - 1);
				addParticleElement.drawColor = addParticleElement.drawColors[selectElementParticleIndex];
				addParticleElement.drawChangeColorInterval = Util::RandomRange(0.2f, 0.5f);

				addParticleElement.renderSortingOrder = static_cast<int>(eRenderSortingOrder::Particle);
				particleSystem.AddParticleElement(std::move(addParticleElement));
			}
		};
	
	outerRingEmitter->SetSpawnElementFunction(SpawnOuterRingElement);
	outerRingEmitter->SetElementSpawnInterval(0.f);
	AddParticleEmitter(outerRingEmitter);


	std::shared_ptr<ParticleEmitter> innerRingEmitter = std::make_shared<ParticleEmitter>();

	auto SpawnInnerRingElement = [](ParticleSystem& particleSystem)
		{
			/* 극 좌표 기준으로 회전하는 효과 */
			static const std::wstring elementImages[] = { L"*", L"+", L"#", L"$" };

			for (int angle = 0; angle < 360; angle += 15)
			{
				const float angleRad = DEG_TO_RAD(static_cast<float>(angle));

				FParticleElement addParticleElement;
				addParticleElement.moveMode = eParticleElementMoveMode::Orbit;

				addParticleElement.orbitAngle = angleRad;
				addParticleElement.orbitRadius = 8.f;
				addParticleElement.orbitAngularSpeed = DEG_TO_RAD(-35.f);
				addParticleElement.orbitMinRadius = 8.f;
				addParticleElement.orbitMaxRadius = 12.f;
				addParticleElement.orbitRadiusScale = Vector2Float(1.f, 0.6f);

				const Vector2Float initializePos(cos(addParticleElement.orbitAngle) * addParticleElement.orbitRadius * addParticleElement.orbitRadiusScale.x,
					sin(addParticleElement.orbitAngle) * addParticleElement.orbitRadius * addParticleElement.orbitRadiusScale.y);

				addParticleElement.position.x = static_cast<int>(round(initializePos.x));
				addParticleElement.position.y = static_cast<int>(round(initializePos.y));

				addParticleElement.noisePosition = Vector2Float::Zero;
				addParticleElement.noiseSpeed = 0.3f;
				addParticleElement.noiseStrength = 5.f;

				addParticleElement.lifeTime = 0.f;

				addParticleElement.drawImages = { L"*", L"+", L"#", L"$" };
				const int selectElementImageIndex = Util::RandomRange(0, static_cast<int>(addParticleElement.drawImages.size()) - 1);
				addParticleElement.image = addParticleElement.drawImages[selectElementImageIndex];
				addParticleElement.drawChangeImageInterval = Util::RandomRange(0.2f, 0.5f);

				addParticleElement.drawColors = { Color::LightBlue, Color::LightBlue, Color::Blue, Color::Blue, Color::BrightWhite };
				const int selectElementParticleIndex = Util::RandomRange(0, static_cast<int>(addParticleElement.drawColors.size()) - 1);
				addParticleElement.drawColor = addParticleElement.drawColors[selectElementParticleIndex];
				addParticleElement.drawChangeColorInterval = Util::RandomRange(0.2f, 0.5f);

				addParticleElement.renderSortingOrder = static_cast<int>(eRenderSortingOrder::Particle);
				particleSystem.AddParticleElement(std::move(addParticleElement));
			}
		};

	innerRingEmitter->SetSpawnElementFunction(SpawnInnerRingElement);
	innerRingEmitter->SetElementSpawnInterval(0.f);
	AddParticleEmitter(innerRingEmitter);

}
