#pragma once
#include <Novice.h>
template <typename t>

struct Vector2 {
	t x;
	t y;
};

struct Segment {
	Vector2<float> origin; //始点
	Vector2<float> diff;	//終点への差分ベクトル
};