#include <vector>
#include "Player.h"
#include "ObstacleManager.h"
#include "Enemy.h"
#include "PlayerProgress.h"
#include "EnemyManager.h"
#include "BulletManager.h"

class GameManager
{
private:

	//プレイヤーオブジェクト
	Player player;

	//プレイヤーの成長・進行状況管理
	PlayerProgress playerProgress;

	//障害物管理オブジェクト
	ObstacleManager obstacleManager;

	//敵オブジェクトの管理
	EnemyManager enemyManager;

	//弾管理オブジェクト　
	BulletManager bulletManager;

public:
	//ゲームの初期化
	void Initialize();

	//ゲーム全体のリセット
	void ResetRun();

	//ゲームの更新
	void Update(float deltaTime);

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

};



