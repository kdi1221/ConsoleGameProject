#include "NavigationBase.h"

namespace Craft
{
	const int NavigationBase::LIMIT_PATH_FIND_NUM = 50;

	NavigationBase::NavigationBase()
	{

	}

	void NavigationBase::ProcessPathFindRequests()
	{
		
	}

	eFindPathResult NavigationBase::RequestFindPath(std::shared_ptr<NavMovementComponent> requester,
													const Vector2Int& startPos, 
													const Vector2Int& endPos,
													RequestPathHandleType& outRequestPathHandle)
	{
		return eFindPathResult::Fail;
	}

	void NavigationBase::CancelFindPathRequest(std::shared_ptr<NavMovementComponent> requester)
	{
		
	}

	eFindPathResult NavigationBase::FindPath(std::shared_ptr<Actor> agent,
															const Vector2Int& startPos,
															const Vector2Int& endPos,
															std::vector<Vector2Int>& resultPath) const
	{
		return eFindPathResult::Fail;
	}

	bool NavigationBase::SimulatePreviousToNextMove(std::shared_ptr<Actor> agent, const Vector2Int& prevPosition, const Vector2Int& nextPosition) const
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

	void NavigationBase::ResetCurrentLevel()
	{
	}

	void NavigationBase::SetCurrentLevel(std::weak_ptr<Level> level)
	{
		currentLevel = level;
	}

	RequestPathHandleType NavigationBase::GenerateRequestPathHandle() const
	{
		static RequestPathHandleType handleRequestPath = INVALID_REQUEST_PATH_HANDLE;
		++handleRequestPath;

		// 계속 증가하다가 INVALID_REQUEST_PATH_HANDLE이 나오면 다시 1부터 시작하도록 한다.
		if(handleRequestPath == INVALID_REQUEST_PATH_HANDLE)
		{
			++handleRequestPath;
		}

		return handleRequestPath;
	}
}
