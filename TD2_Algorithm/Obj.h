#pragma once
#include "struct.h"

class Obj {
public:
	virtual void Initialize ();
	virtual void Update ();
	virtual void Draw ();

protected:
	Vector2 <float> pos_;
	Vector2 <float> radius_;

	//物理演算用の変数
	Vector2 <float> velocity_;
};

