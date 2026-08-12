#pragma once

#include "MapObject.h"

//맵의 특정 방안에 배치되어 플레이어가 시작할 위치를 제공하는 오브젝트
class PlayerStart : public MapObject
{
	TYPE_DECLARATIONS(PlayerStart, MapObject)

public:
	PlayerStart(const Craft::Vector2Int& position);
};

