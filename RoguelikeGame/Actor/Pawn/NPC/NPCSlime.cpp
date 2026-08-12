#include "NPCSlime.h"
#include <Math/Color.h>

using namespace Craft;

NPCSlime::NPCSlime(const Craft::Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:NPCBase(position, L"◎", Color::Blue, 1, 3, roomIndex)
{

}
