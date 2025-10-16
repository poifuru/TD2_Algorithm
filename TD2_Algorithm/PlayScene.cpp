#include "PlayScene.h"
#include "Shape.h"
#include "function.h"
#include <imgui.h>

//反発係数
const float kCOR = 0.95f;

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
	collisionResult_[0] = {};
	collisionResult_[1] = {};
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
		//当たり判定
		collisionResult_[i] = isCollision (player_->GetPositon (), player_->GetRadius (), ground[i]);
		if (collisionResult_[i].isColliding) {
			//めり込みを直す
			player_->SetPosition (player_->GetPositon () + (collisionResult_[i].penetration * collisionResult_[i].normal));
			//反射させる
			reflect = Reflect (player_->GetVelocity (), collisionResult_[i].normal);

			//boundWall[0] = false;
			//boundWall[1] = false;

			//if (!xRef[i] ) {
			//	//x成分だけ符号を反転させる！
			//	reflect.x = -reflect.x;
			//	xRef[i] = true;
			//}

			if (!area) { // 左側
				if (player_->GetVelocity ().x > 0.0f) {
					reflect.x = std::abs (reflect.x);
				}
				if (player_->GetVelocity ().x < 0.0f) {
					reflect.x *= -1.0f;
				}
			}
			if (area) { // 右側
				if (player_->GetVelocity ().x < 0.0f) {
					reflect.x = -std::abs (reflect.x);
				}
				if (player_->GetVelocity ().x > 0.0f) {
					reflect.x *= -1.0f;
				}
			}

			//プレイヤーの速度に掛ける
			player_->SetVelocity (reflect * kCOR);
		}

		ImGui::Text ("normal x:%f y:%f", collisionResult_[i].normal.x, collisionResult_[i].normal.y);
		ImGui::Text ("xRef %d", xRef[i]);
		ImGui::Text ("boundWall %d", boundWall[i]);
	}
	ImGui::Text ("velocity x:%f y:%f", player_->GetVelocity ().x, player_->GetVelocity ().y);
	ImGui::Text ("reflect x:%f y:%f", reflect.x, reflect.y);

	//if (boundWall[0] && collisionResult_[0].isColliding) {
	//	player_->SetVelocity ({ -player_->GetVelocity ().x, player_->GetVelocity ().y });
	//}
	//else if (boundWall[1] && collisionResult_[1].isColliding) {
	//	player_->SetVelocity ({ -player_->GetVelocity ().x, player_->GetVelocity ().y });
	//}

	//if (xRef[0] && collisionResult_[1].isColliding) {
	//	xRef[0] = false;
	//}
	//else if (xRef[1] && collisionResult_[0].isColliding) {
	//	xRef[1] = false;
	//}
	//
	///*if (xRef[1] && boundWall[1]) {
	//	boundWall[0] = false;
	//	boundWall[1] = false;
	//}*/
	//
	//if (keyboard_->IsTrigger (DIK_SPACE)) {
	//	xRef[0] = false;
	//	xRef[1] = false;
	//	/*boundWall[0] = false;
	//	boundWall[1] = false;*/
	//}
}

void PlayScene::Draw () {
	player_->Draw ();

	//地面
	Shape::DrawLine (ground[0].origin.x, ground[0].origin.y, ground[0].diff.x, ground[0].diff.y, BLACK);
	Shape::DrawLine (ground[1].origin.x, ground[1].origin.y, ground[1].diff.x, ground[1].diff.y, BLACK);
}
