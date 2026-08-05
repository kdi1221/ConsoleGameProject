#include "BSPNode.h"
#include "Util/Util.h"
#include "Tilemap/Room/Room.h"
#include "Math/Vector2Float.h"
#include "Types/Enums.h"
#include <queue>
#include <cassert>

namespace BSPRoomConsts
{
	//방의 최소 가로/세로 길이
	static constexpr int MinRoomLength = 10;

	//방을 감싸는 벽 길이
	static constexpr int RoomWallLength = 2;

	//방의 최소 길이 + 방의 양면 벽길이(상하 / 좌우) 
	static constexpr int MinLength = MinRoomLength + (RoomWallLength * 2);
}

using namespace Craft;

BSPNode::BSPNode(int inStartX, int inStartY, int inWidth, int inHeight)
	:StartPosition(inStartX, inStartY)
	,Width(inWidth)
	,Height(inHeight)
{

}

BSPNode::~BSPNode()
{

}

//BSP 맵 공간 분할
void BSPNode::Divide()
{
	//분할 비율 결정
	const float DivideRatio = Util::RandomRange(4.f, 6.f);

	if (Width > Height)
	{
		//가로가 세로보다 클 때 => 수직으로 분할
		DivideDirection = eDivideDirection::Vertical;

		const int LeftWidth = static_cast<int>(Width * (DivideRatio / 10.f));
		const int RightStartPosX = StartPosition.x + LeftWidth;
		const int RightWidth = Width - LeftWidth;

		//분할될 공간 중 한쪽이 최소 길이보다 작은경우 분할하지 않고 반환
		if (LeftWidth <= BSPRoomConsts::MinLength || RightWidth <= BSPRoomConsts::MinLength)
		{
			NodeCategory = eNodeCategory::Room;
			GenerateRoom();
			return;
		}
		
		LeftChild = std::make_unique<BSPNode>(StartPosition.x, StartPosition.y, LeftWidth, Height);
		RightChild = std::make_unique<BSPNode>(RightStartPosX, StartPosition.y, RightWidth, Height);
	}
	else
	{
		//세로가 가로보다 클 때 -> 수평으로 분할
		DivideDirection = eDivideDirection::Horizontal;

		const int LeftHeight = static_cast<int>(Height * (DivideRatio / 10.f));
		const int RightStartPosY = StartPosition.y + LeftHeight;
		const int RightHeight = Height - LeftHeight;

		//분할될 공간 중 한쪽이 최소 길이보다 작은경우 분할하지 않고 반환
		if (LeftHeight <= BSPRoomConsts::MinLength || RightHeight <= BSPRoomConsts::MinLength)
		{
			NodeCategory = eNodeCategory::Room;
			GenerateRoom();
			return;
		}
		
		LeftChild = std::make_unique<BSPNode>(StartPosition.x, StartPosition.y, Width, LeftHeight);
		RightChild = std::make_unique<BSPNode>(StartPosition.x, RightStartPosY, Width, RightHeight);
	}

	NodeCategory = eNodeCategory::Corridor;

	//Left, Right 각각 분할 실행
	assert(LeftChild && "LeftChild Invalid..");
	LeftChild->Divide();

	assert(RightChild && "RightChild Invalid..");
	RightChild->Divide();
}

