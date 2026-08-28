#pragma once

#include "AttributeComponent.h"

/* 플레이어의 Attribute */
class PlayerAttributeComponent : public AttributeComponent
{
	TYPE_DECLARATIONS(PlayerAttributeComponent, AttributeComponent)

public:
	PlayerAttributeComponent() = default;
	virtual ~PlayerAttributeComponent() = default;

	/* Mana 속성 */
	ATTRIBUTE_DECLARATIONS(Mana)
};

