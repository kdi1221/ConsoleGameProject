#include "AbilityEnergyBeamObject.h"
#include "Level/TilemapLevel.h"
#include "Component/ParticleComponent.h"
#include "Particle/PSEnergyBeam.h"
#include "Actor/Pawn/Pawn.h"
#include <Util/Util.h>
#include <Render/Renderer.h>
#include <cassert>

using namespace Craft;

AbilityEnergyBeamObject::AbilityEnergyBeamObject(const Vector2Int& position,
												const Vector2Int& endPos,
												int range)
	:super(position)
	,beamEndPosition(endPos)
	,beamVerticalRange(range)
{
	const Vector2Int& currentPos = GetWorldPosition();
	const Vector2Int ToEndDistance = beamEndPosition - currentPos;
	const int width = abs(ToEndDistance.x) + 1;
	const int height = abs(ToEndDistance.y) + 1;

	if (abs(ToEndDistance.x) > 0)
	{
		progressDirection = ToEndDistance.x > 0 ? eDirection::Right : eDirection::Left;
	}
	else if (abs(ToEndDistance.y) > 0)
	{
		progressDirection = ToEndDistance.y > 0 ? eDirection::Bottom : eDirection::Top;
	}

	switch (progressDirection)
	{
	case eDirection::Right:
		{
			drawBeamImageBuffer.resize(beamVerticalRange);
			for (int i = 0; i < beamVerticalRange; ++i)
			{
				drawBeamImageBuffer[i] = std::wstring(width, L' ');
			}

			maxRange = width;
		}
		break;

	case eDirection::Left:
		{
			drawBeamImageBuffer.resize(beamVerticalRange);
			for (int i = 0; i < beamVerticalRange; ++i)
			{
				drawBeamImageBuffer[i] = std::wstring(width, L' ');
			}

			maxRange = width;
		}
		break;

	case eDirection::Top:
		{
			drawBeamImageBuffer.resize(height);
			for (int i = 0; i < height; ++i)
			{
				drawBeamImageBuffer[i] = std::wstring(beamVerticalRange, L' ');
			}

			maxRange = height;
		}
		break;

	case eDirection::Bottom:
		{
			drawBeamImageBuffer.resize(height);
			for (int i = 0; i < height; ++i)
			{
				drawBeamImageBuffer[i] = std::wstring(beamVerticalRange, L' ');
			}

			maxRange = height;
		}
		break;
	}

	UpdateBeamStartPosition();

	/* 파티클 컴포넌트 */
	particleComponent = AddComponent<ParticleComponent>();
}

void AbilityEnergyBeamObject::BeginPlay()
{
	super::BeginPlay();

	//const Vector2Int& currentPos = GetWorldPosition();
	//const Vector2Int ToEndDistance = beamEndPosition - currentPos;

	///* 파티클 시스템 : 에너지 빔 표시 */
	//const int range = 10;
	//assert(particleComponent && "Invalid particleComponent");
	//std::shared_ptr<PSEnergyBeam> spawnedParticleSystem = std::make_shared<PSEnergyBeam>(ToEndDistance, range);
	//spawnedParticleSystem->Initialize();
	//particleComponent->AddParticleSystem(spawnedParticleSystem);

	SetCurrentExpandMode(eBeamExpandMode::Charging);
}

void AbilityEnergyBeamObject::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	/* 다음 빔 확장 모드로 전환 */
	if (nextBeamExpandMode != eBeamExpandMode::None)
	{
		timerNextBeamExpandModeDelay.Tick(deltaTime);
		if (timerNextBeamExpandModeDelay.IsTimeOut())
		{
			eBeamExpandMode newBeamExpandMode = nextBeamExpandMode;
			nextBeamExpandMode = eBeamExpandMode::None;
			SetCurrentExpandMode(newBeamExpandMode);
			timerNextBeamExpandModeDelay.Reset();
		}
	}

	switch (beamExpandMode)
	{
	case eBeamExpandMode::Expand:
		{
			ExpandBeamRect(deltaTime);
			
			if (currentRange == maxRange)
			{
				/* 딜레이 후 빔 축소 모드로 전환 */
				SetNextExpandMode(eBeamExpandMode::Shrink, 5.f);
			}
			
		}
		break;

	case eBeamExpandMode::Shrink:
		{
			ExpandBeamRect(deltaTime);

			if (currentRange == minRange)
			{
				Destroy();
			}
		}
		break;
	}
}

void AbilityEnergyBeamObject::PostTick(float deltaTime)
{
	super::PostTick(deltaTime);

	// 일정 주기별로 범위 내 대상들에게 데미지 처리 
	if (beamExpandMode == eBeamExpandMode::Expand)
	{
		timerDamageProcessInterval.Tick(deltaTime);
		if (timerDamageProcessInterval.IsTimeOut())
		{
			ApplyDamage();
			timerDamageProcessInterval.Reset();
		}
	}
}

void AbilityEnergyBeamObject::Draw()
{
	super::Draw();

	DrawBeamRect();
}

void AbilityEnergyBeamObject::Destroy()
{
	if (onDestroyBeamCallback)
	{
		onDestroyBeamCallback(*this);
	}

	super::Destroy();
}

void AbilityEnergyBeamObject::SetInstigatorTeamID(eTeamID teamID)
{
	instigatorTeamID = teamID;
}

void AbilityEnergyBeamObject::SetIndex(int index)
{
	beamIndex = index;
}

void AbilityEnergyBeamObject::SetOnDestroyBeamCallback(OnDestroyBeamCallbackType callback)
{
	onDestroyBeamCallback = callback;
}

void AbilityEnergyBeamObject::SetExpandDuration(float duration)
{
	expandDuration = duration;
}

void AbilityEnergyBeamObject::SetDamageValue(float amount)
{
	damageAmount = amount;
}

