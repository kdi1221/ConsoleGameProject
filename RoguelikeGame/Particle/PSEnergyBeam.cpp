#include "PSEnergyBeam.h"
#include "Particle/ParticleEmitter.h"
#include "Types/Enums.h"

using namespace Craft;

PSEnergyBeam::PSEnergyBeam(const Craft::Vector2Int& endPos, int vertical)
	:super((static_cast<size_t>(endPos.LengthSqrt()) * vertical) * 2)
	,endPosition(endPos)
	,verticalRange(vertical)
{
	if (abs(endPosition.x) > 0)
	{
		progressDirection = endPosition.x > 0 ? eDirection::Right : eDirection::Left;
		verticalDirection = (progressDirection == eDirection::Right) ? eDirection::Top : eDirection::Bottom;
	}
	else if (abs(endPosition.y) > 0)
	{
		progressDirection = endPosition.y > 0 ? eDirection::Bottom : eDirection::Top;
		verticalDirection = (progressDirection == eDirection::Bottom) ? eDirection::Right : eDirection::Left;
	}
}

void PSEnergyBeam::Initialize()
{
	std::shared_ptr<ParticleEmitter> beamBackgroundEmitter = std::make_shared<ParticleEmitter>();

	/* 빔의 백그라운드 효과 표시 */
	auto SpawnBackgroundElement = [this](ParticleSystem& particleSystem)
		{
			Vector2Int addDirectionOffset = Vector2Int::Zero;
			switch (progressDirection)
			{
			case eDirection::Left:
				addDirectionOffset = Vector2Int::Left;
				break;

			case eDirection::Right:
				addDirectionOffset = Vector2Int::Right;
				break;

			case eDirection::Top:
				addDirectionOffset = Vector2Int::Up;
				break;

			case eDirection::Bottom:
				addDirectionOffset = Vector2Int::Down;
				break;
			}

			Vector2Int addVerticalOffset = Vector2Int::Zero;
			switch(verticalDirection)
			{ 
			case eDirection::Left:
				addVerticalOffset = Vector2Int::Left;
				break;

			case eDirection::Right:
				addVerticalOffset = Vector2Int::Right;
				break;

			case eDirection::Top:
				addVerticalOffset = Vector2Int::Up;
				break;

			case eDirection::Bottom:
				addVerticalOffset = Vector2Int::Down;
				break;
			}

			for (Vector2Int directionPos = Vector2Int::Zero;
				directionPos != endPosition;
				directionPos += addDirectionOffset)
			{
				Vector2Int elementPos = directionPos;

				for (int verticalCount = 0; verticalCount < verticalRange; ++verticalCount)
				{
					FParticleElement addParticleElement;
					addParticleElement.moveMode = eParticleElementMoveMode::Velocity;

					/* 배경 파티클입자는 따로 움직이지 않는다. */
					addParticleElement.velocity = Vector2Float::Zero;

					addParticleElement.image = L" ";
					addParticleElement.drawColor = Color::BG_Purple;
					addParticleElement.renderSortingOrder = static_cast<int>(eRenderSortingOrder::Particle);

					/* 시작 - 끝 위치까지 일렬로 추가  */
					addParticleElement.position = elementPos;
					particleSystem.AddParticleElement(std::move(addParticleElement));

					elementPos += addVerticalOffset;
				}
			}


			

			
			

			///* 극 좌표 기준으로 회전하는 효과 */
			////static const std::wstring elementImages[] = { L"*", L"+", L"#", L"$" };

			//for (int angle = 0; angle < 360; angle += 15)
			//{
			//	const float angleRad = DEG_TO_RAD(static_cast<float>(angle));

			//	FParticleElement addParticleElement;
			//	addParticleElement.moveMode = eParticleElementMoveMode::Orbit;

			//	addParticleElement.orbitAngle = angleRad;
			//	addParticleElement.orbitRadius = 12.f;
			//	addParticleElement.orbitAngularSpeed = DEG_TO_RAD(25.f);
			//	addParticleElement.orbitMinRadius = 12.f;
			//	addParticleElement.orbitMaxRadius = 25.f;
			//	addParticleElement.orbitRadiusScale = Vector2Float(1.f, 0.6f);

			//	const Vector2Float initializePos(cos(addParticleElement.orbitAngle) * addParticleElement.orbitRadius * addParticleElement.orbitRadiusScale.x,
			//		sin(addParticleElement.orbitAngle) * addParticleElement.orbitRadius * addParticleElement.orbitRadiusScale.y);

			//	addParticleElement.position.x = static_cast<int>(round(initializePos.x));
			//	addParticleElement.position.y = static_cast<int>(round(initializePos.y));

			//	addParticleElement.noisePosition = Vector2Float::Zero;
			//	addParticleElement.noiseSpeed = 0.5f;
			//	addParticleElement.noiseStrength = 15.f;

			//	addParticleElement.lifeTime = 0.f;

			//	addParticleElement.drawImages = { L"*", L"+", L"#", L"$" };
			//	const int selectElementImageIndex = Util::RandomRange(0, static_cast<int>(addParticleElement.drawImages.size()) - 1);
			//	addParticleElement.image = addParticleElement.drawImages[selectElementImageIndex];
			//	addParticleElement.drawChangeImageInterval = Util::RandomRange(0.2f, 0.5f);

			//	addParticleElement.drawColors = { Color::LightGreen, Color::LightGreen, Color::Green, Color::Green, Color::BrightWhite };
			//	const int selectElementParticleIndex = Util::RandomRange(0, static_cast<int>(addParticleElement.drawColors.size()) - 1);
			//	addParticleElement.drawColor = addParticleElement.drawColors[selectElementParticleIndex];
			//	addParticleElement.drawChangeColorInterval = Util::RandomRange(0.2f, 0.5f);

			//	addParticleElement.renderSortingOrder = static_cast<int>(eRenderSortingOrder::Particle);
			//	particleSystem.AddParticleElement(std::move(addParticleElement));
			//}
		};

	beamBackgroundEmitter->SetSpawnElementFunction(SpawnBackgroundElement);
	beamBackgroundEmitter->SetElementSpawnInterval(0.f);
	AddParticleEmitter(beamBackgroundEmitter);
}
