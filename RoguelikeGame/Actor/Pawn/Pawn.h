#pragma once

#include "Actor/Actor.h"
#include "Component/MovementComponent.h"

class Pawn : public Craft::Actor
{
	TYPE_DECLARATIONS(Pawn, Actor)

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

	/* 바라보는 방향 지정 */
	//void SetLookDirection(const Craft::Vector2Float& inDirection);

private:
	/* 이동 컴포넌트 */
	std::shared_ptr<Craft::MovementComponent> movementComponent;

//private:
//	/* 현재 바라보는 방향 */
//	Craft::Vector2Float lookDirection = Craft::Vector2Float::Up;
};

