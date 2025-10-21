#pragma once
#include "Obj.h"

class Enemy : public Obj {
public:
	void Initialize ();

	void Update ();

	void Draw () override;

private:

};

