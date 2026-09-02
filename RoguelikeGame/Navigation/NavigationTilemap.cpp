#include "NavigationTilemap.h"
#include "Level/TilemapLevel.h"
#include "Component/NavMovementComponent.h"
#include <StaticLibrary/StaticFunctionLibrary.h>
#include <Math/Vector2Float.h>
#include <Actor/Actor.h>
#include <cassert>
#include <queue>
#include <unordered_map>

using namespace Craft;

NavigationTilemap::NavigationTilemap()
{
	
}

void NavigationTilemap::ProcessPathFindRequests()
{
	/* 처리된 경로 찾기 요청 횟수 */
	int countProcessRequest = 0;

	/* 요청 처리 최대횟수를 안넘기고, 큐에 요청이 남아있는동안 반복 */
	while (countProcessRequest < maxProcessFindPathRequestFrame && !queueRequestPathFind.empty())
	{
		FRequestPathFind currentRequestPathFind = queueRequestPathFind.front();
		queueRequestPathFind.pop();

		//TODO : 경로 찾기 정보에 대한 유효성 검증 후 경로 찾기 실행

		++countProcessRequest;
	}
}

void NavigationTilemap::RequestFindPath(std::shared_ptr<NavMovementComponent> requester, 
										const Vector2Int& startPos, 
										const Vector2Int& endPos)
{
	if (!requester)
	{
		return;
	}

	std::shared_ptr<Actor> requesterOwner = requester->GetOwner(); 
	if (!requesterOwner || !requesterOwner->IsActive())
	{
		return;
	}

	queueRequestPathFind.push(FRequestPathFind(requester, startPos, endPos));
}

