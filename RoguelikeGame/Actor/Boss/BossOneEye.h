#pragma once

#include <Actor/Actor.h>

/* Boss, One Eye*/
class BossOneEye : public Craft::Actor
{
	TYPE_DECLARATIONS(BossOneEye, Actor)

public:
	BossOneEye(const Craft::Vector2Int& position);
	virtual ~BossOneEye();

public:
	virtual void Draw() override;
};

