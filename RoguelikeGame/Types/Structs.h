#pragma once

struct FRect
{
	//좌측(시작 x좌표)
	int left = 0;

	//상단(시작 y좌표)
	int top = 0;

	//길이
	int width = 0;

	//높이
	int height = 0;

	FRect()
		:left(0)
		,top(0)
		,width(0)
		,height(0)
	{

	}

	FRect(int inLeft, int inTop, int inWidth, int inHeight)
		:left(inLeft)
		,top(inTop)
		,width(inWidth)
		,height(inHeight)
	{

	}
};