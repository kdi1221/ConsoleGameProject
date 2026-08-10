#include "Pawn.h"
#include "Types/Enums.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/BoxCollisionComponent.h"
#include <cassert>

using namespace Craft;

Pawn::Pawn(const Craft::Vector2Float& position,
			const std::wstring& image,
			Craft::Color color,
			int CollisionWidth,
			float moveDelay)
	:super(position)
{
	// 필요한 컴포넌트 추가.
	AddComponent<SpriteRendererComponent>(image, color, static_cast<int>(eRenderSortingOrder::Pawn));
	AddComponent<BoxCollisionComponent>(CollisionWidth);
	movementComponent = AddComponent<MovementComponent>(moveDelay);
}

void Pawn::SetLastMoveDirection(const Craft::Vector2Int& moveDirection)
{
	assert(movementComponent && "Invalid movementComponent");

	movementComponent->SetLastMoveDirection(moveDirection);
}

void Pawn::SetLookDirection(const Craft::Vector2Float& inDirection)
{
	lookDirection = inDirection;
	lookDirection.Normalize();

	char szTmp[256] = { 0 };
	sprintf_s(szTmp, "Set Look Direction - x : %f, y : %f\n", lookDirection.x, lookDirection.y);
	OutputDebugStringA(szTmp);
}
