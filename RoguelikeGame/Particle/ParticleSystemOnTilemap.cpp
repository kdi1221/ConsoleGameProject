#include "ParticleSystemOnTilemap.h"
#include "Level/TilemapLevel.h"

ParticleSystemOnTilemap::ParticleSystemOnTilemap(const size_t elementMaxSize)
	:super(elementMaxSize)
{
}

bool ParticleSystemOnTilemap::IsDrawParticleElement(const Craft::FParticleElement& element, const Craft::Vector2Int& drawPosition) const
{
	std::shared_ptr<TilemapLevel> currentTilemapLevel = Cast<TilemapLevel>(GetCurrentLevel());
	if (!currentTilemapLevel)
	{
		return false;
	}

	/* ground 타일위에서만 그릴 수 있음*/
	return currentTilemapLevel->GetTileCategory(drawPosition) == eTileCategory::Ground;
}
