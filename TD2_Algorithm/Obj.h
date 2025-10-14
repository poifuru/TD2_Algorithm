#pragma once
#include "struct.h"

class Obj {
public:
	virtual void Draw ();

protected:
	//position
	Vector2 <float> pos_;
	Vector2 <float> radius_;

	//物理演算用の変数
	Vector2 <float> velocity_;
};

