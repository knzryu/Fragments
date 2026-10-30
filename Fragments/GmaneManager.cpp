#include "GameManager.h"

//ゲームの初期化
void GameManager::Initialize()
{
	player.Initialize();

	//初期配置の敵を生成
	SpawnInitialEnemies();
}

//ゲームの更新
void GameManager::Update(float deltaTime)
{
	//プレイヤーを自動前進させる
	player.MoveForward(deltaTime);
}

//ゲームの描画
void GameManager::Draw()
{
	player.Draw();
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

// 敵の配列への参照を返す
std::vector<Enemy>& GameManager::GetEnemies()
{
	return enemies;
}

//初期配置の敵を生成する
void GameManager::SpawnInitialEnemies()
{
	Enemy enemy1;
	enemy1.Initialize(-3.0f, 0.0f, 20.0f);
	enemies.push_back(enemy1);

	Enemy enemy2;
	enemy2.Initialize(3.0f, 0.0f, 30.0f);
	enemies.push_back(enemy2);
}

//gameManagerにリセット処理もまかせた
//ゲーム全体をリセット新しいランの状態に戻す
void GameManager::ResetRun()
{
	//プレイヤーを初期状態に戻す
	player.Reset();

	playerProgress.Reset();

	//障害物を全て削除
	obstacleManager.Reset();

	//敵をすべて削除する
	enemies.clear();

	//敵を初期配置
	SpawnInitialEnemies();
}