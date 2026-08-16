#include "Vector2Float.h"
#include <cassert>
#include <cmath>
#include "Math/Vector2Int.h"
#include "Math/Define.h"

namespace Craft
{
	constexpr float EPSILON = 0.00001f;

	Vector2Float Vector2Float::Zero(0.f, 0.f);
	Vector2Float Vector2Float::One(1.f, 1.f);
	Vector2Float Vector2Float::Right(1.f, 0.f);
	Vector2Float Vector2Float::Up(0.f, -1.f);
	Vector2Float Vector2Float::Left(-1.f, 0.f);
	Vector2Float Vector2Float::Down(0.f, 1.f);
	
	Vector2Float::Vector2Float(float x, float y)
		:x(x)
		,y(y)
	{

	}

	Vector2Float Vector2Float::operator+(const Vector2Float& other) const
	{
		return Vector2Float(x + other.x, y + other.y);
	}

	Vector2Float Vector2Float::operator-(const Vector2Float& other) const
	{
		return Vector2Float(x - other.x, y - other.y);
	}

	Vector2Float Vector2Float::operator*(const Vector2Float& other) const
	{
		return Vector2Float(x * other.x, y * other.y);
	}

	Vector2Float Vector2Float::operator*(float value) const
	{
		return Vector2Float(x * value, y * value);
	}

	Vector2Float Vector2Float::operator/(const Vector2Float& other) const
	{
		assert(abs(other.x) > EPSILON);
		assert(abs(other.y) > EPSILON);

		return Vector2Float(x / other.x, y / other.y);
	}

	Vector2Float& Vector2Float::operator+=(const Vector2Float& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Vector2Float& Vector2Float::operator=(const Vector2Float& other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	bool Vector2Float::operator==(const Vector2Float& other) const
	{
		return std::abs(x - other.x) <= EPSILON && std::abs(y - other.y) <= EPSILON;
	}

	bool Vector2Float::operator!=(const Vector2Float& other) const
	{
		return !(*this == other);
	}

	Vector2Float::operator Vector2Int() const
	{
		Vector2Int vector2I = {};
		vector2I.x = static_cast<int>(x);
		vector2I.y = static_cast<int>(y);

		return vector2I;
	}

	Vector2Float::operator Vector2Int()
	{
		Vector2Int vector2I = {};
		vector2I.x = static_cast<int>(x);
		vector2I.y = static_cast<int>(y);

		return vector2I;
	}

	float Vector2Float::DotProduct(const Vector2Float& other) const
	{
		return (x * other.x) + (y * other.y);
	}

	float Vector2Float::Length() const
	{
		return (x * x) + (y * y);
	}

	float Vector2Float::LengthSqrt() const
	{
		return sqrt(Length());
	}

	void Vector2Float::Normalize()
	{
		const float Length = LengthSqrt();

		x /= Length;
		y /= Length;
	}

	bool Vector2Float::IsNearlyZero() const
	{
		return !(Length() > 0.00001f);
	}

	Vector2Float Vector2Float::RotateVector(const Vector2Float& dir, float angleDegree)
	{
		Vector2Float direction = dir;
		direction.Normalize();

		const float angleRad = DEG_TO_RAD(angleDegree);

		const float cosTheta = std::cos(angleRad);
		const float sinTheta = std::sin(angleRad);

		//회전
		const float newX = direction.x * cosTheta - direction.y * sinTheta;
		const float newY = direction.x * sinTheta + direction.y * cosTheta;

		return Vector2Float(newX, newY);
	}

}