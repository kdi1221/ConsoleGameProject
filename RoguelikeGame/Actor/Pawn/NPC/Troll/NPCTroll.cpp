#include "NPCTroll.h"

using namespace Craft;

NPCTroll::NPCTroll(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, L"T", Color::BrightGray, 80.f, 15.f, 10.f, 1.5f, 0.7f, roomIndex)
{

}
