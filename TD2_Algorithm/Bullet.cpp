#include "Bullet.h"
#include "Shape.h"
#include "imgui.h"

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
		ImGui::Text ("recoverTime %d", recoverTime_);

		if (recoverTime_ == 0) {
			isActive_ = false;
		}
	}
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
		if (pos_.x + radius_.x + velocity_.x >= 500.0f) {
			pos_.x = pos_.x - kPos;
		}
	}
}

void Bullet::Draw () {
	if (isActive_) {
		Shape::DrawEllipse (pos_.x, pos_.y, radius_.x, radius_.y, 0.0f, RED, kFillModeSolid);
	}
}