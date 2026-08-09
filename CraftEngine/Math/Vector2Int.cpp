#include "Vector2Int.h"
#include "Vector2Float.h"
#include <cassert>
#include <cmath>

namespace Craft
{
	Vector2Int Vector2Int::Zero(0, 0);
	Vector2Int Vector2Int::One(1, 1);
	Vector2Int Vector2Int::Right(1, 0);
	Vector2Int Vector2Int::Up(0, -1);
	Vector2Int Vector2Int::Left(-1, 0);
	Vector2Int Vector2Int::Down(0, 1);

	Vector2Int::Vector2Int(int x, int y)
		:x(x), y(y)
	{

	}

	Vector2Int Vector2Int::operator+(const Vector2Int& other) const
	{
		return Vector2Int(x + other.x, y + other.y);
	}

	Vector2Int Vector2Int::operator-(const Vector2Int& other) const
	{
		return Vector2Int(x - other.x, y - other.y);
	}

	Vector2Int Vector2Int::operator*(const Vector2Int& other) const
	{
		return Vector2Int(x * other.x, y * other.y);
	}

	Vector2Int Vector2Int::operator*(int value) const
	{
		return Vector2Int(x * value, y * value);
	}

	Vector2Int Vector2Int::operator/(const Vector2Int& other) const
	{
		assert(other.x != 0);
		assert(other.y != 0);
		return Vector2Int(x / other.x, y / other.y);
	}

	Vector2Int& Vector2Int::operator+=(const Vector2Int& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Vector2Int& Vector2Int::operator=(const Vector2Int& other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	bool Vector2Int::operator==(const Vector2Int& other) const
	{
		return (x == other.x) && (y == other.y);
	}

	bool Vector2Int::operator!=(const Vector2Int& other) const
	{
		return (x != other.x) || (y != other.y);
	}

	Vector2Int::operator Vector2Float() const
	{
		Vector2Float vector2f = {};
		vector2f.x = static_cast<float>(x);
		vector2f.y = static_cast<float>(y);

		return vector2f;
	}

	Vector2Int::operator Vector2Float()
	{
		Vector2Float vector2f = {};
		vector2f.x = static_cast<float>(x);
		vector2f.y = static_cast<float>(y);

		return vector2f;
	}

	Vector2Int::operator COORD() const
	{
		COORD coord = {};
		coord.X = static_cast<short>(x);
		coord.Y = static_cast<short>(y);

		return coord;
	}

	Vector2Int::operator COORD()
	{
		COORD coord = {};
		coord.X = static_cast<short>(x);
		coord.Y = static_cast<short>(y);

		return coord;
	}

	int Vector2Int::DotProduct(const Vector2Int& other) const
	{
		return (x * other.x) + (y * other.y);
	}

	int Vector2Int::Length() const
	{
		//sqrt 생략
		return (x * x) + (y * y);
	}

	double Vector2Int::LengthSqrt() const
	{
		//sqrt 포함
		return sqrt(static_cast<double>(Length()));
	}

	
}