void AbilityEnergyBeamObject::SetDamageProcessInterval(float interval)
{
	timerDamageProcessInterval.SetTargetTime(interval);
}

void AbilityEnergyBeamObject::ExpandBeamRect(float deltaTime)
{
	expandElapsedTime += deltaTime;

	const float t = max(0.f, min(1.f, expandElapsedTime / expandDuration));

	//Ease-In
	//const float ratio = t * t;

	//Ease-In(x2)
	const float ratio = t * t * t;

	//Ease-Out
	//const float oneMinusT = 1.f - t;
	//const float ratio = 1.f - (oneMinusT * oneMinusT);

	float minValue = 0.f, maxValue = 0.f;
	if (beamExpandMode == eBeamExpandMode::Expand)
	{
		minValue = static_cast<float>(minRange);
		maxValue = static_cast<float>(maxRange);
	}
	else
	{
		minValue = static_cast<float>(maxRange);
		maxValue = static_cast<float>(minRange);
	}

	const float lerpResult = Util::Lerp(minValue, maxValue, ratio);

	currentRange = max(minRange, min(static_cast<size_t>(lerpResult), maxRange));

	UpdateBeamStartPosition();
}

void AbilityEnergyBeamObject::DrawBeamRect()
{
	if (currentRange == 0)
	{
		return;
	}

	Renderer& renderer = Renderer::Get();
	Vector2Int drawLinePos = drawStartPosition; 

	switch (progressDirection)
	{
	case eDirection::Right:
	case eDirection::Left:
		{
			for (const std::wstring drawBeamImage : drawBeamImageBuffer)
			{
				renderer.Submit(drawBeamImage.substr(0, currentRange),
								drawLinePos,
								Color::BG_Purple,
								static_cast<int>(eRenderSortingOrder::beam));
				++drawLinePos.y;
			}
		}
		break;

	case eDirection::Top:
	case eDirection::Bottom:
		{
			for (size_t i = 0; i < currentRange; ++i)
			{
				renderer.Submit(drawBeamImageBuffer[i],
								drawLinePos,
								Color::BG_Purple,
								static_cast<int>(eRenderSortingOrder::beam));
				++drawLinePos.y;
			}
		}
		break;
	}
}

void AbilityEnergyBeamObject::UpdateBeamStartPosition()
{
	const Vector2Int& currentPos = GetWorldPosition();
	switch (progressDirection)
	{
	case eDirection::Right:
		{
			drawStartPosition = Vector2Int(currentPos.x, currentPos.y - beamVerticalRange + 1);
		}
		break;

	case eDirection::Left:
		{
			drawStartPosition = Vector2Int(currentPos.x - static_cast<int>(currentRange) + 1, currentPos.y);
		}
		break;

	case eDirection::Top:
		{
			drawStartPosition = Vector2Int(currentPos.x - beamVerticalRange + 1, currentPos.y - static_cast<int>(currentRange) + 1);
		}
		break;

	case eDirection::Bottom:
		{
			drawStartPosition = currentPos;
		}
		break;
	}
}

void AbilityEnergyBeamObject::SetCurrentExpandMode(eBeamExpandMode current)
{
	beamExpandMode = current;

	switch (beamExpandMode)
	{
	case eBeamExpandMode::Charging:
		{
			/* 딜레이 후 빔 확장 모드로 전환 */
			SetNextExpandMode(eBeamExpandMode::Expand, 5.f);
		}
		break;

	case eBeamExpandMode::Expand:
		{
			expandElapsedTime = 0.f;
		}
		break;

	case eBeamExpandMode::Shrink:
		{
			expandElapsedTime = 0.f;
		}
		break;
	}
}

void AbilityEnergyBeamObject::SetNextExpandMode(eBeamExpandMode nextMode, float delay)
{
	nextBeamExpandMode = nextMode;
	timerNextBeamExpandModeDelay.SetTargetTime(delay);
}

void AbilityEnergyBeamObject::ApplyDamage()
{
	std::shared_ptr<TilemapLevel> tilemapLevel = Cast<TilemapLevel>(GetOwner());
	assert(tilemapLevel && "Invalid tilemapLevel");

	int yOffsetMax = 0;
	int xOffsetMax = 0;

	switch (progressDirection)
	{
	case eDirection::Right:
	case eDirection::Left:
		{
			yOffsetMax = beamVerticalRange;
			xOffsetMax = static_cast<int>(currentRange);
		}
		break;

	case eDirection::Top:
	case eDirection::Bottom:
		{
			yOffsetMax = static_cast<int>(currentRange);
			xOffsetMax = beamVerticalRange;
		}
		break;
	}

	for (int yOffset = 0; yOffset < yOffsetMax; ++yOffset)
	{
		for (int xOffset = 0; xOffset < xOffsetMax; ++xOffset)
		{
			const Vector2Int checkTileCoord = drawStartPosition + Vector2Int(xOffset, yOffset);

			/* prev, Current 타원 범위안의 대상을 찾아서 데미지를 주어야 함*/
			std::shared_ptr<Pawn> targetPawn = Cast<Pawn>(tilemapLevel->GetActorOnTileCoordinate(
				checkTileCoord,
				[&](std::weak_ptr<ActorOnTile> checkActor)
				{
					std::shared_ptr<Pawn> otherPawn = Cast<Pawn>(checkActor.lock());

					/* Pawn이 유효하지 않으면 건너뜀 */
					if (!otherPawn ||
						otherPawn->HasExpired())
					{
						return false;
					}

					/* 같은 팀 소속, 이미 데미지를 준 상대면 건너뜀 */
					if (otherPawn->GetTeamID() == instigatorTeamID)
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

			targetPawn->TakeDamage(damageAmount);
		}
	}
}
