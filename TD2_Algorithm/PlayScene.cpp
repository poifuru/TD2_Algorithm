#include "PlayScene.h"
#include "Shape.h"
#include "function.h"
#include <imgui.h>

//反発係数
const float kCOR = 0.80f;
const float deltaTime = 1.0f / 60.0f;

//敵の生成間隔の最小値、最大値
const float kMinSpawnTimeFase1 = 1.0f;
const float kMaxSpawnTimeFase1 = 5.0f;

const float kMinSpawnTimeFase2 = 0.75f;
const float kMaxSpawnTimeFase2 = 3.5f;

const float kMinSpawnTimeFase3 = 0.4f;
const float kMaxSpawnTimeFase3 = 1.5f;

void PlayScene::Initialize (Keyboard* keyboard) {
	keyboard_ = keyboard;
	player_ = std::make_unique<Player> ();
	player_->Initialize (keyboard_);
	e_Manager_ = std::make_unique<EnemyManager> ();
	gameOver_ = false;

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
		{90.0f, 90.0f},
	};
	coreHp_ = 3;
	for (int i = 0; i < 10; i++) {
		vec_[i] = {};
		dis_[i] = {};
	}

	//乱数生成機
	// 実行ごとに異なるシード値を取得する
	std::random_device rd;
	// std::mt19937 エンジンのインスタンスを作成し、rd()の結果で初期化する
	engine_.seed (rd ());
	//分布の初期化
	enemy_x_ = std::uniform_real_distribution<float> (25.0f, 475.0f);
	spawnTime_[0] = std::uniform_real_distribution<float> (kMinSpawnTimeFase1, kMaxSpawnTimeFase1);
	spawnTime_[1] = std::uniform_real_distribution<float> (kMinSpawnTimeFase2, kMaxSpawnTimeFase2);
	spawnTime_[2] = std::uniform_real_distribution<float> (kMinSpawnTimeFase3, kMaxSpawnTimeFase3);

	ingameTimer_ = 0.0f;
	Timer_ = 0.0f;
	time_ = spawnTime_[0] (engine_);
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
					reflect.x += 3.0f;
				}
				else if (player_->GetVelocity ().x >= 0.0f) {
					reflect.x = std::abs (reflect.x);
				}
				else if (player_->GetVelocity ().x <= 0.0f) {
					reflect.x *= -1.0f;
				}
			}
			if (area) { // 右側
				if (player_->GetVelocity ().x < 0.0f && player_->GetWallTouch ()) {
					reflect.x = -std::abs (reflect.x);
					reflect.x -= 3.0f;
				}
				else if (player_->GetVelocity ().x <= 0.0f) {
					reflect.x = -std::abs (reflect.x);
				}
				else if (player_->GetVelocity ().x >= 0.0f) {
					reflect.x *= -1.0f;
				}
			}
			//プレイヤーの速度に掛ける
			player_->SetVelocity (reflect * kCOR);
			player_->SetWallTouch ();
		}

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
						reflect.x += 3.0f;
					}
					else if (b.GetVelocity ().x >= 0.0f) {
						reflect.x = std::abs (reflect.x);
					}
					else if (b.GetVelocity ().x <= 0.0f) {
						reflect.x *= -1.0f;
					}
				}
				if (b.GetArea ()) { // 右側
					if (b.GetVelocity ().x < 0.0f && b.GetWallTouch ()) {
						reflect.x = -std::abs (reflect.x);
						reflect.x -= 3.0f;
					}
					else if (b.GetVelocity ().x <= 0.0f) {
						reflect.x = -std::abs (reflect.x);
					}
					else if (b.GetVelocity ().x >= 0.0f) {
						reflect.x *= -1.0f;
					}
				}
				//弾の速度に掛ける
				b.SetVelocity (reflect * kCOR);
				b.SetWallTouch ();
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

		//弾の速度が0且つサークルに当たってたら
		if (b.GetIsActive () && dis_[i] <= circle_.radius.y + b.GetRadius ().y && b.GetVelocity ().x <= 0.01f && b.GetVelocity ().y <= 0.01f) {
			b.Recover ();
		}

		i++;
	}
}