void BSPNode::GeneratePaths()
{
	//노드 종류가 방이면 경로 생성안함.
	if (eNodeCategory::Room == NodeCategory)
	{
		return;
	}

	//Left Child에 위치한 Room들
	std::vector<Room*> leftRoomLists;
	assert(LeftChild && "LeftChild Invalid..");
	LeftChild->GetRoomLists(leftRoomLists);

	//Right Child에 위치한 Room들
	std::vector<Room*> rightRoomLists;
	assert(RightChild && "RightChild Invalid..");
	RightChild->GetRoomLists(rightRoomLists);

	//LeftChild와 RightChild에서 가장 가까운 쌍을 찾는다.
	float MinDistance = FLT_MAX;
	Room* bestLeftRoom = nullptr;
	Room* bestRightRoom = nullptr;
	for (Room* leftRoom : leftRoomLists)
	{
		for (Room* rightRoom : rightRoomLists)
		{
			const Vector2Float leftRoomCenter = static_cast<Vector2Float>(leftRoom->GetPositionCenter());
			const Vector2Float rightRoomCenter = static_cast<Vector2Float>(rightRoom->GetPositionCenter());

			const Vector2Float vectorBetweenRoom = rightRoomCenter - leftRoomCenter;
			
			//벡터길이 구하기(sqrt 생략)
			const float Distance = vectorBetweenRoom.Length();

			if (MinDistance > Distance)
			{
				bestLeftRoom = leftRoom;
				bestRightRoom = rightRoom;
				MinDistance = Distance;
			}
		}
	}

	//가장 가까운 두 방 간의 경로타일을 생성한다.
	assert(bestLeftRoom && "Invalid Best Left Room");
	assert(bestRightRoom && "Invalid Best Right Room");
	GeneratePathBetweenRooms(*bestLeftRoom, *bestRightRoom);

	//Left Child의 경로 생성
	if (LeftChild)
	{
		LeftChild->GeneratePaths();
	}

	//Right Child의 경로 생성
	if (RightChild)
	{
		RightChild->GeneratePaths();
	}	
}

void BSPNode::Foreach_Node(std::function<void(const BSPNode&)> CallbackFunc) const
{
	CallbackFunc(*this);

	if (LeftChild)
	{
		LeftChild->Foreach_Node(CallbackFunc);
	}

	if (RightChild)
	{
		RightChild->Foreach_Node(CallbackFunc);
	}
}

void BSPNode::GenerateRoom()
{
	//방 생성 크기 랜덤 결정
	const int RoomMaxWidth = Width - (BSPRoomConsts::RoomWallLength * 2);
	const int RoomWidth = Util::RandomRange(BSPRoomConsts::MinRoomLength, RoomMaxWidth);
	const int RoomMaxHeight = Height - (BSPRoomConsts::RoomWallLength * 2);
	const int RoomHeight = Util::RandomRange(BSPRoomConsts::MinRoomLength, RoomMaxHeight);

	//방 위치(좌상단) 랜덤 결정
	const int xPosRangeMin = StartPosition.x + BSPRoomConsts::RoomWallLength;
	const int xPosRangeMax = xPosRangeMin + (RoomMaxWidth - RoomWidth);
	const int RoomStartXPos = Util::RandomRange(xPosRangeMin, xPosRangeMax);
	const int yPosRangeMin = StartPosition.y + BSPRoomConsts::RoomWallLength;
	const int yPosRangeMax = yPosRangeMin + (RoomMaxHeight - RoomHeight);
	const int RoomStartYPos = Util::RandomRange(yPosRangeMin, yPosRangeMax);

	room = std::make_unique<Room>(Vector2(RoomStartXPos, RoomStartYPos), RoomWidth, RoomHeight);
	assert(room);
	room->InitializeRoom();
}

void BSPNode::GetRoomLists(std::vector<Room*>& outRoomLists)
{
	switch (NodeCategory)
	{
	case eNodeCategory::Corridor:
		{
			//통로 노드는 다시 Left, Right의 GetRoomLists를 가져와서 합친다.
			if (LeftChild)
			{
				LeftChild->GetRoomLists(outRoomLists);
			}

			if (RightChild)
			{
				RightChild->GetRoomLists(outRoomLists);
			}
		}
		break;

	case eNodeCategory::Room:
		{
			//방 노드는 자신의 Room을 outRoomLists에 담고 반환한다.
			outRoomLists.emplace_back(room.get());
		}
		break;
	}
}

