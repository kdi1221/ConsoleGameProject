#include "StaticFunctionLibrary.h"
#include <Defines/Consts.h>
#include <Math/Vector2Float.h>
#include <unordered_map>

namespace Craft
{
	/* 8방향에 대한 실제 벡터 값들 */
	const std::unordered_map<eDirection, Vector2Float> DIRECTION =
	{
		{eDirection::Left, Vector2Float(-1.f, 0.f)},
		{eDirection::Top, Vector2Float(0.f, -1.f)},
		{eDirection::Right, Vector2Float(1.f, 0.f)},
		{eDirection::Bottom, Vector2Float(0.f, 1.f)},

		{eDirection::LeftTop, Vector2Float(-DIAGONAL_VALUE, -DIAGONAL_VALUE)},
		{eDirection::RightTop, Vector2Float(DIAGONAL_VALUE, -DIAGONAL_VALUE)},
		{eDirection::LeftBottom, Vector2Float(-DIAGONAL_VALUE, DIAGONAL_VALUE)},
		{eDirection::RightBottom, Vector2Float(DIAGONAL_VALUE, DIAGONAL_VALUE)},
	};

	/* 8방향에 대한 실제 벡터 값들(INT) */
	const std::unordered_map<eDirection, Vector2Int> DIRECTION_INT =
	{
		{eDirection::Left, Vector2Int(-1, 0)},
		{eDirection::Top, Vector2Int(0, -1)},
		{eDirection::Right, Vector2Int(1, 0)},
		{eDirection::Bottom, Vector2Int(0, 1)},

		{eDirection::LeftTop, Vector2Int(-1, -1)},
		{eDirection::RightTop, Vector2Int(1, -1)},
		{eDirection::LeftBottom, Vector2Int(-1, 1)},
		{eDirection::RightBottom, Vector2Int(1, 1)},
	};

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

	void StaticFunctionLibrary::GetBresenhamCirclePoints(const Vector2Int& centerPos, const int radius, std::vector<Vector2Int>& outerPoints, const float xScale)
	{
		/* 결과 좌표 저장 벡터 초기화 */
		outerPoints.clear();

		auto callbackCirclePoints = [&](int xPos, int yPos)
			{
				/* 외곽 영역 포인트 저장 */
				outerPoints.emplace_back(centerPos + Vector2Int(xPos, yPos));
			};

		BresenhamCircleProcess(centerPos, radius, callbackCirclePoints, xScale);
	}

	void StaticFunctionLibrary::GetBresenhamCirclePoints(const Vector2Int& centerPos,
															const int radius,
															std::vector<Vector2Int>& outerPoints,
															std::vector<Vector2Int>& innerPoints,
															const float xScale)
	{
		/* 결과 좌표 저장 벡터 초기화 */
		outerPoints.clear();
		innerPoints.clear();

		/* y 위치별 xMin, xMax 값 계산 */
		std::unordered_map<int, std::pair<int, int>> mapCircleXRange;

		auto callbackCirclePoints = [&](int xPos, int yPos)
			{
				/* 외곽 영역 포인트 저장 */
				outerPoints.emplace_back(centerPos + Vector2Int(xPos, yPos));

				/* 원 내부 영역을 구하기위한 x범위를 y위치별로 저장 */
				auto iterYPosXRange = mapCircleXRange.find(yPos);
				if (iterYPosXRange == mapCircleXRange.end())
				{
					mapCircleXRange.insert({ yPos, {xPos, xPos}});
				}
				else
				{
					iterYPosXRange->second.first = min(iterYPosXRange->second.first, xPos);
					iterYPosXRange->second.second = max(iterYPosXRange->second.second, xPos);
				}
			};

		BresenhamCircleProcess(centerPos, radius, callbackCirclePoints, xScale);

		/* y축별로 x min ~ x max를 돌아가면서 원 내부 타일 위치를 채운다. */
		for (auto& iterCircleXRange : mapCircleXRange)
		{
			const int yPos = iterCircleXRange.first;

			for (int xPos = iterCircleXRange.second.first + 1;
				xPos < iterCircleXRange.second.second;
				++xPos)
			{
				innerPoints.emplace_back(centerPos + Vector2Int(xPos, yPos));
			}
		}
	}

