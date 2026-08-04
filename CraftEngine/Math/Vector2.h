#pragma once

#include "Core/Core.h"
#include <Windows.h>

namespace Craft
{
	class Vector2Float;

	class CRAFT_API Vector2
	{
	public:
		//x좌표
		int x = 0;

		//y좌표
		int y = 0;

		// 자주 사용할 만한 값을 전역 변수로 선언.
		static Vector2 Zero;
		static Vector2 One;
		static Vector2 Right;
		static Vector2 Up;

	public:
		Vector2(int x = 0, int y = 0);
		~Vector2() = default;

	public:
		//사칙 연산자 오버로딩.
		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		Vector2 operator*(const Vector2& other) const;
		Vector2 operator*(int value) const;
		Vector2 operator/(const Vector2& other) const;

		// 대입 연산자 오버로딩
		Vector2& operator=(const Vector2& other);

		// 비교 연산자 오버로딩
		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;

		explicit operator Vector2Float() const;
		explicit operator Vector2Float();

		//Windows 콘솔 좌표계로 변환하는 연산자 오버로딩.
		operator COORD() const;
		operator COORD();

	public:
		//내적
		int DotProduct(const Vector2& other) const;

		//길이(sqrt 생략)
		int Length() const;

		//길이(sqrt 포함)
		double LengthSqrt() const;
	};
}

