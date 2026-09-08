#include "NextEndingLevel.h"
#include "Math/Color.h"
#include "Actor/Pawn/Player/PlayerPawn.h"
#include "Engine/Engine.h"
#include "Level/EndingLevel.h"

NextEndingLevel::NextEndingLevel(const Craft::Vector2Int& position)
	:super(position, L"⌂", Craft::Color::BrightYellow)
{

}

void NextEndingLevel::OnCollision(const std::shared_ptr<Actor>& other)
{
	super::OnCollision(other);

	if (other->IsTypeOf<PlayerPawn>())
	{
		/* 플레이어가 접근했으면 보스 레벨로 이동 */
		Craft::Engine::Get().AddNewLevel<EndingLevel>();
	}
}