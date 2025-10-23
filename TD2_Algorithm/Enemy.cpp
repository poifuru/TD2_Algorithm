#include "Enemy.h"
#include "Shape.h"
#include "function.h"
#include <imgui.h>

//敵の速さ
const float kEnemySpeed = 2.0f;

void Enemy::Initialize (float x, Vector2<float> goal) {
	hp_ = 1;
	isAlive_ = true;

	pos_ = { x, -35.0f };
	radius_ = { 25.0f, 25.0f };

	//目標地点までの方向ベクトル(単位ベクトル)を取得
	diff_ = Subtract (goal, pos_);
	normal_ = Normalize (diff_);
	//velocityを目標地点に向かせる
	velocity_ = Multiply(kEnemySpeed, normal_);
}

bool Enemy::IsCollision (Vector2<float> pos, float radius) {
	Vector2<float> diff = Subtract (pos, pos_);
	float dis = Length (diff);

	return dis <= radius + radius_.x;
}

void Enemy::Update () {
	if (isAlive_) {
		//座標更新
		pos_.x += velocity_.x;
		pos_.y += velocity_.y;
	}
}

void Enemy::Draw () {
	if (isAlive_) {
		Shape::DrawEllipse (pos_.x, pos_.y, radius_.x, radius_.y, 0.0f, BLACK, kFillModeSolid);
	}
}
