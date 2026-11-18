#pragma once

#include "Boss.h"
class CourseManager;

class BossManager
{
private:

	// 中ボス本体
	Boss boss;

	//中ボスの出現距離
	float spawnDistance = 50.0f;

	//中ボスが出現したか
	bool hasSpawned = false;

	

public:
	// 中ボスモデルの読み込み
	bool LoadModel();

	// 中ボスモデルの解放
	void ReleaseModel();

	// 中ボスの更新
	void Update(float playerCourseZ,float deltaTime,const CourseManager& courseManager);

	// 中ボスの描画
	void Draw();

	// ボスが戦闘中か確認する
	bool IsBossActive() const;

	//出現距離を設定
	void SetSpawnDistance(float distance);

	//出現距離を取得
	float GetSpawnDistance() const;

	//中ボスの出現判定
	bool CheckSpawn(float playerCourseZ);

};