	void StaticFunctionLibrary::BresenhamCircleProcess(const Vector2Int& centerPos,
														const int radius,
														CallbackCirclePointType callbackPerPoints,
														const float xScale)
	{
		/* 반지름이 0 이하이거나 콜백이 유효하지않으면 반환 */
		if (radius <= 0 || !callbackPerPoints)
		{
			return;
		}

		/* x는 0, y가 radius인 상황에서 우측으로 움직이면서 y좌표를 계산한다. */
		int x = 0;
		int y = radius;

		/* 초기 판별식(1.25 - r이나 1 - r이나 동일하다) */
		int fValue = 1 - radius;

		/* y 위치별 xMin, xMax 값 계산 */
		std::unordered_map<int, std::pair<int, int>> mapCircleXRange;

		/* y 위치별 x 최소값 및 최대값 갱신 람다 */
		auto InsertCircleInnerPoints = [&mapCircleXRange](int x, int y)
			{
				auto iterYPosXRange = mapCircleXRange.find(y);
				if (iterYPosXRange == mapCircleXRange.end())
				{
					mapCircleXRange.insert({ y, {x, x} });
				}
				else
				{
					iterYPosXRange->second.first = min(iterYPosXRange->second.first, x);
					iterYPosXRange->second.second = max(iterYPosXRange->second.second, x);
				}
			};

		/* x가 y이하인 동안 8분면에 대해 반복 */
		while (x <= y)
		{
			/* 8분면은 각각 대칭이므로 x, y값을 부호와 위치를 바꿔가며 반복해서 호출 */
			callbackPerPoints(static_cast<int>(x * xScale), y);
			callbackPerPoints(static_cast<int>(y * xScale), x);

			callbackPerPoints(static_cast<int>(-x * xScale), y);
			callbackPerPoints(static_cast<int>(y * xScale), -x);

			callbackPerPoints(static_cast<int>(x * xScale), -y);
			callbackPerPoints(static_cast<int>(-y * xScale), x);

			callbackPerPoints(static_cast<int>(-x * xScale), -y);
			callbackPerPoints(static_cast<int>(-y * xScale), -x);

			/* x값 증가 */
			++x;

			/* 판별식을 기준으로 y를 감소할지 여부를 결정한다. */
			if (fValue < 0)
			{
				/* 판단할 중점이 원의 안쪽에 있으므로 그대로 유지한다. */
				/* 기존 2x + 3에서 x는 이미 증가했으므로 (x-1)이 대입되어야 한다. */
				fValue += (2 * x) + 1;
			}
			else
			{
				/* 판단할 중점이 원의 바깥쪽에 있으므로 y를 아래로 내린다. */
				--y;

				/* 기존 2x - 2y + 5에서  x는 이미 증가했고 y는 감소했으므로 (x-1), (y+1)이 대입되어야 한다. */
				fValue += (2 * x) - (2 * y) + 1;
			}
		}
	}

	eDirection StaticFunctionLibrary::GetNearestDirection(const Vector2Float& checkDirection)
	{
		if (checkDirection.IsNearlyZero())
		{
			return eDirection::None;
		}

		eDirection resultDirection = eDirection::None;

		float maxDot = -1.f;
		for (const auto& direction : DIRECTION)
		{
			const Vector2Float& currentDirection = direction.second;
			float dotResult = currentDirection.DotProduct(checkDirection);

			if (dotResult > maxDot)
			{
				maxDot = dotResult;
				resultDirection = direction.first;
			}
		}

		return resultDirection;
	}

}