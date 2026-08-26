#include "BSPNode.h"
#include "Engine/Engine.h"
#include "Game/Config/Config.h"
#include "Util/Util.h"
#include "RoomSpace/RoomSpace.h"
#include "Math/Vector2Float.h"
#include "Types/Enums.h"
#include <queue>
#include <cassert>

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
	//설정에 지정된 방의 최소길이 및 두께 정보 가져오기
	const Config& config = Engine::Get().GetConfig<Config>();
	const int minRoomWidth = config.GetBSPMinRoomWidth();
	const int minRoomHeight = config.GetBSPMinRoomHeight();
	const int wallThickness = config.GetBSPRoomWallThickness();

	//방의 최소 가로 길이 + 방의 좌우 벽길이
	const int minWidth = minRoomWidth + (wallThickness * 2);

	//방의 최소 세로 길이 + 방의 상하 벽길이
	const int minHeight = minRoomHeight + (wallThickness * 2);

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
		if (LeftWidth <= minWidth || RightWidth <= minWidth)
		{
			NodeCategory = eNodeCategory::Room;
			GenerateRoomSpace(minRoomWidth, minRoomHeight, wallThickness);
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
		if (LeftHeight <= minHeight || RightHeight <= minHeight)
		{
			NodeCategory = eNodeCategory::Room;
			GenerateRoomSpace(minRoomWidth, minRoomHeight, wallThickness);
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

void BSPNode::ConnectRooms()
{
	//노드 종류가 방이면 경로 생성안함.
	if (eNodeCategory::Room == NodeCategory)
	{
		return;
	}

	//Left Child에 위치한 Room들
	std::vector<RoomSpace*> leftRoomLists;
	assert(LeftChild && "LeftChild Invalid..");
	LeftChild->GetRoomLists(leftRoomLists);

	//Right Child에 위치한 Room들
	std::vector<RoomSpace*> rightRoomLists;
	assert(RightChild && "RightChild Invalid..");
	RightChild->GetRoomLists(rightRoomLists);

	//LeftChild와 RightChild에서 가장 가까운 쌍을 찾는다.
	float MinDistance = FLT_MAX;
	RoomSpace* bestLeftRoom = nullptr;
	RoomSpace* bestRightRoom = nullptr;
	for (RoomSpace* leftRoom : leftRoomLists)
	{
		for (RoomSpace* rightRoom : rightRoomLists)
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
		LeftChild->ConnectRooms();
	}

	//Right Child의 경로 생성
	if (RightChild)
	{
		RightChild->ConnectRooms();
	}	
}

void BSPNode::ExtractNodeContents(std::function<void(const std::vector<Craft::Vector2Int>&)> CorridorCallback,
								std::function<void(std::unique_ptr<RoomSpace>)> RoomCallback)
{
	switch (NodeCategory)
	{
	case eNodeCategory::Corridor:
		{
			CorridorCallback(pathTileIndices);
		}
		break;

	case eNodeCategory::Room:
		{
			RoomCallback(std::move(roomSpace));

			//roomSpace 무효화
			roomSpace.reset();
		}
		break;
	}

	if (LeftChild)
	{
		LeftChild->ExtractNodeContents(CorridorCallback, RoomCallback);
	}
		
	if (RightChild)
	{
		RightChild->ExtractNodeContents(CorridorCallback, RoomCallback);
	}
}

void BSPNode::GenerateRoomSpace(const int minRoomWidth, const int minRoomHeight, const int wallThickness)
{
	//방 생성 크기 랜덤 결정
	const int RoomMaxWidth = Width - (wallThickness * 2);
	const int RoomWidth = Util::RandomRange(minRoomWidth, RoomMaxWidth);

	const int RoomMaxHeight = Height - (wallThickness * 2);
	//const int RoomHeight = Util::RandomRange(minRoomHeight, RoomMaxHeight);

	/* 방의 세로길이는는 방의 가로길이의 60프로 정도 차지해야 함 */
	const float roomHeightRatio = static_cast<float>(RoomWidth) * 0.6f;
	const int RoomHeight = min(max(static_cast<int>(roomHeightRatio), minRoomHeight), RoomMaxHeight);


	//방 위치(좌상단) 랜덤 결정
	const int xPosRangeMin = StartPosition.x + wallThickness;
	const int xPosRangeMax = xPosRangeMin + (RoomMaxWidth - RoomWidth);
	const int RoomStartXPos = Util::RandomRange(xPosRangeMin, xPosRangeMax);
	const int yPosRangeMin = StartPosition.y + wallThickness;
	const int yPosRangeMax = yPosRangeMin + (RoomMaxHeight - RoomHeight);
	const int RoomStartYPos = Util::RandomRange(yPosRangeMin, yPosRangeMax);

	roomSpace = std::make_unique<RoomSpace>(Vector2Int(RoomStartXPos, RoomStartYPos), RoomWidth, RoomHeight, StartPosition, Width, Height);
	assert(roomSpace);
	roomSpace->InitializeRoomSpace();
}

void BSPNode::GetRoomLists(std::vector<RoomSpace*>& outRoomLists)
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
			outRoomLists.emplace_back(roomSpace.get());
		}
		break;
	}
}