eFindPathResult NavigationTilemap::FindPath(std::shared_ptr<Actor> agent,
											const Vector2Int& startPos, 
											const Vector2Int& endPos, 
											std::vector<Vector2Int>& resultPath) const
{
	if (!agent)
	{
		return eFindPathResult::Fail;
	}

	std::shared_ptr<TilemapLevel> tilemapLevel = GetCurrentLevel<TilemapLevel>();
	if (!tilemapLevel)
	{
		return eFindPathResult::Fail;
	}

	/* 같은 위치라면 아무것도 하지 않는다. */
	if (startPos == endPos)
	{
		return eFindPathResult::Fail;
	}

	/* 목적지가 갈 수 없는 위치면 아무것도 하지 않는다. */
	if (CheckPlacementResult::CanMove != tilemapLevel->CanNextMove(agent, endPos))
	{
		return eFindPathResult::Fail;
	}

	/* A* 알고리즘 */
	auto getHeuristic = [&endPos](const Vector2Int& currentPos)
		{
			const int dx = std::abs(endPos.x - currentPos.x);
			const int dy = std::abs(endPos.y - currentPos.y);

			//x, y 두 축중 짧은 쪽과 긴쪽을 구분
			const int minValue = min(dx, dy);
			const int maxValue = max(dx, dy);

			//최소거리만큼은 대각선으로 이동, 나머지는 직선방향으로 움직임
			//예상거리에 가중치를 둬서 목표지점까지의 거리가 가깝다고 여겨지는 타일이 탐색순서상 더 앞으로 오도록 한다.
			return static_cast<int>(heuristicWeight * (minValue * costDiagonal) + ((maxValue - minValue) * costStraight));
		};

	/* 오픈 리스트 : 우선순위큐로 가장 비용이 적은 노드를 꺼내오도록 한다. */
	std::priority_queue<FNodePath, std::vector<FNodePath>, FNodePathCompare> pqOpenNodes;
	
	/* 클로즈 리스트 : 탐색이 끝난 노드의 정보를 저장한다. 이 때 자신을 최적 경로로 가지는 부모노드의 위치를 가리킨다. */
	std::unordered_map<Vector2Int, Vector2Int> mapCloseNodeCoords;
	
	/* 시작 위치를 오픈리스트에 삽입 */
	pqOpenNodes.push(FNodePath(startPos, 0, getHeuristic(startPos), startPos));

	/* 마지막으로 CloseList에 넣은 타일 좌표 */
	Vector2Int lastInsertClostListCoord(-1, -1);

	/* 탐색 결과(성공 또는 제한 걸림 )*/
	eFindPathResult findPathResult = eFindPathResult::None;

	while (!pqOpenNodes.empty())
	{
		/* 현재 오픈리스트에서 가장 비용이 작은 노드를 꺼내옴(현재노드) */
		const FNodePath currentNode = pqOpenNodes.top();
		pqOpenNodes.pop();

		/* 꺼낸 노드가 이전에 이미 최적 경로로 CloseList에 들어간 경우 제외함 */
		if (mapCloseNodeCoords.find(currentNode.pathCoord) != mapCloseNodeCoords.end())
		{
			continue;
		}

		/* 꺼내온 노드를 Close List에 넣는다. */
		mapCloseNodeCoords.insert(std::make_pair(currentNode.pathCoord, currentNode.parentCoord));

		/* 마지막에 CloseList에 삽입한 위치 저장 */
		lastInsertClostListCoord = currentNode.pathCoord;

		/* 꺼낸 노드가 도착 지점인 경우 탐색 종료 */
		if (currentNode.pathCoord == endPos)
		{
			findPathResult = eFindPathResult::Success;
			break;
		}

		/* Close List에 저장된 노드의 탐색 제한 갯수가 초과된 경우 벗어난다. */
		if (NavigationBase::LIMIT_PATH_FIND_NUM == mapCloseNodeCoords.size())
		{
			findPathResult = eFindPathResult::Throttled;
			break;
		}
		
		/* 현재 노드 주변 8방향 타일들을 확인 */
		for (int yAdd = -1; yAdd <= 1; ++yAdd)
		{
			for (int xAdd = -1; xAdd <= 1; ++xAdd)
			{
				/* 중심좌표, 즉 현재노드 자신은 처리하지 않는다. */
				if (xAdd == 0 && yAdd == 0)
				{
					continue;
				}

				const Vector2Int direction = Vector2Int(xAdd, yAdd);
				const Vector2Int checkTileCoord = currentNode.pathCoord + direction;

				/* 이미 클로즈 리스트에 들어간 타일은 제외한다. */
				if (mapCloseNodeCoords.find(checkTileCoord) != mapCloseNodeCoords.end())
				{
					continue;
				}

				const eTileCategory checkTileCategory = tilemapLevel->GetTileCategory(checkTileCoord);

				/* 유효하지 않은 좌표이거나(ex : -1, 0) 갈수 없는 타일(벽)인 경우 제외한다.*/
				if (eTileCategory::None == checkTileCategory || eTileCategory::Wall == checkTileCategory)
				{
					continue;
				}

				/* 다른 객체가 점유중인 타일은 제외한다. */
				if (CheckPlacementResult::CanMove != tilemapLevel->CanNextMove(agent, checkTileCoord))
				{
					continue;
				}

				/* 대각 방향의 타일일때 그 방향의 X,Y 분리 방향 타일이 막혀있으면 지나가지 못한다. */
				if (tilemapLevel->IsDiagonalBlocked(agent, currentNode.pathCoord, direction))
				{
					continue;
				}
				
				/* 현재 노드까지의 비용 + 현재 노드에서 체크중인 노드까지의 비용(대각 : 14, 직선 : 10)*/
				const int CurrentToCheckNodeCost = (1 == abs(yAdd) && 1 == abs(xAdd)) ? costDiagonal : costStraight;
				
				/* 위의 모든 조건들을 통과한 타일들은 오픈리스트에 비용을 계산하여 삽입한다. */
				pqOpenNodes.push(FNodePath(checkTileCoord, 
											currentNode.G + CurrentToCheckNodeCost, 
											getHeuristic(checkTileCoord), 
											currentNode.pathCoord));
			}
		}		
	}

	/* 여기서 None이라면 도착지점까지의 경로가 존재하지 않는다는 것 */
	if (findPathResult == eFindPathResult::None)
	{
		return eFindPathResult::Fail;
	}

	//Success, 또는 Throttled상태에서는 마지막 탐색 지점까지의 경로를 완성한다.
	resultPath.clear();
	auto iterFindEndPosNode = mapCloseNodeCoords.find(lastInsertClostListCoord);

	while (iterFindEndPosNode != mapCloseNodeCoords.end())
	{
		resultPath.emplace_back(iterFindEndPosNode->first);

		if (iterFindEndPosNode->first == startPos)
		{
			break;
		}

		iterFindEndPosNode = mapCloseNodeCoords.find(iterFindEndPosNode->second);
	}

	//마지막으로 역순으로 뒤집어서 시작->종료 경로순으로 완성한다.
	std::reverse(resultPath.begin(), resultPath.end());

	return findPathResult;



	//도착 지점이 map에 존재하는경우 성공적으로 경로탐색이 된 것	
	//resultPath.clear();
	//auto iterFindEndPosNode = mapCloseNodeCoords.find(endPos);

	////For Debug
	//const bool bFindPathSuccess = iterFindEndPosNode != mapCloseNodeCoords.end();
	//char szTmp[512] = { 0 };
	//sprintf_s(szTmp, "FindPath, Start[%d, %d] => End[%d, %d], %s, CloseNodeNum[%d]\n",
	//	startPos.x, startPos.y,
	//	endPos.x, endPos.y,
	//	bFindPathSuccess ? "Success" : "Fail",
	//	static_cast<int>(mapCloseNodeCoords.size()));
	//OutputDebugStringA(szTmp);

	//if (iterFindEndPosNode == mapCloseNodeCoords.end())
	//{
	//	//TODO : 최적화 지점? 애초에 가지 못하는 경로에는 접근 못하게??
	//	int a = 10;
	//	a = a;
	//}

	//while (iterFindEndPosNode != mapCloseNodeCoords.end())
	//{
	//	resultPath.emplace_back(iterFindEndPosNode->first);

	//	if (iterFindEndPosNode->first == startPos)
	//	{
	//		break;
	//	}

	//	iterFindEndPosNode = mapCloseNodeCoords.find(iterFindEndPosNode->second);
	//}

	////마지막으로 역순으로 뒤집어서 시작->종료 경로순으로 완성한다.
	//std::reverse(resultPath.begin(), resultPath.end());

	//const bool result = (!resultPath.empty()) &&
	//					(*resultPath.begin() == startPos) &&
	//					(*(resultPath.end() - 1) == endPos);

	//return result;
}

