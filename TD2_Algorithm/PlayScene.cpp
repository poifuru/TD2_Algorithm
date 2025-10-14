#include "PlayScene.h"
#include "Shape.h"
#include "function.h"

void PlayScene::Initialize (Keyboard* keyboard) {
	keyboard_ = keyboard;
	player_ = std::make_unique<Player> ();
	player_->Initialize (keyboard_);
	ground[0] = {
		{0.0f, 600.0f},
		{640.0f, 720.0f},
	};
	ground[1] = {
		{640.0f, 720.0f},
		{1280.0f, 600.0f},
	};
}

CollisionResult PlayScene::CollisionGround (const Vector2<float>& pos, const Vector2<float>& radius, const Segment& segment) {
	CollisionResult result{};
	Vector2<float> closest = ClosestPoint (pos, segment);
	Vector2<float> diff = Subtract (pos, closest);
	float distance = Length (diff);

	if (distance < radius.x) {
		result.isColliding = true;
		result.closest = closest;

		// 法線方向は点 → 最近接点
		if (distance != 0.0f) {
			result.normal = Multiply (1.0f / distance, diff);
		}
		else {
			result.normal = { 0.0f, 1.0f }; // 0距離なら適当な上向き
		}

		result.penetration = radius.x - distance;
	}
	else {
		result.isColliding = false;
	}

	return result;
}

void PlayScene::Update () {
	player_->Update ();
	for (int i = 0; i < 2; i++) {
		collisionResult_ = CollisionGround (player_->GetPositon (), player_->GetRadius (), ground[i]);
		if (collisionResult_.isColliding) {
			//player_->Repulsion ();
			// 位置補正
			player_->SetPosition(Add (player_->GetPositon(), Multiply (collisionResult_.penetration, collisionResult_.normal)));

			// 反発
			float restitution = 0.3f; // 0〜1
			float dotV = Dot (player_->GetVelocity(), collisionResult_.normal);
			player_->SetVelocity(Subtract (player_->GetVelocity(), Multiply ((1.0f + restitution) * dotV, collisionResult_.normal)));
		}
	}
}

void PlayScene::Draw () {
	player_->Draw ();

	//地面
	Shape::DrawLine (ground[0].origin.x, ground[0].origin.y, ground[0].diff.x, ground[0].diff.y, BLACK);
	Shape::DrawLine (ground[1].origin.x, ground[1].origin.y, ground[1].diff.x, ground[1].diff.y, BLACK);
}
