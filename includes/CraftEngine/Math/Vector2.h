#pragma once

#include "Core/Core.h"

namespace Craft
{
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
		~Vector2();

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
	};
}

