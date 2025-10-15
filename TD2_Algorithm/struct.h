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

//地面との当たり判定
struct CollisionResult {
	bool isColliding;    // 衝突したか
	Vector2<float> closest;     // 最近接点
	Vector2<float> normal;      // 法線（点 → 最近接点方向）
	float penetration;   // めり込み距離（プレイヤー半径とかで使う）
};