#include "NPCTroll.h"

using namespace Craft;

NPCTroll::NPCTroll(const Vector2Int& position, RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:NPCMeleeBase(position, L"T", Color::BrightGray, 80.f, roomIndex, 0.14f, 0.5f, 1.2f, 7.f)
{

}
