#pragma once
#include "Obj.h"

class Bullet : public Obj {
public:
	void Initialize (Vector2<float> pos, float sin, float cos);

	void JudgeScreen ();
	void SpeedCalculation ();
	void Recover ();
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
	bool GetIsActive () { return isActive_; }
	void SetIsActive (bool flag) { isActive_ = flag; }
	bool GetArea () { return area_; }
	bool GetWallTouch () { return wallTouch_; }
	void SetWallTouch () { wallTouch_ = false; }
	int GetRecoverTime () { return recoverTime_; }
	//回収完了を知らせる関数
	bool IsRecovered () { return recoverTime_ == 0 && isActive_ == true; }

private:
	bool isActive_;
	bool area_;
	Vector2<float> reflect_;
	//壁に触ったか
	bool wallTouch_;
	//回収タイマー
	int recoverTime_;
};

