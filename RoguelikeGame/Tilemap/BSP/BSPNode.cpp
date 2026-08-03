#include "BSPNode.h"
#include "Util/Util.h"
#include "Tilemap/Room/Room.h"
#include <cassert>

//방의 최소 가로/세로 길이
static const int MinRoomLength = 10;

//방을 감싸는 벽 길이
static const int RoomWallLength = 2;

//방의 최소 길이 + 방의 양면 벽길이(상하 / 좌우) 
static const int MinLength = MinRoomLength + (RoomWallLength * 2);

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
		if (LeftWidth <= MinLength || RightWidth <= MinLength)
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
		if (LeftHeight <= MinLength || RightHeight <= MinLength)
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
	std::vector<const Room*> leftRoomLists;
	assert(LeftChild && "LeftChild Invalid..");
	LeftChild->GetRoomLists(leftRoomLists);

	//Right Child에 위치한 Room들
	std::vector<const Room*> rightRoomLists;
	assert(RightChild && "RightChild Invalid..");
	RightChild->GetRoomLists(rightRoomLists);

	//LeftChild와 RightChild에서 가장 가까운 쌍을 찾는다.
	float MinDistance = FLT_MAX;
	const Room* bestLeftRoom = nullptr;
	const Room* bestRightRoom = nullptr;
	for (const Room* leftRoom : leftRoomLists)
	{
		for (const Room* rightRoom : rightRoomLists)
		{
			const Vector2 vectorBetweenRoom = leftRoom->GetPositionCenter() - rightRoom->GetPositionCenter();
			
			//벡터길이 구하기(sqrt 생략)
			const float Distance = static_cast<float>(vectorBetweenRoom.x * vectorBetweenRoom.x) + 
													(vectorBetweenRoom.y * vectorBetweenRoom.y);

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
	const int RoomMaxWidth = Width - (RoomWallLength * 2);
	const int RoomWidth = Util::RandomRange(MinRoomLength, RoomMaxWidth);
	const int RoomMaxHeight = Height - (RoomWallLength * 2);
	const int RoomHeight = Util::RandomRange(MinRoomLength, RoomMaxHeight);

	//방 위치(좌상단) 랜덤 결정
	const int xPosRangeMin = StartPosition.x + RoomWallLength;
	const int xPosRangeMax = xPosRangeMin + (RoomMaxWidth - RoomWidth);
	const int RoomStartXPos = Util::RandomRange(xPosRangeMin, xPosRangeMax);
	const int yPosRangeMin = StartPosition.y + RoomWallLength;
	const int yPosRangeMax = yPosRangeMin + (RoomMaxHeight - RoomHeight);
	const int RoomStartYPos = Util::RandomRange(yPosRangeMin, yPosRangeMax);

	room = std::make_unique<Room>(Vector2(RoomStartXPos, RoomStartYPos), RoomWidth, RoomHeight);
	assert(room);
	room->InitializeRoom();
}

void BSPNode::GetRoomLists(std::vector<const Room*>& outRoomLists)
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

void BSPNode::GeneratePathBetweenRooms(const Room& leftRoom, const Room& rightRoom)
{
	static const auto& GetAddValue = [](int Length)
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

	const Vector2& leftRoomCenter = leftRoom.GetPositionCenter();
	const Vector2& rightRoomCenter = rightRoom.GetPositionCenter();
	
	const int widthCurrenttoNext = rightRoomCenter.x - leftRoomCenter.x;
	const int heightCurrenttoNext = rightRoomCenter.y - leftRoomCenter.y;
	
	const int addX = GetAddValue(widthCurrenttoNext);
	const int addY = GetAddValue(heightCurrenttoNext);
	
	const int absWidth = abs(widthCurrenttoNext);
	const int absHeight = abs(heightCurrenttoNext);
	
	int xPos = leftRoomCenter.x;
	int yPos = leftRoomCenter.y;
	
	if (absWidth > absHeight)
	{
		//가로를 나눠서 경로 구성
		const int absHalfWidth = absWidth >> 1;
				
		//첫 절반 경로 생성
		const int DestinationXPos = leftRoomCenter.x + (absHalfWidth * addX);
		for (; xPos != DestinationXPos; xPos += addX)
		{
			//경로내의 타일 뚫기
			pathTileIndices.emplace_back(Vector2(xPos, yPos));
		}
	
		//상하 경로 생성
		for (; yPos != rightRoomCenter.y; yPos += addY)
		{
			//경로내의 타일 뚫기
			pathTileIndices.emplace_back(Vector2(xPos, yPos));
		}
	
		//나머지 절반 경로 생성
		for (; xPos != rightRoomCenter.x; xPos += addX)
		{
			//경로내의 타일 뚫기
			pathTileIndices.emplace_back(Vector2(xPos, yPos));
		}
	}
	else
	{
		//세로를 나눠서 경로 구성
		const int absHalfHeight = absHeight >> 1;
	
		//첫 절반 경로 생성
		const int DestinationYPos = leftRoomCenter.y + (absHalfHeight * addY);
		for (; yPos != DestinationYPos; yPos += addY)
		{
			//경로내의 타일 뚫기
			pathTileIndices.emplace_back(Vector2(xPos, yPos));
		}
	
		//좌우 경로 생성
		for (; xPos != rightRoomCenter.x; xPos += addX)
		{
			//경로내의 타일 뚫기
			pathTileIndices.emplace_back(Vector2(xPos, yPos));
		}
	
		//나머지 절반 경로 생성
		for (; yPos != rightRoomCenter.y; yPos += addY)
		{
			//경로내의 타일 뚫기
			pathTileIndices.emplace_back(Vector2(xPos, yPos));
		}
	}
}

const Room& BSPNode::GetRoom() const
{
	assert(room && "Invalid room..");
	return *room;
}
