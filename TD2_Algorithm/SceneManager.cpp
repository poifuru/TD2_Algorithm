#include "SceneManager.h"

SceneManager::SceneManager () {
	keyboard_ = new Keyboard;
	scene_ = SceneLabel::Play;
	playScene_ = std::make_unique<PlayScene> ();
}

void SceneManager::Initialize (SceneLabel scene) {
	switch (scene) {
	case SceneLabel::Play:
		currentScene_ = playScene_.get();
		currentScene_->Initialize (keyboard_);
		break;
	}
}

void SceneManager::Update (bool& isFinish) {
	keyboard_->Update ();

	//現在シーンのUpdate
	currentScene_->Update ();


	if (keyboard_->IsTrigger (DIK_ESCAPE)) {
		isFinish = true;
	}
}

void SceneManager::Draw () {
	currentScene_->Draw ();
}