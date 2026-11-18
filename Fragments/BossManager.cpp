#include "BossManager.h"

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
float BossManager::SetSpawnDistance() const
{
	return spawnDistance;
}

//中ボスの出現判定
bool BossManager::CheckSpawn(float playerCouseZ)
{
	if (hasSpawned)
	{
		return false;
	}

	if (playerCouseZ >= spawnDistance)
	{
		hasSpawned = true;
		return true;
	}

	return false;
}