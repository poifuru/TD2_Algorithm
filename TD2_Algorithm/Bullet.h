#pragma once
#include "Obj.h"

class Bullet : public Obj {
public:
	void Initialize (Vector2<float> pos, float sin, float cos);

	void Update ();

	void Draw () override;

	Vector2 <float> GetPos () { return pos_; }
	bool GetIsActive () { return isActive_; }
	void SetIsActive () { isActive_ = true; }

private:
	bool isActive_;
};

