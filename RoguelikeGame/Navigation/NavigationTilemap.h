#pragma once

#include "Navigation/NavigationBase.h"
#include "Types/Defines.h"
#include "Types/Enums.h"

namespace Craft
{
	class Actor;
}

/* 타일맵 내에서 경로 찾기를 실행하는 Navigation 클래스 */
class NavigationTilemap : public Craft::NavigationBase
{
private:
	/* 직선 비용 */
	static constexpr int costStraight = 10;

	/* 대각선 비용 */
	static constexpr int costDiagonal = 14;

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

public:
	NavigationTilemap();
	virtual ~NavigationTilemap() = default;

public:
	virtual bool FindPath(std::shared_ptr<Craft::Actor> agent,
						const Craft::Vector2Int& startPos,
						const Craft::Vector2Int& endPos,
						std::vector<Craft::Vector2Int>& resultPath) const override;

	virtual bool CanNextMove(std::shared_ptr<Craft::Actor> agent, const Craft::Vector2Int& checkPos) const override;

public:
	/* 해당 위치의 타일이 속한 방 인덱스 반환 */
	RoomDefines::UNIQUE_INDEX_TYPE GetRoomIndexInTile(const Craft::Vector2Int& tileCoord) const;

	/* 해당 위치의 타일 카테고리 반환*/
	eTileCategory GetTileCategory(const Craft::Vector2Int& tileCoord) const;
};

