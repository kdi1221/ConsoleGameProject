#include "Vector2.h"
#include "Vector2Float.h"
#include <cassert>
#include <cmath>

namespace Craft
{
	Vector2 Vector2::Zero(0, 0);
	Vector2 Vector2::One(1, 1);
	Vector2 Vector2::Right(1, 0);
	Vector2 Vector2::Up(0, -1);

	Vector2::Vector2(int x, int y)
		:x(x), y(y)
	{

	}

	Vector2 Vector2::operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 Vector2::operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 Vector2::operator*(const Vector2& other) const
	{
		return Vector2(x * other.x, y * other.y);
	}

	Vector2 Vector2::operator*(int value) const
	{
		return Vector2(x * value, y * value);
	}

	Vector2 Vector2::operator/(const Vector2& other) const
	{
		assert(other.x != 0);
		assert(other.y != 0);
		return Vector2(x / other.x, y / other.y);
	}

	Vector2& Vector2::operator=(const Vector2& other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	bool Vector2::operator==(const Vector2& other) const
	{
		return (x == other.x) && (y == other.y);
	}

	bool Vector2::operator!=(const Vector2& other) const
	{
		return (x != other.x) || (y != other.y);
	}

	Vector2::operator Vector2Float() const
	{
		Vector2Float vector2f = {};
		vector2f.x = static_cast<float>(x);
		vector2f.y = static_cast<float>(y);

		return vector2f;
	}

	Vector2::operator Vector2Float()
	{
		Vector2Float vector2f = {};
		vector2f.x = static_cast<float>(x);
		vector2f.y = static_cast<float>(y);

		return vector2f;
	}

	Vector2::operator COORD() const
	{
		COORD coord = {};
		coord.X = static_cast<short>(x);
		coord.Y = static_cast<short>(y);

		return coord;
	}

	Vector2::operator COORD()
	{
		COORD coord = {};
		coord.X = static_cast<short>(x);
		coord.Y = static_cast<short>(y);

		return coord;
	}

	int Vector2::DotProduct(const Vector2& other) const
	{
		return (x * other.x) + (y * other.y);
	}

	int Vector2::Length() const
	{
		//sqrt 생략
		return (x * x) + (y * y);
	}

	double Vector2::LengthSqrt() const
	{
		//sqrt 포함
		return sqrt(static_cast<double>(Length()));
	}

	
}
