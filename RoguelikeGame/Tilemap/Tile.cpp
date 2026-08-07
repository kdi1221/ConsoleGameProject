#include "Tile.h"

Tile::Tile(eTileCategory InCategory, const Craft::Vector2Int& InPosition, size_t InTileIndex)
	:category(InCategory)
	,position(InPosition)
	,index(InTileIndex)
{

}

Tile::~Tile()
{

}
