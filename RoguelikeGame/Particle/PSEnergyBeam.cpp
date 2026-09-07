#include "PSEnergyBeam.h"
#include "Particle/ParticleEmitter.h"
#include "Types/Enums.h"
#include <Util/Util.h>

using namespace Craft;

PSEnergyBeam::PSEnergyBeam()
	:super(200)
{
	
}

void PSEnergyBeam::Initialize()
{
	std::shared_ptr<ParticleEmitter> beamBackgroundEmitter = std::make_shared<ParticleEmitter>();

	auto SpawnElementChargingMode = [this](ParticleSystem& particleSystem)
		{
			FParticleElement addParticleElement;
			addParticleElement.moveMode = eParticleElementMoveMode::Velocity;

			Vector2Float targetPosition = Vector2Float::Zero;
			Vector2Float spawnPosition = Vector2Float::Zero;
			switch (progressDirection)
			{
			case eDirection::Left:
				targetPosition = Vector2Float(0.f, Util::RandomRange(0.f, static_cast<float>(verticalRange - 1)));
				spawnPosition = targetPosition + Vector2Float(-Util::RandomRange(1.f, 10.f), 0.f);
				break;

			case eDirection::Right:
				targetPosition = Vector2Float(0.f, -Util::RandomRange(0.f, static_cast<float>(verticalRange - 1)));
				spawnPosition = targetPosition + Vector2Float(Util::RandomRange(1.f, 10.f), 0.f);
				break;

			case eDirection::Top:
				targetPosition = Vector2Float(-Util::RandomRange(0.f, static_cast<float>(verticalRange - 1)), 0.f);
				spawnPosition = targetPosition + Vector2Float(0.f, -Util::RandomRange(1.f, 10.f));
				break;

			case eDirection::Bottom:
				targetPosition = Vector2Float(Util::RandomRange(0.f, static_cast<float>(verticalRange - 1)), 0.f);
				spawnPosition = targetPosition + Vector2Float(0.f, Util::RandomRange(1.f, 10.f));
				break;
			}

			Vector2Float moveDirection = targetPosition - spawnPosition;
			const float distance = moveDirection.Length();
			moveDirection.Normalize();

			const float speed = Util::RandomRange(10.f, 15.f);
			addParticleElement.position = static_cast<Vector2Int>(spawnPosition);
			addParticleElement.velocity = moveDirection * speed;
			addParticleElement.lifeTime = max(0.01f, distance / speed);

			addParticleElement.noiseStrength = Util::RandomRange(2.f, 5.f);
			addParticleElement.noiseSpeed = Util::RandomRange(1.f, 3.f);

			addParticleElement.drawImages = { L"*", L"+", L".", L"·" };
			const int selectElementImageIndex = Util::RandomRange(0, static_cast<int>(addParticleElement.drawImages.size()) - 1);
			addParticleElement.image = addParticleElement.drawImages[selectElementImageIndex];
			addParticleElement.drawChangeImageInterval = Util::RandomRange(0.2f, 0.5f);

			addParticleElement.drawColors = { Color::Purple, Color::LightPurple, Color::Purple, Color::LightPurple, Color::BrightWhite };
			const int selectElementParticleIndex = Util::RandomRange(0, static_cast<int>(addParticleElement.drawColors.size()) - 1);
			addParticleElement.drawColor = addParticleElement.drawColors[selectElementParticleIndex];
			addParticleElement.drawChangeColorInterval = Util::RandomRange(0.2f, 0.5f);

			addParticleElement.renderSortingOrder = static_cast<int>(eRenderSortingOrder::Particle);
			particleSystem.AddParticleElement(std::move(addParticleElement));
		};

	auto SpawnElementExpandMode = [this](ParticleSystem& particleSystem)
		{
			FParticleElement addParticleElement;
			addParticleElement.moveMode = eParticleElementMoveMode::Velocity;

			switch (progressDirection)
			{
			case eDirection::Left:
				addParticleElement.position = Vector2Int(0, Util::RandomRange(0, verticalRange - 1));
				addParticleElement.velocity = Vector2Float(-Util::RandomRange(120.f, 140.f), 0.f);
				addParticleElement.lifeTime = Util::RandomRange(4.f, 6.f);
				addParticleElement.drawImages = { L"*", L"+", L".", L"·", L"-" };

				addParticleElement.noiseStrength = Util::RandomRange(25.f, 35.f);
				break;

			case eDirection::Top:
				addParticleElement.position = Vector2Int(-Util::RandomRange(0, verticalRange - 1), 0);
				addParticleElement.velocity = Vector2Float(0.f, -Util::RandomRange(40.f, 60.f));
				addParticleElement.lifeTime = Util::RandomRange(0.8f, 1.5f);
				addParticleElement.drawImages = { L"*", L"+", L".", L"·", L"|" };

				addParticleElement.noiseStrength = Util::RandomRange(15.f, 20.f);
				break;

			case eDirection::Right:
				addParticleElement.position = Vector2Int(0, -Util::RandomRange(0, verticalRange - 1));
				addParticleElement.velocity = Vector2Float(Util::RandomRange(120.f, 140.f), 0.f);
				addParticleElement.lifeTime = Util::RandomRange(4.f, 6.f);
				addParticleElement.drawImages = { L"*", L"+", L".", L"·", L"-" };

				addParticleElement.noiseStrength = Util::RandomRange(25.f, 35.f);
				break;

			case eDirection::Bottom:
				addParticleElement.position = Vector2Int(Util::RandomRange(0, verticalRange - 1), 0);
				addParticleElement.velocity = Vector2Float(0.f, Util::RandomRange(40.f, 60.f));
				addParticleElement.lifeTime = Util::RandomRange(0.8f, 1.5f);
				addParticleElement.drawImages = { L"*", L"+", L".", L"·", L"|" };

				addParticleElement.noiseStrength = Util::RandomRange(15.f, 20.f);
				break;
			}

			addParticleElement.noisePosition = Vector2Float(Util::RandomRange(0.13f, 0.8f));
			addParticleElement.noiseSpeed = Util::RandomRange(2.f, 5.f);

			const int selectElementImageIndex = Util::RandomRange(0, static_cast<int>(addParticleElement.drawImages.size()) - 1);
			addParticleElement.image = addParticleElement.drawImages[selectElementImageIndex];
			addParticleElement.drawChangeImageInterval = Util::RandomRange(0.2f, 0.5f);

			addParticleElement.drawColors = { Color::Purple, Color::LightPurple, Color::Purple, Color::LightPurple, Color::BrightWhite };
			const int selectElementParticleIndex = Util::RandomRange(0, static_cast<int>(addParticleElement.drawColors.size()) - 1);
			addParticleElement.drawColor = addParticleElement.drawColors[selectElementParticleIndex];
			addParticleElement.drawChangeColorInterval = Util::RandomRange(0.2f, 0.5f);

			addParticleElement.renderSortingOrder = static_cast<int>(eRenderSortingOrder::Particle);
			particleSystem.AddParticleElement(std::move(addParticleElement));
		};

	/* 빔의 백그라운드 효과 표시 */
	auto SpawnBackgroundElement = [this, SpawnElementChargingMode, SpawnElementExpandMode](ParticleSystem& particleSystem)
		{	
			switch (currentExpandMode)
			{
			case eBeamExpandMode::Charging:
				{
					SpawnElementChargingMode(particleSystem);
				}
				break;

			case eBeamExpandMode::Expand:
				{
					SpawnElementExpandMode(particleSystem);
				}
				break;
			}
			
		};

	beamBackgroundEmitter->SetSpawnElementFunction(SpawnBackgroundElement);
	beamBackgroundEmitter->SetElementSpawnInterval(0.1f);
	AddParticleEmitter(beamBackgroundEmitter);
}

void PSEnergyBeam::SetCurrentBeamExpandMode(eBeamExpandMode expandMode)
{
	currentExpandMode = expandMode;
}

void PSEnergyBeam::SetBeamProgressDirection(eDirection direction)
{
	progressDirection = direction;
}

void PSEnergyBeam::SetBeamRange(int range)
{
	currentRange = range;
}

void PSEnergyBeam::SetBeamVerticalRange(int range)
{
	verticalRange = range;
}
