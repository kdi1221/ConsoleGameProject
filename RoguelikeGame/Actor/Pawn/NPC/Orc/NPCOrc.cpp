#include "NPCOrc.h"
#include <Math/Color.h>

using namespace Craft;

NPCOrc::NPCOrc(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:NPCMeleeBase(position, L"O", Color::Green, 50.f, roomIndex, 0.18f, 0.5f, 1.5f, 5.f)
{

}
