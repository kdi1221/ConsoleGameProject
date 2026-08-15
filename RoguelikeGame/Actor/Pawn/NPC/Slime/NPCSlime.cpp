#include "NPCSlime.h"
#include <Math/Color.h>

using namespace Craft;

NPCSlime::NPCSlime(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:NPCMeleeBase(position, L"s", Color::Purple, 30.f, roomIndex, 0.25f, 0.5f, 2.f, 3.f)
{

}