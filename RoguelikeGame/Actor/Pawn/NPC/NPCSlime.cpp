#include "NPCSlime.h"
#include <Math/Color.h>

using namespace Craft;

NPCSlime::NPCSlime(const Craft::Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:NPCBase(position, L"s", Color::Purple, 3, roomIndex, 0.2f, 0.5f, 1.f)
{

}
