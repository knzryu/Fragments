#include "GameManager.h"
#include "ResultData.h"


//ゲームの初期化
void GameManager::Initialize(const CourseManager& courseManager)
{
	player.Initialize();

	combatManager.Initialize();

	// 初期配置の敵を生成
	enemyManager.Initialize(courseManager);

	bulletManager.Initialize();

	// リザルト関連データを初期化
	survivalTime = 0.0f;
	resultData = ResultData{};
	resultInitialized = false;
}


// ゲームの更新
void GameManager::Update(
	float deltaTime,
	const CourseManager& courseManager,
	bool canMoveForward)
{
	// クールタイムを更新
	combatManager.UpdateCooldown(deltaTime);

	//生存時間を加算
	survivalTime += deltaTime;


	// ボス戦以外では前進
	if (canMoveForward)
	{
		player.MoveForward(deltaTime);
	}

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

GameState GameManager::GetGameState() const
{
	return gameState;
}

void GameManager::SetGameState(GameState state)
{
	//GameOverに初めて遷移するときだけ結果を保存
	if (state == GameState::GameOver && gameState != GameState::GameOver && !resultInitialized)
	{
		CreateResult();
	}
	gameState = state;
}

void GameManager::CreateResult()
{
	// 走行距離
	resultData.distance = player.GetCourseZ();

	// 敵の撃破数
	resultData.defeatedEnemyCount =
		playerProgress.GetDefeatedEnemyCount();

	// 生存時間
	resultData.survivalTime = survivalTime;

	// 結果保存済みにする
	resultInitialized = true;
}

// リザルトデータを取得
const ResultData& GameManager::GetResultData() const
{
	return resultData;
}

// プレイヤーの成長・進行状況を取得
PlayerProgress& GameManager::GetPlayerProgress()
{
	return playerProgress;
}

//障害物
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



// 戦闘処理を実行
bool GameManager::UpdateBulletsAndCombat(
	float deltaTime,
	const CourseManager& courseManager,
	BossManager& bossManager)
{
	// 1. すべての弾を更新
	bulletManager.UpdateAll(deltaTime, courseManager);

	// 2. 弾と敵の当たり判定・ダメージ処理
	bool isLevelUp = combatManager.Update(
		enemyManager,
		bulletManager,
		playerProgress,
		bossManager,
		player
	);

	// 3. プレイヤーから200m以上前方に進んだ弾を削除
	bulletManager.RemoveOutOfRangeBullets(player.GetCourseZ());

	return isLevelUp;
}

//gameManagerにリセット処理もまかせた
//ゲーム全体をリセット新しいランの状態に戻す
void GameManager::ResetRun(const CourseManager& courseManager)
{
	//プレイヤーを初期状態に戻す
	player.Reset();

	playerProgress.Reset();

	combatManager.Initialize();

	//障害物を全て削除
	obstacleManager.Reset();

	//敵を全て削除
	enemyManager.Initialize(courseManager);

	//弾を全て削除 
	bulletManager.Initialize();

	// リザルト関連データを初期化
	survivalTime = 0.0f;
	resultData = ResultData{};
	resultInitialized = false;
}

// 弾の発射要求
bool GameManager::TryShot(const CourseManager& courseManager)
{
	// 発射できない場合は終了
	if (!combatManager.CanShot())
	{
		return false;
	}

	// 弾を生成
	bulletManager.SpawnBullet(
		player.GetCourseZ() + 1.0f,
		player.GetLaneOffset(),
		2.0f,// ← ここが弾の高さ
		player.GetBulletSize(),
		courseManager
	);

	// クールタイム開始
	combatManager.StartCooldown(player.GetShotCooldown());

	return true;
}

// クールタイム取得
float GameManager::GetShotCooldown() const
{
	return combatManager.GetShotTimer();
}

