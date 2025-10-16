#include "PlayScene.h"
#include "Shape.h"
#include "function.h"
#include <imgui.h>

//反発係数
const float kCOR = 1.0f;

void PlayScene::Initialize (Keyboard* keyboard) {
	keyboard_ = keyboard;
	player_ = std::make_unique<Player> ();
	player_->Initialize (keyboard_);
	ground[0] = {
		{0.0f, 500.0f},
		{640.0f, 720.0f},
	};
	ground[1] = {
		{640.0f, 720.0f},
		{1280.0f, 500.0f},
	};
	collisionResult_ = {};
}

void PlayScene::Update () {
	player_->Input ();
	player_->SpeedCalculation ();
	player_->Update ();
	for (int i = 0; i < 2; i++) {
		//当たり判定
		collisionResult_ = isCollision (player_->GetPositon (), player_->GetRadius (), ground[i]);
		if (collisionResult_.isColliding) {
			//めり込みを直す
			player_->SetPosition (player_->GetPositon () + (collisionResult_.penetration * collisionResult_.normal));
			//反射させる
			Vector2<float> reflect = Reflect (player_->GetVelocity (), collisionResult_.normal);

			//プレイヤーの速度に掛ける
			player_->SetVelocity (reflect);
		}
	}
	ImGui::Text ("normal x:%f y:%f", collisionResult_.normal.x, collisionResult_.normal.y);
}

void PlayScene::Draw () {
	player_->Draw ();

	//地面
	Shape::DrawLine (ground[0].origin.x, ground[0].origin.y, ground[0].diff.x, ground[0].diff.y, BLACK);
	Shape::DrawLine (ground[1].origin.x, ground[1].origin.y, ground[1].diff.x, ground[1].diff.y, BLACK);
}