void BSPNode::GeneratePathBetweenRooms(RoomSpace& leftRoom, RoomSpace& rightRoom)
{
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

	switch (DivideDirection)
	{
		//수직 분할한 상태면 좌-우 연결
	case eDivideDirection::Vertical:
		{
			//왼쪽 자식 방의 외곽 타일 중 하나 선택
			const Vector2Int selectLeftOuterTile = leftRoom.SelectDoorTile(eRoomSides::Right);

			//오른쪽 자식 방의 외곽 타일 중 하나 선택
			const Vector2Int selectRightOuterTile = rightRoom.SelectDoorTile(eRoomSides::Left);

			//leftRoom을 감싸는 외곽영역의 경계라인 x 인덱스
			const int OuterBorderXPos = leftRoom.GetOuterPostionLT().x + leftRoom.GetOuterWidth() - 1;

			//y 증가량 구하기
			const int addY = getAddValue(selectRightOuterTile.y - selectLeftOuterTile.y);

			int xPos = selectLeftOuterTile.x;
			int yPos = selectLeftOuterTile.y;

			//첫 x 경로 생성.(외곽영역 경계까지)
			for (; xPos != OuterBorderXPos; ++xPos)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2Int(xPos, yPos));
			}

			//y 경로 생성
			for (; yPos != selectRightOuterTile.y; yPos += addY)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2Int(xPos, yPos));
			}

			//나머지 x 경로 생성.
			for (; xPos != selectRightOuterTile.x; ++xPos)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2Int(xPos, yPos));
			}

			//마지막 타일 뚫기
			pathTileIndices.emplace_back(Vector2Int(xPos, yPos));
		}
		break;

		//수평 분할한 상태면 상-하 연결
	case eDivideDirection::Horizontal:
		{
			//왼쪽 자식 방(위쪽에 위치)의 외곽 타일 중 하나 선택
			const Vector2Int selectLeftOuterTile = leftRoom.SelectDoorTile(eRoomSides::Bottom);

			//오른쪽 자식 방(아래쪽에 위치)의 외곽 타일 중 하나 선택
			const Vector2Int selectRightOuterTile = rightRoom.SelectDoorTile(eRoomSides::Top);

			//leftRoom을 감싸는 외곽영역의 경계라인 y 인덱스
			const int OuterBorderYPos = leftRoom.GetOuterPostionLT().y + leftRoom.GetOuterHeight() - 1;

			//x 증가량 구하기
			const int addX = getAddValue(selectRightOuterTile.x - selectLeftOuterTile.x);

			int xPos = selectLeftOuterTile.x;
			int yPos = selectLeftOuterTile.y;

			//첫 y 경로 생성.(외곽영역 경계까지)
			for (; yPos != OuterBorderYPos; ++yPos)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2Int(xPos, yPos));
			}

			//x 경로 생성
			for (; xPos != selectRightOuterTile.x; xPos += addX)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2Int(xPos, yPos));
			}

			//나머지 절반 y 경로 생성.
			for (; yPos != selectRightOuterTile.y; ++yPos)
			{
				//경로내의 타일 뚫기
				pathTileIndices.emplace_back(Vector2Int(xPos, yPos));
			}

			//마지막 타일 뚫기
			pathTileIndices.emplace_back(Vector2Int(xPos, yPos));
		}
		break;
	}
}