void PlayScene::EnemyProcess () {
	if (ingameTimer_ >= time_) {
		//敵を生成する	
		e_Manager_->Spawn (enemy_x_ (engine_), circle_.pos);
		e_Manager_->Spawn (enemy_x_ (engine_), circle_.pos);

		//経過時間から今回スポーンにかかった時間を減算
		ingameTimer_ -= time_;

		if (Timer_ <= 1200.0f) {
			time_ = spawnTime_[0] (engine_);
		}
		else if (Timer_ <= 2400.0f) {
			time_ = spawnTime_[1] (engine_);
		}
		else if (Timer_ <= 3600) {
			time_ = spawnTime_[2] (engine_);
		}
	}

	//敵の更新処理
	for (auto& e : e_Manager_->GetEnemies ()) {
		e.Update ();

		//敵とコア
		if (e.IsCollision (circle_.pos, circle_.radius.x)) {
			if (e.GetIsAlive ())
				coreHp_--;
			e.SetIsAlive ();
		}

		//敵と弾
		for (auto& b : player_->GetBullet ())
			if (e.IsCollision (b.GetPositon (), b.GetRadius ().x)) {
				e.SetIsAlive ();

				if (player_->GetDisToCore () <= 316.6f) {
					player_->CollectBullet (1);
				}
				else if (player_->GetDisToCore () <= 583.2f) {
					player_->CollectBullet (2);
				}
				else if (player_->GetDisToCore () <= 750.0f) {
					player_->CollectBullet (3);
				}
			}

		//敵とプレイヤー
		if (e.IsCollision (player_->GetPositon (), player_->GetRadius ().x)) {
			player_->SetIsStan ();
		}
	}

	e_Manager_->EraseEnemy ();
}

void PlayScene::Update () {
	//時間のカウント
	ingameTimer_ += deltaTime;
	Timer_++;
	player_->Input ();
	player_->SpeedCalculation ();
	player_->disCalculation (circle_.pos);
	player_->Update ();
	Reflection ();
	BulletRecovery ();
	EnemyProcess ();

	if (coreHp_ == 0) {
		gameOver_ = true;
	}

	ImGui::Text ("coreHp %d", coreHp_);
	ImGui::Text ("bulletNum : %d", player_->GetBulletNum ());
	ImGui::Text ("enemyNum : %d", e_Manager_->GetEnemies ().size ());
	ImGui::Text ("playerToCore :%f", player_->GetDisToCore ());
	ImGui::Text ("Timer : %f", Timer_);
}

void PlayScene::Draw () {
	//倍率ゾーン
	Shape::DrawEllipse (circle_.pos.x, circle_.pos.y, 750.0f, 750.0f, 0.0f, 0xffff00ff, kFillModeSolid);
	Shape::DrawEllipse (circle_.pos.x, circle_.pos.y, 583.2f, 583.2f, 0.0f, 0xbbbb00ff, kFillModeSolid);
	Shape::DrawEllipse (circle_.pos.x, circle_.pos.y, 316.6f, 316.6f, 0.0f, 0x888800ff, kFillModeSolid);

	//回収部分
	Shape::DrawEllipse (circle_.pos.x, circle_.pos.y, circle_.radius.x, circle_.radius.y, 0.0f,
						BLUE, kFillModeSolid);
	player_->Draw ();
	for (auto& e : e_Manager_->GetEnemies ()) {
		e.Draw ();
	}

	//地面
	Shape::DrawLine (ground[0].origin.x, ground[0].origin.y, ground[0].diff.x, ground[0].diff.y, BLACK);
	Shape::DrawLine (ground[1].origin.x, ground[1].origin.y, ground[1].diff.x, ground[1].diff.y, BLACK);
}
