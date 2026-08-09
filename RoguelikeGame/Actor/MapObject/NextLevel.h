#pragma once

#include "MapObject.h"

//다음 레벨로 이동할 수 있는 오브젝트
class NextLevel : public MapObject
{
	TYPE_DECLARATIONS(NextLevel, MapObject)

public:
	NextLevel(const Craft::Vector2Float& position);

private:
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;
};

