#include "NPCGolem.h"

using namespace Craft;

NPCGolem::NPCGolem(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, L"G", Color::DarkGray, 150.f, 10.f, 15.f, 4.f, 1.8f, roomIndex)
{

}
