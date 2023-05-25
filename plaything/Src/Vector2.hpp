#pragma once

struct Vector2
{

public:

	float	x;
	float	y;

	Vector2(float X = 0, float Y = 0);
	~Vector2() = default;


public:
	
	Vector2		Normalize()				const;
	float		Magnitude()				const;
	float		Dot(const Vector2& rhs) const;


public:

	Vector2 operator / (float scalar);
	Vector2 operator * (float scalar);
	Vector2 operator + (const Vector2& rhs);
	Vector2 operator - (const Vector2& rhs);
	float	operator * (const Vector2& rhs);

};