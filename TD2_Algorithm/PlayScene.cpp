#include "PlayScene.h"
#include "Shape.h"
#include "function.h"

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
}

void PlayScene::Update () {
	player_->Input ();
	
	for (int i = 0; i < 2; i++) {
		if (isCollision (player_->GetPositon (), player_->GetRadius (), ground[i])) {
			//めり込みを直す

			player_->SetPosition ({ 640.0f, 100.0f });
		}
	}
	player_->SpeedCalculation ();
	player_->Update ();
}

void PlayScene::Draw () {
	player_->Draw ();

	//地面
	Shape::DrawLine (ground[0].origin.x, ground[0].origin.y, ground[0].diff.x, ground[0].diff.y, BLACK);
	Shape::DrawLine (ground[1].origin.x, ground[1].origin.y, ground[1].diff.x, ground[1].diff.y, BLACK);
}
