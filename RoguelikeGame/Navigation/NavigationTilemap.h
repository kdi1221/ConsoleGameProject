#pragma once

#include "Navigation/NavigationBase.h"
#include "Types/Defines.h"
#include "Types/Enums.h"
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
	static constexpr int maxProcessFindPathRequestFrame = 30;

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
		std::weak_ptr<Craft::NavMovementComponent> requester;
			
		Craft::Vector2Int startPos = Craft::Vector2Int::Zero;

		Craft::Vector2Int endPos = Craft::Vector2Int::Zero;

	public:
		FRequestPathFind()
		{

		}

		FRequestPathFind(std::shared_ptr<Craft::NavMovementComponent> requester, const Craft::Vector2Int& startPos, const Craft::Vector2Int& endPos)
			:requester(requester)
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
	virtual void RequestFindPath(std::shared_ptr<Craft::NavMovementComponent> requester,
								const Craft::Vector2Int& startPos,
								const Craft::Vector2Int& endPos) override;







	virtual Craft::eFindPathResult FindPath(std::shared_ptr<Craft::Actor> agent,
									const Craft::Vector2Int& startPos,
									const Craft::Vector2Int& endPos,
									std::vector<Craft::Vector2Int>& resultPath) const override;

	virtual bool CanNextMove(std::shared_ptr<Craft::Actor> agent, const Craft::Vector2Int& checkPos) const override;

	/* 타겟까지 이동 시뮬레이션 도중 충돌되는 대상이 있는지 여부 반환 */
	virtual CheckMoveResultType CheckEnableMoveToTargetPosition(std::shared_ptr<Craft::Actor> agent,
																const Craft::Vector2Int& checkPos,
																Craft::Vector2Int& enableMovePosition) const override;

public:
	/* 해당 위치의 타일이 속한 방 인덱스 반환 */
	RoomDefines::UNIQUE_INDEX_TYPE GetRoomIndexInTile(const Craft::Vector2Int& tileCoord) const;

	/* 해당 위치의 타일 카테고리 반환*/
	eTileCategory GetTileCategory(const Craft::Vector2Int& tileCoord) const;

private:
	/* 경로 찾기 요청 정보가 담긴 큐 */
	std::queue<FRequestPathFind> queueRequestPathFind;
};

