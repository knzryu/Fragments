#pragma once

#include "Boss.h"

class CourseManager;

//ボスの進行状態
enum class BossPhase
{
	Normal,    //通常ラン
	Warning,   //ボス予告
	Battle     //ボス戦フェーズ
};


enum class BossLane
{
	Left,   // 0
	Center, // 1
	Right   // 2
};


class BossManager
{
private:

	// ボス本体
	Boss boss;

	//現在のボス進行状況
	BossPhase phase = BossPhase::Normal;

	//ボス出現間隔
	//float bossInterval = 1000.0f;

	//ボス出現前の予告距離
	float warningDistance = 100.0f;

	// 次のボス出現距離
	float nextBossDistance = 1000.0f;


public:
	// ボスモデルの読み込み
	bool LoadModel();

	// ボスモデルの解放
	void ReleaseModel();

	void Reset(); 

	// ボスの更新
	void Update(float playerCourseZ,float deltaTime,const CourseManager& courseManager);

	// ボスの描画
	void Draw();

	// 弾が命中した場合、ボスにダメージを与える
	bool ApplyBulletHit(
		float bulletPreviousCourseZ,
		float bulletCourseZ,
		float bulletLaneOffset,
		float bulletSize,
		int damage);

	// レーザーがプレイヤーに命中したか判定
	bool IsPlayerHitByLaser(
		VECTOR playerMin,
		VECTOR playerMax
	) const;

	// ボスが戦闘中か確認する
	bool IsBossActive() const;

	// ボスのHPを取得する
	int GetBossHP() const;

	// 現在の進行状況を取得
	BossPhase GetPhase() const;

	// 次のボス出現距離を取得
	float GetNextBossDistance() const;

};


