#pragma once
#include <Novice.h>
template <typename t>

struct Vector2 {
	t x;
	t y;
};

struct Circle {
	Vector2 <float> pos;
	Vector2 <float> radius;
	float angle;
	float color[4];
	FillMode fillMode;
};