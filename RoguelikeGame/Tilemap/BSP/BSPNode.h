#pragma once

#include "Math/Vector2.h"
#include <memory>
#include <functional>

enum class eTileCategory;

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
	void ForeachNode(std::function<void(const BSPNode&)> CallbackFunc);

private:
	void GenerateRoom();

public:
	inline const Craft::Vector2& GetStartPosition() const { return StartPosition; }
	inline int GetWidth() const { return Width; }
	inline int GetHeight() const { return Height; }
	inline eNodeCategory GetNodeCategory() const { return NodeCategory; }

private:
	//시작 위치
	Craft::Vector2 StartPosition = Craft::Vector2::Zero;

	//가로 길이
	int Width = 0;

	//세로 길이
	int Height = 0;

	//노드 종류(통로 / 방)
	eNodeCategory NodeCategory = eNodeCategory::None;

	//분할 방향
	eDivideDirection DivideDirection = eDivideDirection::None;

	//왼쪽 트리 노드
	std::unique_ptr<BSPNode> LeftChild;

	//오른쪽 트리 노드
	std::unique_ptr<BSPNode> RightChild;
};

