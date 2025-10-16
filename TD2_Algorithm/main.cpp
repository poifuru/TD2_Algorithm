#include <Novice.h>
#include "struct.h"
#include "SceneManager.h"
#include "player.h"

const char kWindowTitle[] = "";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 500, 700);

	std::unique_ptr<SceneManager> sceneManager = std::make_unique<SceneManager> ();
	sceneManager->Initialize (SceneLabel::Play);
	bool isFinish = false;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		///
		/// ↓更新処理ここから
		///

		sceneManager->Update (isFinish);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		sceneManager->Draw ();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (isFinish) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
