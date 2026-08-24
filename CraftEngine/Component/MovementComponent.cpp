#include "MovementComponent.h"
#include "Actor/Actor.h"
#include "Level/Level.h"
#include "StaticLibrary/StaticFunctionLibrary.h"
#include <cassert>

namespace Craft
{
	//MovementComponent::MovementComponent(float inMoveDelay)
	MovementComponent::MovementComponent(float inMoveSpeed)
		:moveSpeed(inMoveSpeed)
	{
		//이동 딜레이 타이머 설정
		//moveTimer.SetTargetTime(inMoveDelay);
	}

	void MovementComponent::BeginPlay()
	{
		super::BeginPlay();

		std::shared_ptr<Actor> ownerActor = GetOwner();
		assert(ownerActor && "ownerActor is invalid");

		/* 초기 생성 위치 정보를 Float 형으로 저장해둔다.*/
		currentWorldPosition = static_cast<Vector2Float>(ownerActor->GetWorldPosition());
	}

	void MovementComponent::Tick(float deltaTime)
	{
		super::Tick(deltaTime);

		/* 이동 방향이 존재할때만 이동 딜레이 타이머 작동 */
		if (lastMoveDirection != Vector2Float::Zero)
		{
			UpdateMovement(deltaTime);
		}

		/*if (lastMoveDirection != Vector2Int::Zero)
		{
			moveTimer.Tick(deltaTime);

			if (moveTimer.IsTimeOut())
			{
				UpdateMovement();
				moveTimer.Reset();
			}
		}*/
	}

	//void MovementComponent::SetLastMoveDirection(const Vector2Int& moveDirection)
	void MovementComponent::SetLastMoveDirection(const Vector2Float& moveDirection)
	{
		lastMoveDirection = moveDirection;
		lastMoveDirection.Normalize();		
		
		//if (lastMoveDirection == Vector2Int::Zero)
		//{
			/* 이동 방향이 존재하지 않으면 타이머 초기화 */
		//	moveTimer.Reset();
		//}
	}

	//void MovementComponent::UpdateMovement()
	void MovementComponent::UpdateMovement(const float deltaTime)
	{
		std::shared_ptr<Actor> ownerActor = GetOwner();
		assert(ownerActor && "ownerActor is invalid");

		std::shared_ptr<Level> level = ownerActor->GetOwner();
		assert(level && "level is invalid");

		/* 현재 타일 위치 */
		const Vector2Int& currentTilecoord = ownerActor->GetWorldPosition();

		/* 이동값 계산(방향 * 속도 * 델타타임) */
		const Vector2Float moveDelta = lastMoveDirection * moveSpeed * deltaTime;

		/* 이동할 새로운 위치 */
		const Vector2Float newPosition = currentWorldPosition + moveDelta;

		/* 점유할 타일 위치 (round) */
		const Vector2Int newTileCoord(static_cast<int>(round(newPosition.x)), static_cast<int>(round(newPosition.y)));

		/* 이동할 타일위치가 현재위치와 같으면 */
		if (currentTilecoord == newTileCoord)
		{
			//실수 위치만 갱신
			currentWorldPosition = newPosition;
		}
		else
		{
			//브레젠험 직선그리기로 도착지점까지의 경로 타일들을 구한다.
			std::vector<Vector2Int> pathTiles;
			StaticFunctionLibrary::GetBresenhamPath(currentTilecoord, newTileCoord, pathTiles);
			assert(!pathTiles.empty() && "pathTiles empty..");

			//브레젠험으로 구한 경로의 첫 시작은 현재타일위치와 같음
			auto iterMoveNextTileCoord = pathTiles.begin();

			//마지막으로 이동가능이 확인된 타일 위치
			auto iterlastMoveEnableTileCoord = iterMoveNextTileCoord;

			//시작 바로 다음 경로부터 체크 시작
			++iterMoveNextTileCoord;
			
			/* 각 경로타일에서 막히는 경로가 있는지(벽, 다른 Pawn) 확인해서 블록된 경로 바로 앞부분을 이동위치로 삼는다. */
			for (; iterMoveNextTileCoord != pathTiles.end(); ++iterMoveNextTileCoord)
			{
				const Vector2Int& checkTileCoord = *iterMoveNextTileCoord;
				if (!level->CanNextMove(ownerActor, checkTileCoord))
				{
					break;
				}

				iterlastMoveEnableTileCoord = iterMoveNextTileCoord;
			}

			/* 마지막에 이동가능한 위치로 확인된 타일 인덱스 */
			const Vector2Int& lastCheckEanbleMoveCoord = *iterlastMoveEnableTileCoord;
			
			/* 이동 가능한 위치가 현재위치와 같으면 => 블록때문에 못움직이므로 리턴한다. */
			if (lastCheckEanbleMoveCoord == currentTilecoord)
			{
				return;
			}

			/* 그게 아니면 이동이 실제 가능하므로 새로운 위치로 Owner의 타일 위치를 갱신한다. */
			ownerActor->SetPosition(lastCheckEanbleMoveCoord);
			
			/* 처음 예상한 위치와 실제 이동 가능한 위치가 다른경우 float형 위치값은 실제 이동 가능한 위치로 갱신해줘야한다. */
			if (lastCheckEanbleMoveCoord != newTileCoord)
			{
				currentWorldPosition = static_cast<Vector2Float>(lastCheckEanbleMoveCoord);
			}
			else
			{
				currentWorldPosition = newPosition;
			}

			/* 이동이 실제 가능한지 점검(타일 기준) */
			//if (level->CanNextMove(ownerActor, newTileCoord))
			//{
			//	/* 이동할 새로운 위치 지정 */
			//	ownerActor->SetPosition(newTileCoord);
			//	currentWorldPosition = newPosition;
			//}
			//else
			//{
			//	/* TODO : 현재 타일 위치에서 브레젠험알고리즘으로 직선 경로를 만든 뒤, 이동가능한 마지막 위치를 찾아서 움직여야 한다. */
			//	char szTmp[256] = { 0 };
			//	sprintf_s(szTmp, "Can't Move, currentPosition[%f, %f], newPosition[%f, %f] \n", currentWorldPosition.x, currentWorldPosition.y, newPosition.x, newPosition.y);
			//	OutputDebugStringA(szTmp);
			//}
		}

		///* 이동이 실제 가능한지 점검 필요 */
		//std::shared_ptr<Level> level = ownerActor->GetOwner();
		//assert(level && "level is invalid");
		//if (level->CanNextMove(ownerActor, newPosition))
		//{
		//	/* 이동할 새로운 위치 지정 */
		//	ownerActor->SetPosition(newPosition);
		//}
		//else if ((lastMoveDirection.x != 0) && (lastMoveDirection.y != 0))
		//{
		//	/* 대각 이동인 상태에서 이동이 막혔던 경우 막혀있지 않은 한쪽 축 방향으로 이동한다. */
		//	const Vector2Int newXPosition = currentPosition + Vector2Int(lastMoveDirection.x, 0);
		//	const Vector2Int newYPosition = currentPosition + Vector2Int(0, lastMoveDirection.y);
		//	const bool canMoveXPos = level->CanNextMove(ownerActor, newXPosition);
		//	const bool canMoveYPos = level->CanNextMove(ownerActor, newYPosition);

		//	if (canMoveXPos && !canMoveYPos)
		//	{
		//		ownerActor->SetPosition(newXPosition);
		//	}
		//	else if (!canMoveXPos && canMoveYPos)
		//	{
		//		ownerActor->SetPosition(newYPosition);
		//	}
		//	else if (canMoveXPos && canMoveYPos)
		//	{
		//		/* 대각이동은 막혔으나 x, y 개별축으로는 이동이 가능한 상태 */
		//		/* 이 경우 x축으로 이동 */
		//		ownerActor->SetPosition(newXPosition);
		//	}
		//}

		///* 이동 방향 벡터 초기화 */
		//lastMoveDirection = Vector2Int::Zero;
	}

