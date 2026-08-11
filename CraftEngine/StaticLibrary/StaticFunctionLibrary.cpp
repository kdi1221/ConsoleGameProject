#include "StaticFunctionLibrary.h"

using namespace Craft;

void StaticFunctionLibrary::GetBresenhamPath(const Craft::Vector2Int& startPos, 
											const Craft::Vector2Int& endPos, 
											std::vector<Craft::Vector2Int>& outPaths)
{
	outPaths.clear();

	const Vector2Int distance = endPos - startPos;
	if (Vector2Int::Zero == distance)
	{
		/* 시작과 끝이 같으면 시작 경로만 담고 반환한다. */
		outPaths.emplace_back(startPos);
		return;
	}

	/* X, Y 두 축의 길이(절대값) */
	const int width = abs(distance.x);
	const int height = abs(distance.y);

	/* X, Y 두 축에서 증가될 값 */
	const int addX = distance.x == 0 ? 0 : distance.x > 0 ? 1 : -1;
	const int addY = distance.y == 0 ? 0 : distance.y > 0 ? 1 : -1;

	/* x, y 두 축 중 큰 축을 중심 축으로 삼는다. */
	if (height <= width) //X 축 기반
	{
		/* 판별식 1(좌표가 선보다 위에 있을 때) */
		const int dF1 = height << 1;

		/* 판별식 2(좌표가 선보다 아래에 있을 때) */
		const int dF2 = (height - width) << 1;

		/* 판별식 초기 값 */
		int fValue = (height << 1) - width;

		/* x를 1씩 증가하면서 y값을 결정한다. */
		int xPos = startPos.x;
		int yPos = startPos.y;
		for (; xPos != endPos.x; xPos += addX)
		{
			//경로 삽입
			outPaths.emplace_back(Vector2Int(xPos, yPos));

			//다음 픽셀의 y위치 및 판별식 결정
			if (fValue < 0)
			{
				//판별식이 0보다 작은 경우 기준점이 선 위에 있는것으로 기준점 아래 픽셀, 즉 y위치 그대로 유지한다.
				fValue += dF1;
			}
			else
			{
				//판별식이 0보다 큰 경우 기준점이 선 아래에 있는것으로 기준점 위 픽셀, 즉 y위치를 하나 증가한다.
				fValue += dF2;
				yPos += addY;
			}
		}

		/* 마지막 위치를 경로에 삽입한다. */
		outPaths.emplace_back(Vector2Int(xPos, yPos));
	}
	else //Y 축 기반
	{
		/* 판별식 1(좌표가 선보다 위에 있을 때) */
		const int dF1 = width << 1;

		/* 판별식 2(좌표가 선보다 아래에 있을 때) */
		const int dF2 = (width - height) << 1;

		/* 판별식 초기 값 */
		int fValue = (width << 1) - height;

		/* y를 1씩 증가하면서 x값을 결정한다. */
		int xPos = startPos.x;
		int yPos = startPos.y;
		for (; yPos != endPos.y; yPos += addY)
		{
			//경로 삽입
			outPaths.emplace_back(Vector2Int(xPos, yPos));

			//다음 픽셀의 y위치 및 판별식 결정
			if (fValue < 0)
			{
				//판별식이 0보다 작은 경우 기준점이 선 위에 있는것으로 기준점 아래 픽셀, 즉 x위치 그대로 유지한다.
				fValue += dF1;
			}
			else
			{
				//판별식이 0보다 큰 경우 기준점이 선 아래에 있는것으로 기준점 위 픽셀, 즉 x위치를 하나 증가한다.
				fValue += dF2;
				xPos += addX;
			}
		}

		/* 마지막 위치를 경로에 삽입한다. */
		outPaths.emplace_back(Vector2Int(xPos, yPos));
	}
}
