#pragma once

#include "Math/Vector2.h"
#include <memory>

class BSPNode
{
public:
	BSPNode(int inStartX, int inStartY, int inWidth, int inHeight);
	virtual ~BSPNode();

public:
	void Divide();

private:
	//시작 위치
	Craft::Vector2 StartPosition = Craft::Vector2::Zero;

	//가로 길이
	int Width = 0;

	//세로 길이
	int Height = 0;

	//왼쪽 트리 노드
	std::unique_ptr<BSPNode> LeftChild;

	//오른쪽 트리 노드
	std::unique_ptr<BSPNode> RightChild;
};

