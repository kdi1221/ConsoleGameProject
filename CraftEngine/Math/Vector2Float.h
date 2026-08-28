#pragma once

#include "Core/Core.h"

namespace Craft
{
	constexpr float EPSILON = 0.00001f;

	class Vector2Int;

	class CRAFT_API Vector2Float
	{
	public:
		// 자주 사용할 만한 값을 전역 변수로 선언.
		static Vector2Float Zero;
		static Vector2Float One;
		static Vector2Float Right;
		static Vector2Float Up;
		static Vector2Float Left;
		static Vector2Float Down;

	public:
		//x좌표
		float x = 0.f;

		//y좌표
		float y = 0.f;

	public:
		Vector2Float(float x = 0.f, float y = 0.f);
		~Vector2Float() = default;

	public:
		//사칙 연산자 오버로딩.
		Vector2Float operator+(const Vector2Float& other) const;
		Vector2Float operator-(const Vector2Float& other) const;
		Vector2Float operator*(const Vector2Float& other) const;
		Vector2Float operator*(float value) const;
		Vector2Float operator/(const Vector2Float& other) const;

		//복합 대입 연산자.
		Vector2Float& operator+=(const Vector2Float& other);

		// 대입 연산자 오버로딩
		Vector2Float& operator=(const Vector2Float& other);

		// 비교 연산자 오버로딩
		bool operator==(const Vector2Float& other) const;
		bool operator!=(const Vector2Float& other) const;

		// Float => Int Casting
		explicit operator Vector2Int() const;
		explicit operator Vector2Int();

	public:
		//내적
		float DotProduct(const Vector2Float& other) const;

		//길이(sqrt 생략)
		float Length() const;

		//길이(sqrt 포함)
		float LengthSqrt() const;

		//정규화
		void Normalize();

		//벡터 크기가 0에 가까운지 확인
		bool IsNearlyZero() const;

	public:
		static Vector2Float RotateVector(const Vector2Float& dir, float angleDegree);
	};
}


