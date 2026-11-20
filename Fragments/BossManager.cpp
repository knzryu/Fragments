#include "BossManager.h"
#include "CourseManager.h"

// 中ボスモデルの読み込み
bool BossManager::LoadModel()
{
	return boss.LoadModel();
}

// 中ボスモデルの解放
void BossManager::ReleaseModel()
{
	boss.ReleaseModel();
}


// 中ボスの更新
void BossManager::Update(
	float playerCourseZ,
	float deltaTime,
	const CourseManager& courseManager
)
{
	if (!hasSpawned)
	{
		if (CheckSpawn(playerCourseZ))
		{
			// プレイヤーの30m先にあるコース中心座標を取得
			CoursePoint spawnPosition =
				courseManager.GetPosition(playerCourseZ + 30.0f);

			// 取得したワールド座標にボスを出現させる
			boss.Spawn(
				spawnPosition.x,
				0.0f,
				spawnPosition.z
			);
		}
	}

	if (boss.IsActive())
	{
		boss.Update(deltaTime);
	}
}


//中ボスの出現距離を設定
void BossManager::SetSpawnDistance(float distance)
{
	if (distance < 0.0f)
	{
		return;
	}

	spawnDistance = distance;
}

//中ボスの出現距離を取得
float BossManager::GetSpawnDistance() const
{
	return spawnDistance;
}

//中ボスの出現判定
bool BossManager::CheckSpawn(float playerCourseZ)
{
	if (hasSpawned)
	{
		return false;
	}

	if (playerCourseZ >= spawnDistance)
	{
		hasSpawned = true;
		return true;
	}

	return false;
}

// 中ボスの描画
void BossManager::Draw()
{
	if (boss.IsActive())
	{
		boss.Draw();
	}
}

bool BossManager::IsBossActive() const
{
	return boss.IsActive() && !boss.IsDead();
}