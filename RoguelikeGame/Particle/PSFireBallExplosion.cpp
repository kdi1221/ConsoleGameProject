#include "PSFireBallExplosion.h"
#include "Particle/ParticleEmitter.h"
#include "Types/Enums.h"
#include "Level/TilemapLevel.h"
#include <StaticLibrary/StaticFunctionLibrary.h>
#include <Math/Define.h>
#include <Util/Util.h>

using namespace Craft;

PSFireBallExplosion::PSFireBallExplosion()
	:super(32)
{

}

void PSFireBallExplosion::Initialize()
{
	super::Initialize();

	std::shared_ptr<ParticleEmitter> explosionEmitter = std::make_shared<ParticleEmitter>();

	auto SpawnExplosionElement = [this](ParticleSystem& particleSystem)
		{
			static const int particleNum = 32;
			const float unitAngle = (2 * PI) / static_cast<float>(particleNum);

			for (int i = 0; i < particleNum; ++i)
			{
				const float angle = (unitAngle * static_cast<float>(i)) + Util::RandomRange(-0.15f, 0.15f);
				const Vector2Float direction(cos(angle), sin(angle));
				//const Vector2Float spwnPositionOffset = direction * 2.f;
				//const Vector2Int spawnPosition(static_cast<int>(round(spwnPositionOffset.x)), static_cast<int>(round(spwnPositionOffset.y)));

				FParticleElement addParticleElement;
				addParticleElement.moveMode = eParticleElementMoveMode::Velocity;

				addParticleElement.position = Vector2Int::Zero;
				addParticleElement.velocity = direction * Util::RandomRange(2.f, 5.f);
				addParticleElement.lifeTime = Util::RandomRange(0.2f, 1.f);

				addParticleElement.drawImages = { L"*", L"#", L"x" };
				const int selectElementImageIndex = Util::RandomRange(0, static_cast<int>(addParticleElement.drawImages.size()) - 1);
				addParticleElement.image = addParticleElement.drawImages[selectElementImageIndex];
				addParticleElement.drawChangeImageInterval = Util::RandomRange(0.2f, 0.5f);

				addParticleElement.drawColors = { Color::Red, Color::LightRed, Color::Yellow, Color::Red, Color::LightRed };
				const int selectElementParticleIndex = Util::RandomRange(0, static_cast<int>(addParticleElement.drawColors.size()) - 1);
				addParticleElement.drawColor = addParticleElement.drawColors[selectElementParticleIndex];
				addParticleElement.drawChangeColorInterval = Util::RandomRange(0.2f, 0.5f);

				addParticleElement.renderSortingOrder = static_cast<int>(eRenderSortingOrder::Particle);
				particleSystem.AddParticleElement(std::move(addParticleElement));
			}
			
		};

	explosionEmitter->SetSpawnElementFunction(SpawnExplosionElement);
	explosionEmitter->SetElementSpawnInterval(0.f);
	AddParticleEmitter(explosionEmitter);
}

bool PSFireBallExplosion::ShouldRemoveParticleElement(const FParticleElement& element, const Vector2Int& worldPosition) const
{
	std::shared_ptr<TilemapLevel> currentTilemapLevel = Cast<TilemapLevel>(GetCurrentLevel());
	if (!currentTilemapLevel)
	{
		return true;
	}

	const Vector2Int& centerPos = GetCenterPosition();

	/* 중심위치에서 스폰된 입자는 지우지 않는다. */
	/*if (centerPos == worldPosition)
	{
		return false;
	}*/

	/* 현재 위치가 Ground가 아니면 지운다. */
	if (currentTilemapLevel->GetTileCategory(worldPosition) != eTileCategory::Ground)
	{
		return true;
	}


	/* 중심위치 - 현재위치까지 브레젠험으로 경로를 구한뒤 중간이 벽이나 문으로 막혀 있으면 지운다. */
	//std::vector<Vector2Int> movePaths;
	//StaticFunctionLibrary::GetBresenhamPath(centerPos, worldPosition, movePaths);
	//for (const Vector2Int& pathTileCoord : movePaths)
	//{
	//	if (pathTileCoord == centerPos)
	//	{
	//		continue;
	//	}

	//	/* 중심위치와 자신까지의 경로에 벽이 있으면 지워야 한다. */
	//	if (eTileCategory::Ground != currentTilemapLevel->GetTileCategory(pathTileCoord))
	//	{
	//		return true;
	//	}

	//	/* 문도 체크해야 함..*/
	//}

	return false;
}
