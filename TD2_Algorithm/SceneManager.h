#pragma once
#include "Scene.h"
#include "PlayScene.h"
#include "Keyboard.h"

class SceneManager {
public:
	SceneManager ();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scene">最初に出したいシーン</param>
	void Initialize (SceneLabel scene);
	void Update (bool& isFinish);
	void Draw ();

private:
	//それぞれのシーンのポインタを持たせる
	std::unique_ptr<PlayScene> playScene_ = nullptr;

	//現在のシーン
	SceneLabel scene_;
	//処理を共通化するためのポインタ
	Scene* currentScene_ = nullptr;

	//キーボード
	Keyboard* keyboard_ = nullptr;
};

