#include "NavigationBase.h"

namespace Craft
{
	const int NavigationBase::LIMIT_PATH_FIND_NUM = 50;

	NavigationBase::NavigationBase()
	{

	}

	eFindPathResult NavigationBase::FindPath(std::shared_ptr<Actor> agent,
															const Vector2Int& startPos,
															const Vector2Int& endPos,
															std::vector<Vector2Int>& resultPath) const
	{
		return eFindPathResult::Fail;
	}

	bool NavigationBase::CanNextMove(std::shared_ptr<Actor> agent, const Vector2Int& checkPos) const
	{
		return false;
	}

	CheckMoveResultType NavigationBase::CheckEnableMoveToTargetPosition(std::shared_ptr<Craft::Actor> agent,
																		const Craft::Vector2Int& checkPos, 
																		Vector2Int& enableMovePosition) const
	{
		return eCheckMoveTargetResult::Success;
	}

	void NavigationBase::SetCurrentLevel(std::weak_ptr<Level> level)
	{
		currentLevel = level;
	}
}
