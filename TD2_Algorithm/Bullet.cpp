#include "Bullet.h"
#include "Shape.h"
#include "imgui.h"
#include <cmath>

const float kGravity = -8.5f;
const float deltaTime = 1.0f / 60.0f;
//めり込み防止用の定数
const float kPos = 3.0f;

void Bullet::Initialize (Vector2<float> pos, float sin, float cos) {
	pos_ = pos;
	radius_ = { 15.0f, 15.0f };
	velocity_ = { sin * 10.0f, cos * 10.0f }; // 発射方向に速度を設定
	isActive_ = false;
	recoverTime_ = 60;
	t_ = 0.0f;
	result_ = 0.0f;
	isReturn_ = false;
}

void Bullet::JudgeScreen () {
	if (pos_.x > 250.0f) {
		area_ = true;
	}
	else {
		area_ = false;
	}
}

void Bullet::SpeedCalculation () {
	if (pos_.x - radius_.x <= 0.0f || pos_.x + radius_.x >= 500.0f) {
		velocity_.x *= -1.0f;
	}
	if (pos_.y - radius_.y <= 0.0f) {
		velocity_.y = 0.0f;
	}

	velocity_.y += kGravity * deltaTime;
}

void Bullet::Recover () {
	if (recoverTime_ >= 0) {
		recoverTime_--;
	}
}

void Bullet::Collect () {
	if (isActive_) {
		recoverTime_ = 0;
	}
}

float Bullet::moveT (float second) {
	float frame = second * 60;
	float moveSpeed = 1.0f / frame;

	if (t_ <= 1.0f) {
		t_ += moveSpeed;
	}

	return t_;
}

float Bullet::easeInExpo (float t) {
	return (t == 0.0f) ? 0.0f : std::pow (2.0f, 10.0f * t - 10.0f);
}

Vector2<float> Bullet::Return (Vector2<float> pos, float t) {
	Vector2<float> v;

	v.x = t * pos.x + (1 - t) * pos_.x;
	v.y = t * pos.y + (1 - t) * pos_.y;

	return v;
}

void Bullet::Update () {
	if (isActive_) {
		JudgeScreen ();
		SpeedCalculation ();

		//座標更新
		pos_.x += velocity_.x;
		pos_.y -= velocity_.y;
		//壁へのめり込み予防
		if (pos_.x - radius_.x - velocity_.x <= 0.0f) {
			pos_.x = pos_.x + kPos;
		}
		/*if (pos_.x + radius_.x + velocity_.x >= 500.0f) {
			pos_.x = pos_.x - kPos;
		}*/
	}
}

void Bullet::Draw () {
	if (isActive_ || isReturn_) {
		Shape::DrawEllipse (pos_.x, pos_.y, radius_.x, radius_.y, 0.0f, RED, kFillModeSolid);
	}
}