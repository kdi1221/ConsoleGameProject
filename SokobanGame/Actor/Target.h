#pragma once

#include "Actor/Actor.h"

class Target : public Craft::Actor
{
	//RTTI µî·Ï
	TYPE_DECLARATIONS(Target, Actor)

public:
	Target(const Craft::Vector2Float& position);
};

