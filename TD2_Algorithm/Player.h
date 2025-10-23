#pragma once
#include "Obj.h"
#include "Bullet.h"
#include <array>
#include "Keyboard.h"

class Player :	public Obj {
public:
	void Initialize (Keyboard* keyboard);

	//固有の処理
	void Jump ();
	void Rotate ();
	void Fire ();
	void SpeedCalculation ();
	void Input ();
	bool Stan ();
	void Update ();
	void Draw () override;

	//プレイヤー
	Vector2<float> GetPositon () { return pos_; }
	void SetPosition (Vector2<float> pos) { pos_ = pos; }
	Vector2<float> GetRadius () { return radius_; }
	Vector2<float> GetVelocity () { return velocity_; }
	void SetVelocity (Vector2<float> velocity) { velocity_ = velocity; }
	Vector2<float> GetReflect () { return reflect_; }
	void SetReflect (Vector2<float> velocity) { reflect_ = velocity; }
	bool GetWallTouch () { return wallTouch_; }
	void SetWallTouch () { wallTouch_ = false; }
	int GetBulletNum () { return bulletNum_; }
	void SetBulletNum () { bulletNum_++; }
	//弾
	std::array<Bullet, 10>& GetBullet () { return bullet; }

private:
	//砲台の変数
	Vector2 <float> cannonPos_;
	Vector2 <float> cannonRadius_;
	//回転用
	Vector2<float> cannonOffset_;
	float angle_;
	float rad_;
	float sinf_;
	float cosf_;
	Vector2<float> newPos_;
	//反射ベクトル
	Vector2<float> reflect_;
	//壁に触ったか
	bool wallTouch_;
	//残弾数
	int bulletNum_;
	//スタン
	bool isStan_;
	int stanTime_;

	//弾
	std::array<Bullet, 10> bullet;

	//キーボード
	Keyboard* keyboard_ = nullptr;
};

