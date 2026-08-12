#include "NPCBase.h"

NPCBase::NPCBase(const Craft::Vector2Int& position, 
				const std::wstring& image, 
				Craft::Color color, 
				int CollisionWidth,
				int initialHealth,
				RoomDefines::UNIQUE_INDEX_TYPE roomIndex)
	:super(position, image, color, CollisionWidth, initialHealth, eTeamID::NPC)
	,spawnedRoomIndex(roomIndex)
{

}
