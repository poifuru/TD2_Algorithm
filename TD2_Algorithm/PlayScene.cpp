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
	circle_ = {
		{250.0f, 700.0f},
		{30.0f, 30.0f},
	};
	for (int i = 0; i < 10; i++) {
		vec_[i] = {};
		dis_[i] = {};
	}
}

void PlayScene::Reflection () {
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
			player_->SetReflect (Reflect (player_->GetVelocity (), p_result_[i].normal));

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
		ImGui::Text ("wallTouch %d", player_->GetWallTouch ());

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

				if (!b.GetArea ()) { // 左側
					if (b.GetVelocity ().x > 0.0f && b.GetWallTouch ()) {
						reflect.x = std::abs (reflect.x);
						reflect.x += 4.0f;
					}
					if (b.GetVelocity ().x > 0.0f) {
						reflect.x = std::abs (reflect.x);
					}
					if (b.GetVelocity ().x < 0.0f) {
						reflect.x *= -1.0f;
					}
				}
				if (b.GetArea ()) { // 右側
					if (b.GetVelocity ().x < 0.0f && b.GetWallTouch ()) {
						reflect.x = -std::abs (reflect.x);
						reflect.x -= 4.0f;
					}
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

void PlayScene::BulletRecovery () {
	int i = 0;
	for (auto& b : player_->GetBullet ()) {
		//サークルから弾の差分ベクトルを出して距離にする
		vec_[i] = { circle_.pos - b.GetPositon () };
		dis_[i] = { Length (vec_[i]) };
		ImGui::Text ("dis[%d] : %f", i, dis_[i]);

		//弾の速度が0且つサークルに当たってたら
        if (dis_[i] <= circle_.radius.y && b.GetVelocity ().x <= 0.02f && b.GetVelocity().y <= 0.02f) {
			b.Recover ();
		}
		i++;
	}
}

void PlayScene::Update () {
	player_->Input ();
	player_->SpeedCalculation ();
	player_->Update ();
	BulletRecovery ();

	Reflection ();
	ImGui::Text ("bulletNum : %d", player_->GetBulletNum ());
}

void PlayScene::Draw () {
	//回収部分
	Shape::DrawEllipse (circle_.pos.x, circle_.pos.y, circle_.radius.x, circle_.radius.y, 0.0f,
						BLUE, kFillModeSolid);
	player_->Draw ();

	//地面
	Shape::DrawLine (ground[0].origin.x, ground[0].origin.y, ground[0].diff.x, ground[0].diff.y, BLACK);
	Shape::DrawLine (ground[1].origin.x, ground[1].origin.y, ground[1].diff.x, ground[1].diff.y, BLACK);
}
