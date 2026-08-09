#pragma once

#include "Actor/Actor.h"
#include "Component/MovementComponent.h"

class Pawn : public Craft::Actor
{
	TYPE_DECLARATIONS(Pawn, Actor)

public:
	Pawn(const Craft::Vector2Float& position,
		const std::string& image,
		Craft::Color color,
		int CollisionWidth,
		float moveDelay);

	~Pawn() = default;

public:
	/* 다음 프레임에 처리할 이동방향 지정 */
	void SetLastMoveDirection(const Craft::Vector2Int& moveDirection);

private:
	std::shared_ptr<Craft::MovementComponent> movementComponent;
};

