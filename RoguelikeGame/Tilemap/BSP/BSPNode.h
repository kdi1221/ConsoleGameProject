#pragma once

#include "Math/Vector2.h"
#include <memory>
#include <functional>
#include <vector>

class RoomSpace;

class BSPNode
{
public:
	//분할 방향
	enum class eDivideDirection
	{
		None = 0,
		//수평으로 분할
		Horizontal,
		//수직으로 분할
		Vertical
	};

	//BSP 노드 카테고리
	enum class eNodeCategory
	{
		None = 0,
		//통로
		Corridor,
		//방
		Room
	};

public:
	BSPNode(int inStartX, int inStartY, int inWidth, int inHeight);
	virtual ~BSPNode();

public:
	void Divide();
	void ConnectRooms();

	/* 각 노드를 순회하면서 노드 종류(경로, 방)에 따라 */
	void ExtractNodeContents(std::function<void(const std::vector<Craft::Vector2>&)> CorridorCallback,
						std::function<void(std::unique_ptr<RoomSpace>)> RoomCallback);

public:
	inline const Craft::Vector2& GetStartPosition() const { return StartPosition; }
	inline int GetWidth() const { return Width; }
	inline int GetHeight() const { return Height; }
	inline eNodeCategory GetNodeCategory() const { return NodeCategory; }

private:
	//방 내의 공간 계산 
	void GenerateRoomSpace();

	//자식노드들에 위치한 모든 방 리스트 반환 
	void GetRoomLists(std::vector<RoomSpace*>& outRoomLists);

	//두 방 사이의 경로 생성
	void GeneratePathBetweenRooms(RoomSpace& leftRoom, RoomSpace& rightRoom);

private:
	//분할된 공간의 시작 위치
	Craft::Vector2 StartPosition = Craft::Vector2::Zero;

	//분할된 공간의 가로 길이
	int Width = 0;

	//분할된 공간의 세로 길이
	int Height = 0;

	//노드 종류(통로 / 방)
	eNodeCategory NodeCategory = eNodeCategory::None;

	//분할 방향
	eDivideDirection DivideDirection = eDivideDirection::None;

	//노드 공간 내 위치한 방 정보
	std::unique_ptr<RoomSpace> roomSpace;

	//통로인 경우 Left와 Right를 연결하는 경로 타일 인덱스
	std::vector<Craft::Vector2> pathTileIndices;

	//왼쪽 트리 노드
	std::unique_ptr<BSPNode> LeftChild;

	//오른쪽 트리 노드
	std::unique_ptr<BSPNode> RightChild;
};

