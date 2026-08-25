#include "NavigationBase.h"

namespace Craft
{
	NavigationBase::NavigationBase()
	{

	}

	bool NavigationBase::FindPath(std::shared_ptr<Actor> agent,
								const Vector2Int& startPos,
								const Vector2Int& endPos,
								std::vector<Vector2Int>& resultPath) const
	{
		return false;
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
