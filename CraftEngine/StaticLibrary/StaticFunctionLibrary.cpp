#include "StaticFunctionLibrary.h"
#include <Defines/Consts.h>
#include <Math/Vector2Float.h>
#include <Util/Util.h>
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

	void StaticFunctionLibrary::MidpointCircleProcess(const Vector2Int& centerPos, 
														const int xRadius, 
														const int yRadius, 
														CallbackCirclePointType callback)
	{
		/* 함수 포인터 유효 확인 */
		if (!callback)
		{
			return;
		}
		
		/* 두 반지름 모두 0보다 커야 한다.*/
		if (xRadius <= 0 || yRadius <= 0)
		{
			return;
		}

		/* 대칭되는 4분면들의 x,y 값을 가지고 callback을 호출하는 람다 */
		auto SymmetricPointsCallback = [&](int x, int y)
			{
				callback(centerPos.x + x, centerPos.y + y);

				/* 중복 방지(ex : 0, 5) */
				if (x != 0)
				{
					callback(centerPos.x + (-x), centerPos.y + y);
				}

				/* 중복 방지(ex : 5, 0) */
				if (y != 0)
				{
					callback(centerPos.x + x, centerPos.y + (-y));

					/* 중복 방지(ex : 0, 5) */
					if (x != 0)
					{
						callback(centerPos.x + (-x), centerPos.y + (-y));
					}
				}
			};

		/* 식에 사용할 상수 - 반지름 제곱을 미리 정의 */
		const int a2 = xRadius * xRadius;
		const int b2 = yRadius * yRadius;

		/* x초기값 0, y 초기값 y반지름 */
		int x = 0, y = yRadius;

		/* 판별식의 X 증가량(2 * b의 제곱 * x), 판정식의 실수 부분을 제거하기위해 4배를 했으므로 여기서도 4배 적용 */
		int deltaX4 = 0;

		/* 판별식의 Y 증가량(2 * a의 제곱 * y), 판정식의 실수 부분을 제거하기위해 4배를 했으므로 여기서도 4배 적용 */
		int deltaY4 = 8 * a2 * y;

		/* 기울기 절대값이 1보다 작은 영역(x가 1씩 증가할때 y의 감소 여부 결정) */

		/* 초기 판별식(x = 1, y = yRadius - 0.5) */
		/* b의제곱 - (a의 제곱 * yRadius) + (0.25 * a의 제곱)*/
		/* 소수점 0.25를 없애기 위해 원래 식에 4배를 곱함*/
		int fValue = (4 * b2) - (4 * (a2 * yRadius)) + a2;

		while (deltaX4 < deltaY4)
		{
			/* 현재 x, y값에 대해서 4분면에 대한 좌표 출력 */
			SymmetricPointsCallback(x, y);

			/* x는 1씩 무조건 증가 */
			++x;

			/* 판별식의 X 증가량 증가(2 * b의 제곱에 4배 적용) */
			deltaX4 += 8 * b2;

			/* 다음 판별식 값 계산 */
			if (fValue < 0)
			{
				//중점이 타원 안에 있어 x만 증가한 경우
				//원래식 dx + b2에 4배 적용
				fValue += deltaX4 + (4 * b2);
			}
			else
			{
				//중점이 타원 밖에 있어 x 증가, y가 감소한 경우
				--y;

				// 판별식의 Y 증가량 감소(2 * a의 제곱에 4배 적용)
				deltaY4 -= 8 * a2;

				// 원래식 dx - dy + b2에 4배 적용
				fValue += deltaX4 - deltaY4 + (4 * b2);
			}
		}

		/* 기울기 절대값이 1보다 큰 영역(y가 1씩 감소할때 x의 증가 여부 결정) */
		const int twoXPlueOne = 2 * x + 1;
		const int yMinusOne = y - 1;

		/* 원래 식 = b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2*/
		/* 원래 식에 4를 곱해서 실수 제거 */
		fValue = b2 * (twoXPlueOne * twoXPlueOne) + (4 * a2 * yMinusOne * yMinusOne) - (4 * a2 * b2);
		while (y >= 0)
		{
			/* 현재 x, y값에 대해서 4분면에 대한 좌표 출력 */
			SymmetricPointsCallback(x, y);

			/* y는 1씩 무조건 감소*/
			--y;

			// 판별식의 Y 증가량 감소(2 * a의 제곱에 4배 적용)
			deltaY4 -= 8 * a2;

			/* 다음 판별식 값 계산 */
			if (fValue > 0)
			{
				//중점이 타원 밖에 있어 y만 감소한 경우

				//원래식 a2 - dy에 4배 적용
				fValue += (4 * a2) - deltaY4;
			}
			else
			{
				//중점이 타원 안에 있어 y 감소, x 증가한 경우

				/* x 증가 */
				++x;

				/* 판별식의 X 증가량 증가(2 * b의 제곱에 4배 적용) */
				deltaX4 += 8 * b2;

				/* 원래식 dx - dy + a2에 4배 적용 */
				fValue += deltaX4 - deltaY4 + (4 * a2);
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

	float StaticFunctionLibrary::perlin(float x, float y)
	{
		/* 넘어온 실수형 좌표값을 기준으로 LT, RT, LB, RB의 모서리 좌표값(정수)를 구한다. */
		const int x0 = static_cast<int>(x);
		const int y0 = static_cast<int>(y);
		const int x1 = x0 + 1;
		const int y1 = y0 + 1;

		/* 보간 인자 - LT로부터 실수형 좌표값까지 얼마나 떨어져있는지 나타냄. */
		float sx = x - static_cast<float>(x0);
		float sy = y - static_cast<float>(y0);

		/* LT, RT 코너의 방향벡터와 실수형 좌표값까지의 방향에 대한 내적결과를 보간한다. */
		float n0 = dotGridGradient(x0, y0, x, y);
		float n1 = dotGridGradient(x1, y0, x, y);
		float ix0 = Interpolate(n0, n1, sx);

		/* LB, RB 코너의 방향벡터와 실수형 좌표값까지의 방향에 대한 내적결과를 보간한다. */
		n0 = dotGridGradient(x0, y1, x, y);
		n1 = dotGridGradient(x1, y1, x, y);
		float ix1 = Interpolate(n0, n1, sx);

		/* 두 내적 보간 결과를 위아래로 다시 보간한다. */
		return Interpolate(ix0, ix1, sy);
	}

	/* 사전에 계산된 정점의 방향 테이블 대신 해시 연산을 통해 각 정점의 위치에 대응되는 방향값을 반환 */
	Vector2Float StaticFunctionLibrary::randomGradient(int x, int y)
	{
		/* unsigned 총 비트수 계산(32bit) */
		static const unsigned w = 8 * sizeof(unsigned);

		/* 총 비트수의 절반 */
		static const unsigned s = w >> 1;

		/* 입력된 좌표값을 가지고 해시 연산을 진행할 변수 정의 */
		unsigned a = x, b = y;

		/* a에 특정 큰 상수를 곱하여 의사 난수처럼 보이는 비트 패턴을 만듬*/
		a *= 3284157443;

		/* a의 상위 16비트와 하위 16비트의 위치를 서로 바꾼 뒤 xor연산을 통해 b에 그 정보를 섞음 */
		b ^= a << s | a >> (w - s);

		/* b에 특정 큰 상수를 곱하여 의사 난수처럼 보이는 비트 패턴을 만듬 */
		b *= 1911520717;

		/* 다시 b의 상위 16비트와 하위 16비트의 위치를 서로 바꾼 뒤 xor연산을 통해 a에 그 정보를 섞음 */
		a ^= b << s | b >> (w - s);

		/* a에 특정 큰 상수를 곱하여 최종적으로 해시 연산 결과값 도출 */
		a *= 2048419325;

		/* 해시 연산 결과값을 비례공식을 이용하여 2 * PI(360도) 범위내의 라디안 값으로 변환한다. */
		/* ~(~0u >> 1) => 2^31(맨앞이 1이고 나머지가 모두 0) */
		/* 2 * PI / 2 ^ 32 => PI / 2 ^ 31 */
		const float random = a * static_cast<float>(3.14159265 / ~(~0u >> 1));

		/* 계산된 방향을 반환한다. */
		return Vector2Float(cos(random), sin(random));
	}

	float StaticFunctionLibrary::dotGridGradient(int ix, int iy, float x, float y)
	{
		/* 모서리의 고유 방향 */
		const Vector2Float gradient = randomGradient(ix, iy);

		/* 모서리에서 정점으로 향하는 방향 계산 */
		const Vector2Float distance = Vector2Float(x, y) - Vector2Float(static_cast<float>(ix), static_cast<float>(iy));
		
		/* 두 방향간의 내적값을 반환(방향이 같으면 양수, 방향이 다르면 음수, 수직이면 0)*/
		return distance.DotProduct(gradient);
	}

	float StaticFunctionLibrary::Interpolate(float a0, float a1, float w)
	{
		/* 격자 경계에서의 변화를 완화하기 위해 선형 보간에 사용할 보간 계수를 부드럽게 변형한다.*/
		float t = fade(w);
		return Util::Lerp(a0, a1, t);
	}

	float StaticFunctionLibrary::fade(float w)
	{
		//6t^5 - 15t^4 + 10t^3
		return (6.f * w * w * w * w * w)
			- (15.f * w * w * w * w)
			+ (10.f * w * w * w);
	}

}