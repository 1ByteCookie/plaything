#include "Vector2.hpp"

#include <math.h>

Vector2::Vector2(float X, float Y)
	: x(X)
	, y(Y)
{

}


float Vector2::Magnitude() const
{
	return sqrt( pow(x, 2) + pow(y, 2) );
}


Vector2 Vector2::Normalize() const
{
	Vector2 This = *this;

	return This / Magnitude();
}


float Vector2::Dot(const Vector2& rhs) const
{
	return (x * rhs.x) + (y * rhs.y);
}


Vector2 Vector2::operator / (float scalar)
{
	if (scalar)	// non-zero check
	{
		return Vector2(x / scalar, y / scalar);
	}

	return *this;
}


Vector2 Vector2::operator * (float scalar)
{
	return Vector2(x * scalar, y * scalar);
}


float Vector2::operator * (const Vector2& rhs)
{
	return Dot(rhs);
}


Vector2 Vector2::operator + (const Vector2& rhs)
{
	return Vector2(x + rhs.x, y + rhs.y);
}


Vector2 Vector2::operator - (const Vector2& rhs)
{
	return Vector2(x - rhs.x, y - rhs.y);
}
