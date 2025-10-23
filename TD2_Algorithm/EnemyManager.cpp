#include "EnemyManager.h"
#include "function.h"

void EnemyManager::Spawn (float pos, Vector2<float> goal) {
	enemy_.Initialize (pos, goal);

	enemies_.push_back (enemy_);
}

void EnemyManager::EraseEnemy () {
	//isAlive_がfalseの敵を削除する
	enemies_.erase (
		std::remove_if (enemies_.begin (), enemies_.end (),
		[](Enemy& enemy) {
			return !enemy.GetIsAlive ();
		}),
		enemies_.end ()
	);
}