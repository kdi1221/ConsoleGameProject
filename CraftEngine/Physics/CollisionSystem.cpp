#include "CollisionSystem.h"
#include "Actor/Actor.h"
#include "Component/BoxCollisionComponent.h"

namespace Craft
{
	void CollisionSystem::ProcessCollision(const std::vector<std::shared_ptr<Actor>>& actorList)
	{
		//예외처리.
		if (actorList.empty())
		{
			return;
		}

		//충돌한 액터에 이벤트를 전달하기 위한 배열.
		std::vector<CollisionPair> collidedActorList;

		//레벨에 배치된 액터 수
		const int count = static_cast<int>(actorList.size());

		//모든 액터를 순회하면서 충돌 검사.
		for (int ix = 0; ix < count; ++ix)
		{
			const std::shared_ptr<Actor>& left = actorList[ix];

			//액터가 null이거나 비활성화 상태라면 건너뛰기
			if (!left || !left->IsActive())
			{
				continue;
			}

			//다음 액터들과의 충돌확인을 위한 루프.
			for (int jx = ix + 1; jx < count; ++jx)
			{
				const std::shared_ptr<Actor>& right = actorList[jx];

				//액터가 null이거나 비활성화 상태라면 건너뛰기
				if (!right || !right->IsActive())
				{
					continue;
				}

				// 충돌 검사
				if (Test(left, right))
				{
					//이벤트를 발행할 목록에 추가할 데이터 생성.
					CollisionPair pair = {};
					pair.actor = left;
					pair.other = right;

					//목록에 추가.
					collidedActorList.emplace_back(pair);
				}
			}
		}

		if (collidedActorList.empty())
		{
			return;
		}

		// 충돌한 액터에 이벤트 전달. 
		for (const CollisionPair& pair : collidedActorList)
		{
			// 예외처리. 이미 삭제되었거나 비활성화된 액터는 제외.
			if (!pair.actor->IsActive() || !pair.other->IsActive())
			{
				continue;
			}

			// 충돌 이벤트 전달.
			pair.actor->OnCollision(pair.other);
			pair.other->OnCollision(pair.actor);
		}
	}

	bool CollisionSystem::Test(const std::shared_ptr<Actor>& left, const std::shared_ptr<Actor>& right)
	{
		//예외 처리.
		if (!left || !left->IsActive() || !right || !right->IsActive())
		{
			return false;
		}

		// AABB(Axis Aligned Bounding Box)
		// x의 크기와 y크기를 고려해 박스를 구성하고 두 박스가 서로 겹치는 확인하는 방법.

		// 두 액터의 충돌 컴포넌트 확인.
		std::shared_ptr<BoxCollisionComponent> leftCollision = left->GetComponent<BoxCollisionComponent>();
		std::shared_ptr<BoxCollisionComponent> rightCollision = right->GetComponent<BoxCollisionComponent>();

		//예외 처리
		if (!leftCollision || !rightCollision)
		{
			return false;
		}

		//left 액터의 현재/이전 위치.
		const Vector2Int leftCurrent = left->GetWorldPosition();
		const Vector2Int leftPrevious = left->GetPreviousPosition();

		//right 액터의 현재/이전 위치.
		const Vector2Int rightCurrent = right->GetWorldPosition();
		const Vector2Int rightPrevious = right->GetPreviousPosition();

		// 이전 프레임 위치와 현재 위치를 모두 포함하는 바운드(충돌 영역)계산.
		const int leftXMin = (leftCurrent.x < leftPrevious.x) ? leftCurrent.x : leftPrevious.x;
		const int leftXMaxCurrent = leftCurrent.x + leftCollision->GetWidth() - 1;
		const int leftXMaxPrevious = leftPrevious.x + leftCollision->GetWidth() - 1;
		const int leftXMax = (leftXMaxCurrent > leftXMaxPrevious) ? leftXMaxCurrent : leftXMaxPrevious;

		const int rightXMin = (rightCurrent.x < rightPrevious.x) ? rightCurrent.x : rightPrevious.x;
		const int rightXMaxCurrent = rightCurrent.x + rightCollision->GetWidth() - 1;
		const int rightXMaxPrevious = rightPrevious.x + rightCollision->GetWidth() - 1;
		const int rightXMax = (rightXMaxCurrent > rightXMaxPrevious) ? rightXMaxCurrent : rightXMaxPrevious;

		//x 좌표 기준 비교.
		if (rightXMin > leftXMax)
		{
			return false;
		}

		if (rightXMax < leftXMin)
		{
			return false;
		}

		// 이전 프레임까지 고려한 y충돌 영역 계산
		const int leftYMin = (leftCurrent.y < leftPrevious.y) ? leftCurrent.y : leftPrevious.y;
		const int leftYMax = (leftCurrent.y > leftPrevious.y) ? leftCurrent.y : leftPrevious.y;

		const int rightYMin = (rightCurrent.y < rightPrevious.y) ? rightCurrent.y : rightPrevious.y;
		const int rightYMax = (rightCurrent.y > rightPrevious.y) ? rightCurrent.y : rightPrevious.y;

		// y 좌표 기준으로 비교.
		if (rightYMin > leftYMax)
		{
			return false;
		}

		if (rightYMax < leftYMin)
		{
			return false;
		}

		//충돌 발생.
		return true;
	}
}