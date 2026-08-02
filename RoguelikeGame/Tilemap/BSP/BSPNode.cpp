#include "BSPNode.h"
#include "Util/Util.h"
#include <cassert>

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
	//최소 길이보다 작은경우 분할하지 않고 반환
	if (Width <= 20 || Height <= 20)
	{
		NodeCategory = eNodeCategory::Room;
		GenerateRoom();
		return;
	}

	NodeCategory = eNodeCategory::Corridor;

	//분할 비율 결정
	const float DivideRatio = Util::RandomRange(4.f, 6.f);

	if (Width > Height)
	{
		//가로가 세로보다 클 때 => 수직으로 분할
		DivideDirection = eDivideDirection::Vertical;

		const int LeftWidth = static_cast<int>(Width * (DivideRatio / 10.f));
		const int RightStartPosX = StartPosition.x + LeftWidth;
		const int RightWidth = Width - LeftWidth;
		
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
		
		LeftChild = std::make_unique<BSPNode>(StartPosition.x, StartPosition.y, Width, LeftHeight);
		RightChild = std::make_unique<BSPNode>(StartPosition.x, RightStartPosY, Width, RightHeight);
	}

	//Left, Right 각각 분할 실행
	assert(LeftChild && "LeftChild Invalid..");
	LeftChild->Divide();

	assert(RightChild && "RightChild Invalid..");
	RightChild->Divide();
}

void BSPNode::ForeachNode(std::function<void(const BSPNode&)> CallbackFunc)
{
	CallbackFunc(*this);

	if (LeftChild)
	{
		LeftChild->ForeachNode(CallbackFunc);
	}

	if (RightChild)
	{
		RightChild->ForeachNode(CallbackFunc);
	}
}

void BSPNode::GenerateRoom()
{
}
