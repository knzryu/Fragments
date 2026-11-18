#pragma once

class BossManager
{
private:

	//中ボスの出現距離
	float spawnDistance = 500.0f;

	//中ボスが出現したか
	bool hasSpawned = false;

public:

	//出現距離を設定
	void SetSpawnDistance(float distance);

	//出現距離を取得
	float SetSpawnDistance() const;

	//中ボスの出現判定
	bool CheckSpawn(float playerCourseZ);

};


