#pragma once

#include "Core/Core.h"
#include <Windows.h>

namespace Craft
{
	class Vector2Float;

	class CRAFT_API Vector2Int
	{
	public:
		//x좌표
		int x = 0;

		//y좌표
		int y = 0;

		// 자주 사용할 만한 값을 전역 변수로 선언.
		static Vector2Int Zero;
		static Vector2Int One;
		static Vector2Int Right;
		static Vector2Int Up;
		static Vector2Int Left;
		static Vector2Int Down;

	public:
		Vector2Int(int x = 0, int y = 0);
		~Vector2Int() = default;

	public:
		//사칙 연산자 오버로딩.
		Vector2Int operator+(const Vector2Int& other) const;
		Vector2Int operator-(const Vector2Int& other) const;
		Vector2Int operator*(const Vector2Int& other) const;
		Vector2Int operator*(int value) const;
		Vector2Int operator/(const Vector2Int& other) const;

		//복합 대입 연산자.
		Vector2Int& operator+=(const Vector2Int& other);

		// 대입 연산자 오버로딩
		Vector2Int& operator=(const Vector2Int& other);

		// 비교 연산자 오버로딩
		bool operator==(const Vector2Int& other) const;
		bool operator!=(const Vector2Int& other) const;

		explicit operator Vector2Float() const;
		explicit operator Vector2Float();

		//Windows 콘솔 좌표계로 변환하는 연산자 오버로딩.
		operator COORD() const;
		operator COORD();

	public:
		//내적
		int DotProduct(const Vector2Int& other) const;

		//길이(sqrt 생략)
		int Length() const;

		//길이(sqrt 포함)
		double LengthSqrt() const;


	};
}

