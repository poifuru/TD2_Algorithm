#pragma once
#include "Scene.h"
#include "Player.h"
#include "struct.h"

class PlayScene : public Scene {
public:
	void Initialize(Keyboard* keyboard) override;
	
	void Reflection ();
	void BulletRecovery ();
	void Update() override;
	void Draw() override;

private:
	std::unique_ptr<Player> player_;

	//地面
	Segment ground[2];
	CollisionResult p_result_[2];
	CollisionResult b_result_[2];

	//弾の回収部分
	Circle circle_;
	//弾とサークルの距離
	Vector2<float> vec_[10];
	float dis_[10];
};