void BSPNode::GeneratePathBetweenRooms(Room& leftRoom, Room& rightRoom)
{
	const Vector2& leftRoomCenter = leftRoom.GetPositionCenter();
	const Vector2& rightRoomCenter = rightRoom.GetPositionCenter();

	//두 방간의 방향을 구함(left => right)
	const Vector2 betweenRoomVector = rightRoomCenter - leftRoomCenter;
	Vector2Float toRightVectorDirection = static_cast<Vector2Float>(betweenRoomVector);
	toRightVectorDirection.Normalize();
	Vector2Float toLeftVectorDirection = toRightVectorDirection * -1.f;

	//주어진 방향 벡터가 방의 4방향벡터와 가장 가깝게 일치하는 면을 반환
	auto getRoomsFacingFlags = [](const Vector2Float& direction)
		{
			using EdgeDot = std::pair<eRoomFacingFlags, float>;

			auto compareDot = [](const EdgeDot& lhs, const EdgeDot& rhs)
				{
					return lhs.second < rhs.second;
				};

			//내적 결과 값이 가장큰 순서대로 정렬되는 우선순위 큐
			std::priority_queue<EdgeDot, std::vector<EdgeDot>, 
								std::function<bool(const EdgeDot&, const EdgeDot&)>> pQueue(compareDot);

			//Room의 4면의 방향과 방향벡터 내적을 우선순위 큐에 삽입
			pQueue.push(EdgeDot(eRoomFacingFlags::Up, Vector2Float::Up.DotProduct(direction)));
			pQueue.push(EdgeDot(eRoomFacingFlags::Down, Vector2Float::Down.DotProduct(direction)));
			pQueue.push(EdgeDot(eRoomFacingFlags::Left, Vector2Float::Left.DotProduct(direction)));
			pQueue.push(EdgeDot(eRoomFacingFlags::Right, Vector2Float::Right.DotProduct(direction)));

			//내적의 크기가 가장 큼 == 방향이 가장 일치하는 면을 반환
			return pQueue.top().first;
		};

	const eRoomFacingFlags leftRoomFaceFlag = getRoomsFacingFlags(toRightVectorDirection);
	const eRoomFacingFlags rightRoomFaceFlag = getRoomsFacingFlags(toLeftVectorDirection);

	//eRoomBetweenFace -> eRoomSide 맵핑
	auto mappingRoomFaceToSide = [](const eRoomFacingFlags FaceFlag)
		{
			switch (FaceFlag)
			{
			case eRoomFacingFlags::Left:
				return eRoomSides::Left;
			case eRoomFacingFlags::Up:
				return eRoomSides::Top;
			case eRoomFacingFlags::Right:
				return eRoomSides::Right;
			case eRoomFacingFlags::Down:
				return eRoomSides::Bottom;
			}

			//4면 중 하나의 케이스로 반드시 처리가 되어야 함
			assert(false);
			return eRoomSides::Left;
		};

	//왼쪽 자식 방의 외곽 타일 중 하나 선택
	Vector2 selectLeftOuterTile = leftRoom.SelectDoorTile(mappingRoomFaceToSide(leftRoomFaceFlag));

	//오른쪽 자식 방의 외곽 타일 중 하나 선택
	Vector2 selectRightOuterTile = rightRoom.SelectDoorTile(mappingRoomFaceToSide(rightRoomFaceFlag));

	//Length의 양수, 음수, 0에 따른 증가 인덱스 반환
	auto getAddValue = [](int Length)
		{
			if (Length == 0)
			{
				return 0;
			}
			else
			{
				return Length > 0 ? 1 : -1;
			}
		};

	// 두 방이 마주한 면의 조합별로 처리한다.
	switch (leftRoomFaceFlag | rightRoomFaceFlag)
	{
	case eRoomFacingFlags::Right_Left:
		{
			//왼쪽방에서 뻗어 나오는 방향이 오른쪽이 아니라면
			if (eRoomFacingFlags::Right != leftRoomFaceFlag)
			{
				//Swap하여 LeftRoom => RightRoom상황으로 바꿈
				std::swap(selectLeftOuterTile, selectRightOuterTile);
			}

			const Vector2 TileNums = selectRightOuterTile - selectLeftOuterTile;

			const int addX = getAddValue(TileNums.x);
			const int addY = getAddValue(TileNums.y);
			const int widthHalfNum = abs(TileNums.x) >> 1;

			int xPos = selectLeftOuterTile.x;
			int yPos = selectLeftOuterTile.y;

			//첫 절반 x 경로 생성.
			const int halfXDestinationPos = selectLeftOuterTile.x + (widthHalfNum * addX);
			for (; xPos != halfXDestinationPos; xPos += addX)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2(xPos, yPos));
			}

			//y 경로 생성
			for (; yPos != selectRightOuterTile.y; yPos += addY)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2(xPos, yPos));
			}

			//나머지 절반 x 경로 생성.
			for (; xPos != selectRightOuterTile.x; xPos += addX)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2(xPos, yPos));
			}

			//마지막 타일 뚫기
			pathTileIndices.emplace_back(Vector2(xPos, yPos));
		}
		break;

	case eRoomFacingFlags::Down_Up:
		{
			//왼쪽방에서 뻗어 나오는 방향이 아래쪽이 아니라면
			if (eRoomFacingFlags::Down != leftRoomFaceFlag)
			{
				//Swap하여 LeftRoom => RightRoom상황으로 바꿈
				std::swap(selectLeftOuterTile, selectRightOuterTile);
			}

			const Vector2 TileNums = selectRightOuterTile - selectLeftOuterTile;

			const int addX = getAddValue(TileNums.x);
			const int addY = getAddValue(TileNums.y);
			const int heightHalfNum = abs(TileNums.y) >> 1;

			int xPos = selectLeftOuterTile.x;
			int yPos = selectLeftOuterTile.y;

			//첫 절반 y 경로 생성.
			const int halfYDestinationPos = selectLeftOuterTile.y + (heightHalfNum * addY);
			for (; yPos != halfYDestinationPos; yPos += addY)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2(xPos, yPos));
			}

			//x 경로 생성
			for (; xPos != selectRightOuterTile.x; xPos += addX)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2(xPos, yPos));
			}

			//나머지 절반 y 경로 생성.
			for (; yPos != selectRightOuterTile.y; yPos += addY)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2(xPos, yPos));
			}

			//마지막 타일 뚫기
			pathTileIndices.emplace_back(Vector2(xPos, yPos));

		}
		break;

	case eRoomFacingFlags::Up_Left:
	case eRoomFacingFlags::Down_Left:
	case eRoomFacingFlags::Up_Right:
	case eRoomFacingFlags::Down_Right:
		{
			//왼쪽방에서 뻗어 나오는 방향이 위, 아래 방향이 아니라면
			if (eRoomFacingFlags::Up != leftRoomFaceFlag && eRoomFacingFlags::Down != leftRoomFaceFlag)
			{
				//Swap하여 LeftRoom => RightRoom상황으로 바꿈
				std::swap(selectLeftOuterTile, selectRightOuterTile);
			}

			const Vector2 TileNums = selectRightOuterTile - selectLeftOuterTile;

			const int addX = getAddValue(TileNums.x);
			const int addY = getAddValue(TileNums.y);

			int xPos = selectLeftOuterTile.x;
			int yPos = selectLeftOuterTile.y;

			//y 경로 생성.
			for (; yPos != selectRightOuterTile.y; yPos += addY)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2(xPos, yPos));
			}

			//x 경로 생성
			for (; xPos != selectRightOuterTile.x; xPos += addX)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2(xPos, yPos));
			}

			//마지막 타일 뚫기
			pathTileIndices.emplace_back(Vector2(xPos, yPos));

		}
		break;
	}
}

const Room& BSPNode::GetRoom() const
{
	assert(room && "Invalid room..");
	return *room;
}
