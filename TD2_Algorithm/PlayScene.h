#pragma once
#include "Scene.h"
#include "Player.h"
#include "struct.h"

class PlayScene : public Scene {
public:
	void Initialize(Keyboard* keyboard) override;

	void Update() override;
	void Draw() override;

private:
	std::unique_ptr<Player> player_;

	//地面
	Segment ground[2];
	CollisionResult p_result_[2];
	CollisionResult b_result_[2];
	//反射させる
	Vector2<float> reflect_;
};