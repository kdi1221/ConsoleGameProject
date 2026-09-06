#pragma once

#include "Core/Core.h"
#include <Math/Vector2Float.h>
#include <Math/Vector2Int.h>
#include <Math/Color.h>
#include <string>
#include <vector>

namespace Craft
{
	/* 파티클 입자 이동 모드 */
	enum class eParticleElementMoveMode
	{
		/* 속도 기반 이동 */
		Velocity = 0,

		/* 궤도 기반 이동 */
		Orbit,
	};

	/* 파티클 입자 개별 정보 */
	struct CRAFT_API FParticleElement
	{
		/* 입자 이동 모드 */
		eParticleElementMoveMode moveMode = eParticleElementMoveMode::Velocity;

		/* 입자 위치 */
		Vector2Int position = Vector2Int::Zero;

		/* 입자 위치 이동에 대한 실수형 델타값 */
		Vector2Float moveDeltaValue = Vector2Float::Zero;

		/* 속도 */
		Vector2Float velocity = Vector2Float::Zero;

		/* 궤도 - 회전각도 */
		float orbitAngle = 0.f;

		/* 궤도 - 반지름 */
		float orbitRadius = 0.f;

		/* 궤도 - 초당 회전속도 */
		float orbitAngularSpeed = 0.f;

		/* 궤도 - 반지름 최소값 */
		float orbitMinRadius = 0.f;

		/* 궤도 - 반지름 최대값 */
		float orbitMaxRadius = 0.f;

		/* 궤도 반지름 x,y 축별 스케일 */
		Vector2Float orbitRadiusScale = Vector2Float::One;

		/* 노이즈 위치(펄린 노이즈 공간 내 좌표) */
		Vector2Float noisePosition = Vector2Float::Zero;

		/* 노이즈 속도 */
		float noiseSpeed = 0.f;

		/* 노이즈 강도 */
		float noiseStrength = 0.f;

		/* 생명 주기 */
		float lifeTime = 0.f;

		/* 경과시간 */
		float duration = 0.f;

		/* 입자 표시 이미지(여기선 String) */
		std::wstring image;

		/*입자 표시 이미지들 */
		std::vector<std::wstring> drawImages;

		/* 이미지 변경 주기 */
		float drawChangeImageInterval = 0.f;

		/* 이미지 변경 경과 시간 */
		float drawChangeImageElapsedTime = 0.f;

		/* 입자 표시 색상 */
		Color drawColor = Color::BrightWhite;

		/* 표시 색상 리스트 */
		std::vector<Color> drawColors;

		/* 색상 변경 주기 */
		float drawChangeColorInterval = 0.f;

		/* 색상 변경 경과 시간 */
		float drawChangeColorElapsedTime = 0.f;

		/* 랜더링 순서 */
		int renderSortingOrder = 0;

	public:
		void Reset()
		{
			moveMode = eParticleElementMoveMode::Velocity;
			position = Vector2Int::Zero;
			moveDeltaValue = Vector2Float::Zero;
			velocity = Vector2Float::Zero;
			orbitAngle = 0.f;
			orbitRadius = 0.f;
			orbitAngularSpeed = 0.f;
			orbitMinRadius = 0.f;
			orbitMaxRadius = 0.f;
			orbitRadiusScale = Vector2Float::One;
			noisePosition = Vector2Float::Zero;
			noiseSpeed = 0.f;
			noiseStrength = 0.f;
			lifeTime = 0.f;
			duration = 0.f;
			image = L"";
			drawImages.clear();
			drawChangeImageInterval = 0.f;
			drawChangeImageElapsedTime = 0.f;
			drawColor = Color::BrightWhite;
			drawColors.clear();
			drawChangeColorInterval = 0.f;
			drawChangeColorElapsedTime = 0.f;
			renderSortingOrder = 0;
		}
	};
}

