#pragma once

struct SpeedComponent
{
	float Speed;

	SpeedComponent(float speed)
		:Speed(speed) { }

	~SpeedComponent() = default;
};