#pragma once

#include "Actor/Actor.h"

class Ground : public Craft::Actor
{
	// RTTI Àû¿ë
	TYPE_DECLARATIONS(Ground, Actor)

public:
	Ground(const Craft::Vector2& position);
};

