#pragma once
#include "Enemy.h"
#include <vector>

class EnemyManager {
public:
	void Spawn (float pos, Vector2<float> goal);

	//死んだ敵をベクターから消す処理
	void EraseEnemy ();

	//void SetIsAlive () {
	//	// isAlive_がfalseの敵を削除する
	//	enemies_.erase(
	//		std::remove_if(enemies_.begin(), enemies_.end(),
	//			[](Enemy& enemy) {
	//				enemy.SetIsAlive();
	//				return !enemy.GetIsAlive();
	//			}),
	//		enemies_.end()
	//	);
	//}

	std::vector<Enemy>& GetEnemies () { return enemies_; }

private:
	//敵をvectorで持つ
	Enemy enemy_;
	std::vector<Enemy> enemies_;
};

