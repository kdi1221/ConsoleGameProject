#include "Util.h"

namespace Util
{
	//랜덤 엔진 반환 함수
	std::mt19937& GetRandomEngine()
	{
		//프로그램 실행 중에 1번만 생성.
		static std::mt19937 engine;
		return engine;
	}

	// 랜덤 시드(종자 값) 설정 함수
	void SetRandomSeed()
	{
		//하드웨어 기반 난수 생성기(시드값 제공용)
		std::random_device randomDevice;

		// 랜덤 엔진에 종자 값 설정
		GetRandomEngine().seed(randomDevice());
	}

	//정수 난수 함수.
	int RandomRange(int min, int max)
	{
		//min에서 max까지 균등하게 정수 난수를 생성해주는 분포 정의
		std::uniform_int_distribution<int> distribution(min, max);

		//랜덤 엔진을 통해 무작위 수 반환
		return distribution(GetRandomEngine());
	}

	//부동소수점 난수 함수.
	float RandomRange(float min, float max)
	{
		//min에서 max까지 균등하게 부동소수점 난수를 생성해주는 분포 정의
		std::uniform_real_distribution<float> distribution(min, max);

		//랜덤 엔진을 통해 무작위 수 반환
		return distribution(GetRandomEngine());
	}

	//선형 보간
	float Lerp(float a, float b, float t)
	{
		// t clamp
		t = std::min(1.f, t);
		t = std::max(0.f, t);

		return ((1.f - t) * a) + t * b;
	}
}