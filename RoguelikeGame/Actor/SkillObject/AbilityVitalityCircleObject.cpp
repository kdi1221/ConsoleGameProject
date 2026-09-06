#include "AbilityVitalityCircleObject.h"
#include "Component/ParticleComponent.h"
#include "Level/TilemapLevel.h"
#include "Particle/ParticleSystemVitalityCircle.h"
#include "StaticLibrary/StaticFunctionLibrary.h"
#include "Actor/Pawn/Player/PlayerPawn.h"
#include <Render/Renderer.h>
#include <cassert>

using namespace Craft;

AbilityVitalityCircleObject::AbilityVitalityCircleObject(const Craft::Vector2Int& position)
	:super(position)
{
	/* 파티클 컴포넌트 */
	particleComponent = AddComponent<ParticleComponent>();
}

void AbilityVitalityCircleObject::BeginPlay()
{
	super::BeginPlay();

	/* 파티클 시스템 : 회복진 원형 표시 */
	assert(particleComponent && "Invalid particleComponent");
	std::shared_ptr<ParticleSystemVitalityCircle> spawnedParticleSystem = std::make_shared<ParticleSystemVitalityCircle>();
	spawnedParticleSystem->Initialize();
	particleComponent->AddParticleSystem(spawnedParticleSystem);
}

void AbilityVitalityCircleObject::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	/* 주기별로 범위 내 대상들을 향해 회복 시전 */
	timerIntervalRegen.Tick(deltaTime);
	if (timerIntervalRegen.IsTimeOut())
	{
		ExecuteRegeneration();
		timerIntervalRegen.Reset();
	}

	/* 수명이 끝나면 Destory */
	timerLifeSpan.Tick(deltaTime);
	if (timerLifeSpan.IsTimeOut())
	{
		Destroy();
	}
}

//void AbilityVitalityCircleObject::Draw()
//{
//	super::Draw();
//
//	//디버깅용
//	Renderer& renderer = Renderer::Get();
//
//	for (const Vector2Int& tileCoord : circleOuterTileCoord)
//	{
//		renderer.Submit(L" ", tileCoord, Color::BG_Blue, static_cast<int>(eRenderSortingOrder::FieldItem));
//	}
//}

void AbilityVitalityCircleObject::SetInstigatorTeamID(eTeamID teamID)
{
	instigatorTeamID = teamID;
}

void AbilityVitalityCircleObject::SetlimitRange(float range)
{
	limitRange = range;
}

void AbilityVitalityCircleObject::SetHealthRegeneration(float rate)
{
	healthRegeneration = rate;
}

void AbilityVitalityCircleObject::SetManaRegeneration(float rate)
{
	manaRegeneration = rate;
}

void AbilityVitalityCircleObject::SetLifeSpan(float lifeTime)
{
	timerLifeSpan.SetTargetTime(lifeTime);
}

void AbilityVitalityCircleObject::SetIntervalRegeneration(float interval)
{
	timerIntervalRegen.SetTargetTime(interval);
}

void AbilityVitalityCircleObject::ExecuteRegeneration()
{
	std::shared_ptr<TilemapLevel> tilemapLevel = Cast<TilemapLevel>(GetOwner());
	assert(tilemapLevel && "Invalid tilemapLevel");

	int minX = INT_MAX, maxX = INT_MIN;
	int minY = INT_MAX, maxY = INT_MIN;
	
	//circleOuterTileCoord.clear();
	StaticFunctionLibrary::MidpointCircleProcess(
		GetWorldPosition(),
		static_cast<int>(limitRange),
		static_cast<int>(limitRange * 0.6f),
		[&](int x, int y)
		{
			//circleOuterTileCoord.emplace_back(Vector2Int(x, y));

			minX = min(minX, x);
			maxX = max(maxX, x);
			minY = min(minY, y);
			maxY = max(maxY, y);
		}
	);

	auto checkInsideEllipse = [](const Vector2Int& centerPos, const Vector2Int& checkPos, float xRadiusSq, float yRadiusSq)
		{
			const Vector2Int distance = checkPos - centerPos;
			const float distanceXSq = static_cast<float>(distance.x * distance.x);
			const float distanceYSq = static_cast<float>(distance.y * distance.y);

			const float ellipseValue = distanceXSq / xRadiusSq + distanceYSq / yRadiusSq;

			return ellipseValue <= 1.f;
		};

	const Vector2Int& centerPos = GetWorldPosition();
	float currentXRadiusSq = (limitRange * limitRange), currentYRadiusSq = (currentXRadiusSq * 0.6f * 0.6f);

	for (int yPos = minY; yPos <= maxY; ++yPos)
	{
		for (int xPos = minX; xPos <= maxX; ++xPos)
		{
			const Vector2Int checkTileCoord(xPos, yPos);

			/* 현재 범위 안에 들지 않은 위치는 지나감 */
			if (!checkInsideEllipse(centerPos, checkTileCoord, currentXRadiusSq, currentYRadiusSq))
			{
				continue;
			}

			/* 현재 타원 범위안의 대상을 찾아서 회복 */
			std::shared_ptr<Pawn> targetPawn = Cast<Pawn>(tilemapLevel->GetActorOnTileCoordinate(
				checkTileCoord,
				[&](std::weak_ptr<ActorOnTile> checkActor)
				{
					std::shared_ptr<Pawn> otherPawn = Cast<Pawn>(checkActor.lock());

					/* Pawn이 유효하지 않으면 건너뜀 */
					if (!otherPawn || otherPawn->HasExpired())
					{
						return false;
					}

					/* Pawn이 이미 죽었으면 건너뜀 */
					if (otherPawn->IsDeath())
					{
						return false;
					}

					/* 다른 팀 소속이면 건너뜀*/
					if (otherPawn->GetTeamID() != instigatorTeamID)
					{
						return false;
					}

					return true;
				}
			));

			if (!targetPawn)
			{
				continue;
			}

			targetPawn->AddHealthValue(healthRegeneration);
			
			/* 플레이어 폰이면 마나도 회복 */
			if (std::shared_ptr<PlayerPawn> targetPlayerPawn = Cast<PlayerPawn>(targetPawn))
			{
				targetPlayerPawn->AddManaValue(manaRegeneration);
			}
		}
	}
}