	//void MovementComponent::UpdateMovement()
	//{
	//	/* 이동방향 벡터의 크기가 있을때만 작동 */
	//	if (lastMoveDirection == Vector2Int::Zero)
	//	{
	//		return;
	//	}

	//	std::shared_ptr<Actor> ownerActor = GetOwner();
	//	assert(ownerActor && "ownerActor is invalid");

	//	/* 현재 위치(Local 기준) */
	//	const Vector2Int currentPosition = ownerActor->GetPosition();

	//	/* 이동할 새로운 위치(Local 기준) */
	//	const Vector2Int newPosition = currentPosition + lastMoveDirection;

	//	/* 이동이 실제 가능한지 점검 필요 */
	//	std::shared_ptr<Level> level = ownerActor->GetOwner();
	//	assert(level && "level is invalid");
	//	if (level->CanNextMove(ownerActor, newPosition))
	//	{
	//		/* 이동할 새로운 위치 지정 */
	//		ownerActor->SetPosition(newPosition);
	//	}
	//	else if((lastMoveDirection.x != 0) && (lastMoveDirection.y != 0))
	//	{
	//		/* 대각 이동인 상태에서 이동이 막혔던 경우 막혀있지 않은 한쪽 축 방향으로 이동한다. */
	//		const Vector2Int newXPosition = currentPosition + Vector2Int(lastMoveDirection.x, 0);
	//		const Vector2Int newYPosition = currentPosition + Vector2Int(0, lastMoveDirection.y);
	//		const bool canMoveXPos = level->CanNextMove(ownerActor, newXPosition);
	//		const bool canMoveYPos = level->CanNextMove(ownerActor, newYPosition);

	//		if (canMoveXPos && !canMoveYPos)
	//		{
	//			ownerActor->SetPosition(newXPosition);
	//		}
	//		else if (!canMoveXPos && canMoveYPos)
	//		{
	//			ownerActor->SetPosition(newYPosition);
	//		}
	//		else if (canMoveXPos && canMoveYPos)
	//		{
	//			/* 대각이동은 막혔으나 x, y 개별축으로는 이동이 가능한 상태 */
	//			/* 이 경우 x축으로 이동 */
	//			ownerActor->SetPosition(newXPosition);
	//		}
	//	}

	//	/* 이동 방향 벡터 초기화 */
	//	lastMoveDirection = Vector2Int::Zero;
	//}
}