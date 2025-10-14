#include "Keyboard.h"

void Keyboard::Update () {
	// キー入力を受け取る
	memcpy (preKeys_, keys_, 256);              // まず前フレームの状態を保存
	Novice::GetHitKeyStateAll (keys_);         // それから今の状態を取得
}