bool NavigationTilemap::CanNextMove(std::shared_ptr<Actor> agent, const Vector2Int& checkPos) const
{
	if (!agent)
	{
		return false;
	}

	if (agent->GetWorldPosition() == checkPos)
	{
		return false;
	}

	std::shared_ptr<TilemapLevel> tilemapLevel = GetCurrentLevel<TilemapLevel>();
	if (!tilemapLevel)
	{
		return false;
	}

	return CheckPlacementResult::CanMove == tilemapLevel->CanNextMove(agent, checkPos);
}

CheckMoveResultType NavigationTilemap::CheckEnableMoveToTargetPosition(std::shared_ptr<Craft::Actor> agent,
																		const Craft::Vector2Int& checkPos, 
																		Vector2Int& enableMovePosition) const
{
	if (!agent || !agent->IsActive())
	{
		return eCheckMoveTargetResult::Unknown;
	}

	std::shared_ptr<TilemapLevel> tilemapLevel = GetCurrentLevel<TilemapLevel>();
	if (!tilemapLevel)
	{
		return eCheckMoveTargetResult::Unknown;
	}

	const Vector2Int& currentTilecoord = agent->GetWorldPosition();

	//브레젠험 직선그리기로 도착지점까지의 경로 타일들을 구한다.
	std::vector<Vector2Int> pathTiles;
	StaticFunctionLibrary::GetBresenhamPath(currentTilecoord, checkPos, pathTiles);
	assert(!pathTiles.empty() && "pathTiles empty..");

	//브레젠험으로 구한 경로의 첫 시작은 현재타일위치와 같음
	auto iterMoveNextTileCoord = pathTiles.begin();

	//마지막으로 이동가능이 확인된 타일 위치
	auto iterlastMoveEnableTileCoord = iterMoveNextTileCoord;

	//시작 바로 다음 경로부터 체크 시작
	++iterMoveNextTileCoord;

	//반환 결과 값
	CheckMoveResultType checkResult = CheckMoveResultType::None;
			
	/* 각 경로타일에서 막히는 경로가 있는지(벽, 다른 Pawn) 확인해서 블록된 경로 바로 앞부분을 이동위치로 삼는다. */
	for (; iterMoveNextTileCoord != pathTiles.end(); ++iterMoveNextTileCoord)
	{
		const Vector2Int& checkTileCoord = *iterMoveNextTileCoord;

		//이동 불가능한 이유에 대해 Return 값 결정
		const CheckPlacementResult checkMoveResult = tilemapLevel->CanNextMove(agent, checkTileCoord);
		if (checkMoveResult != CheckPlacementResult::CanMove)
		{
			switch (checkMoveResult)
			{
			case CheckPlacementResult::BlockActor:
				{
					//충돌한 액터 존재
					checkResult = CheckMoveResultType::BlockActor;
				}
				break;

			case CheckPlacementResult::BlockWall:
				{
					//충돌한 벽 존재
					checkResult = CheckMoveResultType::BlockWall;
				}
				break;

			default:
				{
					//기타
					checkResult = CheckMoveResultType::Unknown;
				}
				break;
			}

			/* 중간에 충돌되었으므로 더이상 체크하지 않고 빠져나옴 */
			break;
		}
		
		/* 마지막으로 이동가능이 확인된 타일 위치 저장 */
		iterlastMoveEnableTileCoord = iterMoveNextTileCoord;
	}

	/* 여기까지 왔는데 checkResult가 None이면 경로상의 충돌된 대상이 없다는것 */
	checkResult = (checkResult == CheckMoveResultType::None) ? CheckMoveResultType::Success : checkResult;

	/* 마지막으로 이동 가능이 확인된 타일 위치 반환 */
	enableMovePosition = *iterlastMoveEnableTileCoord;

	return checkResult;
}

RoomDefines::UNIQUE_INDEX_TYPE NavigationTilemap::GetRoomIndexInTile(const Craft::Vector2Int& tileCoord) const
{
	std::shared_ptr<TilemapLevel> tilemapLevel = GetCurrentLevel<TilemapLevel>();
	if (!tilemapLevel)
	{
		return RoomDefines::ROOM_INDEX_INVALID;
	}

	return tilemapLevel->GetRoomIndexInTile(tileCoord);
}

eTileCategory NavigationTilemap::GetTileCategory(const Craft::Vector2Int& tileCoord) const
{
	std::shared_ptr<TilemapLevel> tilemapLevel = GetCurrentLevel<TilemapLevel>();
	if (!tilemapLevel)
	{
		return eTileCategory::None;
	}

	return tilemapLevel->GetTileCategory(tileCoord);
}