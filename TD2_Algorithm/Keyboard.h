#pragma once
#include "Novice.h"

class Keyboard {
public:
	void Update ();

	const char* GetKeys () { return keys_; }
	const char* GetPreKeys () { return preKeys_; }

	bool IsTrigger (int key) { return (keys_[key] != 0) && (preKeys_[key] == 0); }

	bool IsRelease (int key) { return !keys_[key] && preKeys_[key]; }

private:
	char keys_[256] = { 0 };
	char preKeys_[256] = { 0 };
};

