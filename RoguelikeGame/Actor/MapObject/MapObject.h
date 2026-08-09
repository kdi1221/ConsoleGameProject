#pragma once

#include "Actor/Actor.h"
#include "Math/Color.h"

//맵 상에 배치되는 오브젝트들의 부모 오브젝트
class MapObject : public Craft::Actor
{
	TYPE_DECLARATIONS(MapObject, Actor)

public:
	MapObject(const Craft::Vector2Float& position,
			const std::wstring& image,
			Craft::Color color);
	~MapObject() = default;
};

