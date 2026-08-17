#include "NPCGolem.h"

using namespace Craft;

NPCGolem::NPCGolem(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:NPCMeleeBase(position, L"G", Color::DarkGray, 150.f, roomIndex, 0.22f, 0.5f, 3.f, 12.f)
{

}
