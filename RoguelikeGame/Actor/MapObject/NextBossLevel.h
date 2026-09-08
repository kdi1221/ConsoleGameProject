#pragma once

#include "MapObject.h"

// 보스 레벨로 이동할 수 있는 오브젝트
class NextBossLevel : public MapObject
{
	TYPE_DECLARATIONS(NextBossLevel, MapObject)

public:
	NextBossLevel(const Craft::Vector2Int& position);

private:
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;
};

