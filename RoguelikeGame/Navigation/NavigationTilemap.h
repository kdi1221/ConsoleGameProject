#pragma once

#include "Navigation/NavigationBase.h"
#include "Types/Defines.h"
#include "Types/Enums.h"
#include <Defines/Defines.h>
#include <unordered_map>
#include <queue>

namespace Craft
{
	class Actor;
	class NavMovementComponent;
}

/* 타일맵 내에서 경로 찾기를 실행하는 Navigation 클래스 */
class NavigationTilemap : public Craft::NavigationBase
{
private:
	/* 직선 비용 */
	static constexpr int costStraight = 10;

	/* 대각선 비용 */
	static constexpr int costDiagonal = 14;

	/* 휴리스틱 가중치, 예상 거리에 곱하여 가깝다고 여겨지는 타일이 더 빨리 탐색되도록 한다. */
	static constexpr float heuristicWeight = 1.3f;

	/* 한 프레임당 최대 요청 처리 갯수 */
	static constexpr int maxProcessFindPathRequestFrame = 5;

	struct FNodePath
	{
	public:
		FNodePath()
		{

		}

		/* 오픈리스트에 들어갈 중간 경로 예상 노드들 생성자 */
		FNodePath(const Craft::Vector2Int& coord, int valueG, int valueH, const Craft::Vector2Int& parent)
			:pathCoord(coord)
			,G(valueG)
			,H(valueH)
			,parentCoord(parent)
		{

		}

	public:
		/* F = G + H*/
		int GetF() const
		{
			return G + H;
		}

	public:
		/* 경로 좌표 */
		Craft::Vector2Int pathCoord = Craft::Vector2Int::Zero;

		/* 시작위치부터 지금노드까지의 비용 (G) */
		int G = 0;

		/* 지금노드부터 도착위치까지 예상 비용 (H), 대각선 사용 */
		int H = 0;

		/* 해당 노드의 부모 노드 좌표 (경로 완성에 사용) */
		Craft::Vector2Int parentCoord = Craft::Vector2Int::Zero;
	};

	struct FNodePathCompare
	{
		bool operator()(const FNodePath& lhs, const FNodePath& rhs) const
		{
			return lhs.GetF() > rhs.GetF();
		}
	};

	/* 경로 찾기 요청 정보 */
	struct FRequestPathFind
	{
	public:
		/* 요청자 참조 포인터 */
		std::weak_ptr<Craft::NavMovementComponent> requester;

		/* 요청자의 Unique ID(요청자가 유효하지 않을 수 있으므로) */
		NavigationUniqueIDType requesterUniqueID = 0;

		/* 요청 Handle */
		RequestPathHandleType requestHandle = 0;
			
		Craft::Vector2Int startPos = Craft::Vector2Int::Zero;

		Craft::Vector2Int endPos = Craft::Vector2Int::Zero;

	public:
		FRequestPathFind()
		{

		}

		FRequestPathFind(std::shared_ptr<Craft::NavMovementComponent> requester, 
							const NavigationUniqueIDType uniqueID,
							const RequestPathHandleType requestHandle,
							const Craft::Vector2Int& startPos, 
							const Craft::Vector2Int& endPos)
			:requester(requester)
			,requesterUniqueID(uniqueID)
			,requestHandle(requestHandle)
			,startPos(startPos)
			,endPos(endPos)
		{

		}
	};

public:
	NavigationTilemap();
	virtual ~NavigationTilemap() = default;

public:
	/* 모아둔 경로 찾기 요청 처리 */
	virtual void ProcessPathFindRequests() override;

	/* 경로찾기 요청 */
	virtual Craft::eFindPathResult RequestFindPath(std::shared_ptr<Craft::NavMovementComponent> requester,
													const Craft::Vector2Int& startPos,
													const Craft::Vector2Int& endPos,
													RequestPathHandleType& outRequestPathHandle) override;

	/* 경로찾기 취소 요청 */
	virtual void CancelFindPathRequest(std::shared_ptr<Craft::NavMovementComponent> requester) override;





	virtual Craft::eFindPathResult FindPath(std::shared_ptr<Craft::Actor> agent,
									const Craft::Vector2Int& startPos,
									const Craft::Vector2Int& endPos,
									std::vector<Craft::Vector2Int>& resultPath) const override;

	/* agent가 prevPosition에서 nextPosition으로 이동가능한지 체크 */
	virtual bool SimulatePreviousToNextMove(std::shared_ptr<Craft::Actor> agent,
											const Craft::Vector2Int& prevPosition,
											const Craft::Vector2Int& nextPosition) const override;

	virtual bool CanNextMove(std::shared_ptr<Craft::Actor> agent, const Craft::Vector2Int& checkPos) const override;

	/* 타겟까지 이동 시뮬레이션 도중 충돌되는 대상이 있는지 여부 반환 */
	virtual CheckMoveResultType CheckEnableMoveToTargetPosition(std::shared_ptr<Craft::Actor> agent,
																const Craft::Vector2Int& checkPos,
																Craft::Vector2Int& enableMovePosition) const override;

	
public:
	/* 기존 레벨이 정리될 때 호출 */
	virtual void ResetCurrentLevel() override;

public:
	/* 해당 위치의 타일이 속한 방 인덱스 반환 */
	RoomDefines::UNIQUE_INDEX_TYPE GetRoomIndexInTile(const Craft::Vector2Int& tileCoord) const;

	/* 해당 위치의 타일 카테고리 반환*/
	eTileCategory GetTileCategory(const Craft::Vector2Int& tileCoord) const;

private:
	/* 이전에 경로 찾기 요청했던 요청자의 유니크 ID와 요청 Handle */
	std::unordered_map<NavigationUniqueIDType, RequestPathHandleType> requesterHandles;

	/* 경로 찾기 요청 정보가 담긴 큐 */
	std::queue<FRequestPathFind> queueRequestPathFind;

	/* 경로 찾기 요청에 대한 결과를 임시로 받을 버퍼 */
	std::vector<Craft::Vector2Int> cachedPathFindBuffer;
};

