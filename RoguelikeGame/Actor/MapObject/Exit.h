#pragma once

#include "MapObject.h"

//게임 최종 목표 위치 오브젝트
class Exit : public MapObject
{
	TYPE_DECLARATIONS(Exit, MapObject)

public:
	Exit(const Craft::Vector2Float& position);
};

