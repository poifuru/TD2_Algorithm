#pragma once
#include "Obj.h"

class Enemy : public Obj {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="x">ランダム値</param>
	/// <param name="goal">敵を動かす目標地点</param>
	void Initialize (float x, Vector2<float> goal);

	/// <summary>
	/// 対象と敵との当たり判定
	/// </summary>
	/// <param name="pos">対象の位置</param>
	/// <param name="radius">対象の半径</param>
	/// <returns>当たってるか</returns>
	bool IsCollision (Vector2<float> pos, float radius);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update ();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw () override;

	bool GetIsAlive () { return isAlive_; }
	void SetIsAlive () { isAlive_ = false; }
	Vector2<float> GetPosition () { return pos_; }
	Vector2<float> GetRadius () { return radius_; }
	Vector2<float> GetDiff () { return diff_; }

private:
	//敵の情報
	int hp_;
	bool isAlive_;

	//目標地点までの差分ベクトル
	Vector2<float> diff_;
	//diffを正規化した方向ベクトル
	Vector2<float> normal_;
	//目標までの距離
	float dis_;
};

