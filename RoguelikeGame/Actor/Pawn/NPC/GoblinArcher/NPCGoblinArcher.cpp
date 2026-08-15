#include "NPCGoblinArcher.h"

using namespace Craft;
NPCGoblinArcher::NPCGoblinArcher(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, L"g", Color::Green, 15.f, roomIndex, 0.15f, 0.5f)
{
	projectileFireDelay = 1.5f;
	projectileImage = L"*";
	projectileColor = Color::Red;
	projectileMoveDelayMin = 0.04f;
	projectileMoveDelayMax = 0.06f;
	projectileDamageValue = 2.f;
	attackRange = 8.f;
}