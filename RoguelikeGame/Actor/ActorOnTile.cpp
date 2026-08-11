#include "ActorOnTile.h"
#include <Level/TilemapLevel.h>
#include <cassert>

using namespace Craft;

ActorOnTile::ActorOnTile(const Vector2Float& position)
	:super(position)
{

}

void ActorOnTile::OnTileOverlap(const eTileCategory tileCategory)
{

}

//void ActorOnTile::BeginPlay()
//{
//	super::BeginPlay();
//
//	//TODO : Tilemap Level에 등록 처리
//	std::shared_ptr<TilemapLevel> tilemapLevel = GetOwnerTilemapLevel();
//	assert(tilemapLevel && "Invalid tilemapLevel");
//
//	//tilemapLevel->
//}

