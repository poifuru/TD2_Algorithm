#include "PlayScene.h"
#include "Shape.h"
#include "function.h"
#include <imgui.h>

//反発係数
const float kCOR = 0.80f;

void PlayScene::Initialize (Keyboard* keyboard) {
	keyboard_ = keyboard;
	player_ = std::make_unique<Player> ();
	player_->Initialize (keyboard_);
	ground[0] = {
		{0.0f, 650.0f},
		{250.0f, 700.0f},
	};
	ground[1] = {
		{250.0f, 700.0f},
		{500.0f, 650.0f},
	};
	for (int i = 0; i < 2; i++) {
		p_result_[i] = {};
		b_result_[i] = {};
	}
}

void PlayScene::Update () {
	player_->Input ();
	player_->SpeedCalculation ();
	player_->Update ();

	//画面のどっちにいるかの一時フラグ
	bool area = false;

	if (player_->GetPositon ().x > 250.0f) {
		area = true;
	}
	else {
		area = false;
	}

	for (int i = 0; i < 2; i++) {
		//プレイヤーと地面の当たり判定
		p_result_[i] = isCollision (player_->GetPositon (), player_->GetRadius (), ground[i]);
		if (p_result_[i].isColliding) {
			//めり込みを直す
			player_->SetPosition (player_->GetPositon () + (p_result_[i].penetration * p_result_[i].normal));
			//反射ベクトル更新
			player_->SetReflect(Reflect (player_->GetVelocity (), p_result_[i].normal));

			//反射ベクトルをxだけ反転させるように一時的に宣言
			Vector2<float> reflect = player_->GetReflect ();

			if (!area) { // 左側
				if (player_->GetVelocity ().x > 0.0f && player_->GetWallTouch ()) {
					reflect.x = std::abs (reflect.x);
					reflect.x += 4.0f;
				}
				else if (player_->GetVelocity ().x > 0.0f) {
					reflect.x = std::abs (reflect.x);
				}
				else if (player_->GetVelocity ().x < 0.0f) {
					reflect.x *= -1.0f;
					
				}
			}
			if (area) { // 右側
				if (player_->GetVelocity ().x < 0.0f && player_->GetWallTouch ()) {
					reflect.x = -std::abs (reflect.x);
					reflect.x -= 4.0f;
				}
				else if (player_->GetVelocity ().x < 0.0f) {
					reflect.x = -std::abs (reflect.x);
				}
				else if (player_->GetVelocity ().x > 0.0f) {
					reflect.x *= -1.0f;
					
				}
			}
			player_->SetWallTouch ();
			//プレイヤーの速度に掛ける
			player_->SetVelocity (reflect * kCOR);
		}
		ImGui::Text ("wallTouch %d", player_->GetWallTouch());

		//弾と地面の当たり判定
		for (auto& b : player_->GetBullet ()) {
			b_result_[i] = isCollision (b.GetPositon (), b.GetRadius (), ground[i]);
			if (b_result_[i].isColliding) {
				//めり込みを直す
				b.SetPosition (b.GetPositon () + (b_result_[i].penetration * b_result_[i].normal));
				//反射ベクトル更新
				b.SetReflect (Reflect (b.GetVelocity (), b_result_[i].normal));

				//一時的な反射ベクトル
				Vector2<float> reflect = b.GetReflect ();

				if (!b.GetArea()) { // 左側
					if (b.GetVelocity ().x > 0.0f) {
						reflect.x = std::abs (reflect.x);
					}
					if (b.GetVelocity ().x < 0.0f) {
						reflect.x *= -1.0f;
					}
				}
				if (b.GetArea ()) { // 右側
					if (b.GetVelocity ().x < 0.0f) {
						reflect.x = -std::abs (reflect.x);
					}
					if (b.GetVelocity ().x > 0.0f) {
						reflect.x *= -1.0f;
					}
				}
				//弾の速度に掛ける
				b.SetVelocity (reflect * kCOR);
			}
		}
	}
}

void PlayScene::Draw () {
	player_->Draw ();

	//地面
	Shape::DrawLine (ground[0].origin.x, ground[0].origin.y, ground[0].diff.x, ground[0].diff.y, BLACK);
	Shape::DrawLine (ground[1].origin.x, ground[1].origin.y, ground[1].diff.x, ground[1].diff.y, BLACK);
}
