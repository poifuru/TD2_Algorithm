#include "Player.h"
#include <Novice.h>
#include "Shape.h"
#include <cmath>

const float kGravity = -9.8f;
const float kMaxFallSpeed = 20.0f;
const float deltaTime = 1.0f / 60.0f;
//めり込み防止用の定数
const float kPos = 3.0f;

void Player::Initialize (Keyboard* keyboard) {
	keyboard_ = keyboard;
	pos_ = { 100.0f, 400.0f };
	radius_ = { 30.0f, 30.0f };
	velocity_ = {-1.0f, 0.0f};

	cannonPos_ = { pos_.x, pos_.y - radius_.y };
	cannonRadius_ = { 12.0f, 24.0f };
	cannonOffset_ = { 0.0f, -30.0f };
	angle_ = 0.0f;
	rad_ = 0.0f;
	sinf_ = 0.0f;
	cosf_ = 0.0f;
	reflect_ = { 0.0f, 0.0f };
	wallTouch_ = false;
	bulletNum_ = 10;

	for (auto& b : bullet) {
		b.Initialize (pos_, sinf_, cosf_);
	}
}

void Player::Jump () {
	if (keyboard_->IsTrigger (DIK_SPACE) && bulletNum_ > 0) {
		if (pos_.x <= 250.0f) {
			velocity_.x = 4.0f;
			velocity_.y = 6.0f;
		}
		else if (pos_.x >= 250.0f) {
			velocity_.x = -4.0f;
			velocity_.y = 6.0f;
		}
	}
}

void Player::Rotate () {
	if (keyboard_->GetKeys ()[DIK_A]) {
		angle_ -= 5.0f;
	}
	if (keyboard_->GetKeys ()[DIK_D]) {
		angle_ += 5.0f;
	}

	// --- ラジアン変換 ---
	rad_ = angle_ * (3.14159265f / 180.0f);

	// --- 回転を適用（オフセットを回転） ---
	Vector2<float> rotatedOffset;
	rotatedOffset.x = cannonOffset_.x * cosf (rad_) - cannonOffset_.y * sinf (rad_);
	rotatedOffset.y = cannonOffset_.x * sinf (rad_) + cannonOffset_.y * cosf (rad_);

	// --- プレイヤー位置を中心に戻す ---
	cannonPos_.x = pos_.x + rotatedOffset.x;
	cannonPos_.y = pos_.y + rotatedOffset.y;
}

void Player::Fire () {
	if (keyboard_->IsTrigger (DIK_SPACE) && bulletNum_ > 0) {
		for (auto& b : bullet) {
			if (!b.GetIsActive ()) {
				b.Initialize (pos_, sinf (rad_), cosf (rad_));
				b.SetIsActive ();
				bulletNum_--;
				break;
			}
		}
	}
}

void Player::SpeedCalculation () {
	if (pos_.x - radius_.x <= 0.0f || pos_.x + radius_.x >= 500.0f) {
		wallTouch_ = true;
		velocity_.x *= -1.0f;
	}
	if (pos_.y - radius_.y <= 0.0f) {
		velocity_.y = 0.0f;
	}

	velocity_.y += kGravity * deltaTime;
}

void Player::Input () {
	Jump ();
	Fire ();

}

void Player::Update () {
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

	//大砲
	Rotate ();

	//弾
	for (auto& b : bullet) {
		b.Update ();
		if (b.GetIsActive() && b.GetRecoverTime () == 0) {
			bulletNum_++;
			break;
		}
	}
}

void Player::Draw () {
	//player
	Shape::DrawEllipse (pos_.x, pos_.y, radius_.x, radius_.y, 0.0f, WHITE, kFillModeSolid);
	//cannon
	// 回転行列を使って4点を回す
	Vector2<float> local[4] = {
		{-cannonRadius_.x, -cannonRadius_.y},
		{ cannonRadius_.x, -cannonRadius_.y},
		{-cannonRadius_.x,  cannonRadius_.y},
		{ cannonRadius_.x,  cannonRadius_.y},
	};

	for (int i = 0; i < 4; i++) {
		float x = local[i].x;
		float y = local[i].y;
		local[i].x = x * cosf (rad_) - y * sinf (rad_);
		local[i].y = x * sinf (rad_) + y * cosf (rad_);
		local[i].x += cannonPos_.x;
		local[i].y += cannonPos_.y;
	}

	Shape::DrawQuad (
		local[0].x, local[0].y,
		local[1].x, local[1].y,
		local[2].x, local[2].y,
		local[3].x, local[3].y,
		0, 0, 0, 0, 0, BLACK
	);

	//弾
	for (auto& b : bullet) {
		b.Draw ();
	}
}
