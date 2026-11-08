#include "GameManager.h"

//ゲームの初期化
void GameManager::Initialize(const CourseManager& courseManager)
{
	player.Initialize();

	// 初期配置の敵を生成
	enemyManager.Initialize(courseManager);

	bulletManager.Initialize();
}


// ゲームの更新
void GameManager::Update(
	float deltaTime,
	const CourseManager& courseManager)
{
	// コース上の進行位置を更新
	player.MoveForward(deltaTime);

	// コース上の位置からワールド座標を計算
	player.UpdateCoursePosition(courseManager);
}

//ゲームの描画
void GameManager::Draw()
{
	player.Draw();
	enemyManager.DrawAll();
}

Player& GameManager::GetPlayer()
{
	return player;
}

// プレイヤーの成長・進行状況を取得
PlayerProgress& GameManager::GetPlayerProgress()
{
	return playerProgress;
}

//障害物の
ObstacleManager& GameManager::GetObstacleManager()
{
	return obstacleManager;
}

// EnemyManagerを取得
EnemyManager& GameManager::GetEnemyManager()
{
	return enemyManager;
}

// BulletManagerを取得　
BulletManager& GameManager::GetBulletManager()
{
	return bulletManager;
}


//gameManagerにリセット処理もまかせた
//ゲーム全体をリセット新しいランの状態に戻す
void GameManager::ResetRun(const CourseManager& courseManager)
{
	//プレイヤーを初期状態に戻す
	player.Reset();

	playerProgress.Reset();

	//障害物を全て削除
	obstacleManager.Reset();

	//敵を全て削除
	enemyManager.Initialize(courseManager);

	//弾を全て削除 
	bulletManager.Initialize();
}