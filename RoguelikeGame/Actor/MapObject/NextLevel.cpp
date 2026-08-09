#include "NextLevel.h"
#include "Math/Color.h"
#include "Actor/Pawn/PlayerPawn.h"
#include "Engine/Engine.h"
#include "Level/TestLevel.h"

NextLevel::NextLevel(const Craft::Vector2Float& position)
	:super(position, "N", Craft::Color::Purple)
{
}

void NextLevel::OnCollision(const std::shared_ptr<Actor>& other)
{
	super::OnCollision(other);

	if (other->IsTypeOf<PlayerPawn>())
	{
		/* 플레이어가 접근했으면 다음 레벨로 이동 */
		Craft::Engine::Get().AddNewLevel<TestLevel>();
	}
}
