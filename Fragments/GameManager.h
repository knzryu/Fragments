#pragma once

#include <vector>
#include "Player.h"
#include "ObstacleManager.h"
#include "Enemy.h"
#include "PlayerProgress.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "CourseManager.h"
#include "CombatManager.h"
#include "GameState.h"
#include "ResultData.h"

class GameManager
{
private:
	//タイトルステート
	GameState gameState = GameState::Title;


	//プレイヤーオブジェクト
	Player player;

	//プレイヤーの成長・進行状況管理
	PlayerProgress playerProgress;

	//障害物管理
	ObstacleManager obstacleManager;

	//敵の管理
	EnemyManager enemyManager;

	//弾管理
	BulletManager bulletManager;

	// 戦闘管理
	CombatManager combatManager;

	//リザルト画面に渡すデータ
	ResultData resultData;

	//ランの経過時間
	float survivalTime = 0.0f;

	// ゲームオーバー処理が完了したか
	bool resultInitialized = false;

public:
	//ゲームの初期化
	void Initialize(const CourseManager& courseManager);

	//ゲーム全体のリセット
	void ResetRun(const CourseManager& courseManager);

	//ゲームの更新
	void Update(float deltaTime, const CourseManager& courseManager);

	// ゲーム状態を取得
	GameState GetGameState() const;

	// ゲーム状態を変更
	void SetGameState(GameState state);

	//ゲームの描画
	void Draw();

	//プレイヤーオブジェクトへの参照を返す関数
	Player& GetPlayer();

	//プレイヤーの成長・進行状況を取得
	PlayerProgress& GetPlayerProgress();

	//障害物オブジェクトへの参照を返す
	ObstacleManager& GetObstacleManager();

	//Enemymanagerを取得
	EnemyManager& GetEnemyManager();

	//BulletManagerを取得　
	BulletManager& GetBulletManager();

	// 敵の撃破数を増やす
	void AddDefeatedEnemyCount();

	// 経験値を追加する
	void AddExperience(int amount);

	// 撃破数を取得する
	int GetDefeatedEnemyCount() const;

	// 経験値を取得する
	int GetExperience() const;

	// レベルを取得する
	int GetPlayerLevel() const;

	// 次のレベルに必要な経験値を取得する
	int GetNextLevelExp() const;

	// 戦闘処理を実行 
	// 弾の更新と戦闘処理を実行
	bool UpdateBulletsAndCombat(
		float deltaTime,
		const CourseManager& courseManager
	);

	//リザルトデータを取得
	const ResultData& GetResultData() const;

	//ゲームオーバー時の結果を保存
	void CreateResult();

};



