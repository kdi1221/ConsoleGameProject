#pragma once

#include "Core/Core.h"
#include <random>

namespace Util
{
	//랜덤 성능 지표: 무작위 수를 얼마나 잘 생성해주는가 / 속도.
	
	//랜덤 엔진 반환 함수
	CRAFT_API std::mt19937& GetRandomEngine();

	// 랜덤 시드(종자 값) 설정 함수
	CRAFT_API void SetRandomSeed();

	//정수 난수 함수.
	CRAFT_API int RandomRange(int min, int max);

	//부동소수점 난수 함수.
	CRAFT_API float RandomRange(float min, float max);
}