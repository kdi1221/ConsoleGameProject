#include "NPCOrc.h"
#include <Math/Color.h>

using namespace Craft;

NPCOrc::NPCOrc(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, L"O", Color::Green, 50.f, 12.f, 6.f, 1.8f, 0.8f, roomIndex)
{

}
