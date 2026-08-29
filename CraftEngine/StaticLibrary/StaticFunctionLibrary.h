#pragma once

#include <Core/Core.h>
#include <Defines/Enums.h>
#include <Math/Vector2Int.h>
#include <vector>
#include <unordered_map>
#include <functional>

namespace Craft
{
	class Vector2Float;

	/* 각 방향에 대한 실제 벡터 값들 */
	extern const CRAFT_API std::unordered_map<eDirection, Vector2Float> DIRECTION;
	extern const CRAFT_API std::unordered_map<eDirection, Vector2Int> DIRECTION_INT;
	
	class CRAFT_API StaticFunctionLibrary
	{
	public:
		/* 브레젠험 원 알고리즘에서 각 좌표별로 호출될 콜백 타입 */
		using CallbackCirclePointType = std::function<void(int, int)>;

	public:
		/* 브레젠험 선 경로 반환 */
		static void GetBresenhamPath(const Vector2Int& startPos,
									const Vector2Int& endPos,
									std::vector<Vector2Int>& outPaths);

		/* 브레젠험 원 좌표 반환 (외곽 영역만) */
		static void GetBresenhamCirclePoints(const Vector2Int& centerPos, 
											const int radius, 
											std::vector<Vector2Int>& outerPoints,
											const float xScale = 1.5f);

		/* 브레젠험 원 좌표들 반환(외곽, 내부 영역 구분해서 반환) */
		static void GetBresenhamCirclePoints(const Vector2Int& centerPos, 
											const int radius, 
											std::vector<Vector2Int>& outerPoints,
											std::vector<Vector2Int>& innerPoints,
											const float xScale = 1.5f);

		/* 브레젠험 원 그리기(외곽 좌표별 처리를 위한 함수 포인터) */
		static void BresenhamCircleProcess(const Vector2Int& centerPos,
											const int radius,
											CallbackCirclePointType callback,
											const float xScale = 1.5f);

		static eDirection GetNearestDirection(const Vector2Float& checkDirection);

	private:
		StaticFunctionLibrary() = delete;
	};
}

