#pragma once

#include "Actor/ActorOnTile.h"
#include "Component/MovementComponent.h"

class Pawn : public ActorOnTile
{
	TYPE_DECLARATIONS(Pawn, ActorOnTile)

public:
	Pawn(const Craft::Vector2Float& position,
		const std::wstring& image,
		Craft::Color color,
		int CollisionWidth,
		float moveDelay);

	~Pawn() = default;

protected:
	/* 다음 프레임에 처리할 이동방향 지정 */
	void SetLastMoveDirection(const Craft::Vector2Int& moveDirection);

private:
	/* 이동 컴포넌트 */
	std::shared_ptr<Craft::MovementComponent> movementComponent;
};

