#pragma once
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "struct.h"
#include "EnemyManager.h"
#include <random>
#include <iostream>

class PlayScene : public Scene {
public:
	void Initialize(Keyboard* keyboard) override;
	
	void Reflection ();
	void BulletRecovery ();
	void Update() override;
	void Draw() override;

private:
	std::unique_ptr<Player> player_;
	//エネミーマネージャー
	std::unique_ptr<EnemyManager> e_Manager_;

	//地面
	Segment ground[2];
	CollisionResult p_result_[2];
	CollisionResult b_result_[2];

	//コア
	Circle circle_;
	int coreHp_;
	//弾とサークルの距離
	Vector2<float> vec_[10];
	float dis_[10];

	//===乱数生成器===///
	//生成エンジンの型
	using RNG_Engine = std::mt19937;
	//乱数エンジン
	RNG_Engine engine_;
	//使う分布
	std::uniform_real_distribution<float> enemy_x_;

	//===敵のランダム生成===//
	//経過時間のカウント
	float ingameTimer_;
	//生成した乱数を入れる箱
	float time_;
	//乱数の分布(敵を生成する目標時間)
	std::uniform_real_distribution<float> spawnTime_;
};