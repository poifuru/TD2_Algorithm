#pragma once
#include "Scene.h"
#include "Player.h"
#include "struct.h"

//地面との当たり判定
struct CollisionResult {
	bool isColliding;    // 衝突したか
	Vector2<float> closest;     // 最近接点
	Vector2<float> normal;      // 法線（点 → 最近接点方向）
	float penetration;   // めり込み距離（プレイヤー半径とかで使う）
};

class PlayScene : public Scene {
public:
	void Initialize(Keyboard* keyboard) override;

	CollisionResult CollisionGround (const Vector2<float>& pos, const Vector2<float>& radius, const Segment& segment);

	void Update() override;
	void Draw() override;

private:
	std::unique_ptr<Player> player_;

	//地面
	Segment ground[2];
	CollisionResult collisionResult_;
};