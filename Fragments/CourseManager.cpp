#include <cmath>  //数学関数を利用する
#include "CourseManager.h"
#include <algorithm>　//std::maxやstd::minを利用する

//コースの初期生成  初期化関数
void CourseManager::Initialize()
{
	//clearは、vectorに保存されている要素を全て削除する関数
	//なぜ必要なのか？Initialize() が複数回呼び出された場合、以前の制御点が残ってしまうのを防ぐため
	//clear() は要素を削除するが、通常はvectorの確保済み容量までは解放しない。
	controlPoints.clear();

	// 乱数を初期化
	//random_device は乱数の種（シード）を取得するために使用するもの。
	//なぜ乱数の種が必要なの？
	//例えば、乱数生成器に毎回同じシードを与えると、基本的に同じ乱数の並びが生成される。
	// 異なるシードを与えれば、異なる並びになることが期待できる。
	std::random_device rd;
	//seed() は乱数生成器の初期状態を設定する関数。
	randomEngine.seed(rd());

	//最初の制御点を設定
	//push_back最後に要素を追加
	controlPoints.push_back({ 0.0f, -60.0f });
	controlPoints.push_back({ 0.0f, -30.0f });
	controlPoints.push_back({ 0.0f,   0.0f });


	// 先のコースを生成
	for (int i = 0; i < 17; i++)
	{
		GeneratePoint();
	}
}

// ランダムな制御点を追加
void CourseManager::GeneratePoint()
{
	// 前の制御点のX座標を取得
	float previousX = controlPoints.back().x;

	// 前の位置からの変化量
	std::uniform_real_distribution<float> dist(-2.6f, 2.5f);

	float nextX = previousX + dist(randomEngine);

	// コースの最大幅を制限
	nextX = std::max(-8.0f, std::min(8.0f, nextX));

	// Z座標は30ずつ進める
	float nextZ = controlPoints.back().z + 30.0f;

	controlPoints.push_back({ nextX, nextZ });
}

// プレイヤーの前方にコースが不足していたら追加生成
void CourseManager::UpdateCourse(float playerCourseZ)
{
	// プレイヤーより300先までコースを確保する
	float generateLimit = playerCourseZ + 300.0f;

	// 最後の制御点が生成範囲に届くまで追加
	while (controlPoints.back().z < generateLimit)
	{
		GeneratePoint();
	}
}

// プレイヤーより十分後方にある制御点を削除
void CourseManager::RemoveOldPoints(float playerCourseZ)
{
	// プレイヤーより150後方を削除基準にする
	float removeLimit = playerCourseZ - 150.0f;

	// 曲線計算に必要な制御点を最低4つ残す
	while (controlPoints.size() > 4 &&
		controlPoints[1].z < removeLimit)
	{
		controlPoints.erase(controlPoints.begin());
	}
}

// 制御点を取得
const std::vector<CoursePoint>&
CourseManager::GetControlPoints() const
{
	return controlPoints;

}


// 指定したZ座標のコース中心を取得する関数
CoursePoint CourseManager::GetPosition(float z) const
{
	// 制御点の数を取得
	int count = (int)controlPoints.size();

	// Z座標に対応する区間を探す
	for (int i = 0; i < count - 3; i++)
	{
		const CoursePoint& p1 = controlPoints[i + 1];
		const CoursePoint& p2 = controlPoints[i + 2];

		if (z >= p1.z && z <= p2.z)
		{
			// 区間内の割合を計算
			float t = (z - p1.z) / (p2.z - p1.z);

			// Catmull-Rom曲線上の位置を取得
			return Spline::CatmullRom(
				controlPoints[i],
				controlPoints[i + 1],
				controlPoints[i + 2],
				controlPoints[i + 3],
				t
			);
		}
	}

	// 範囲外の場合は最後の制御点を返す
	return controlPoints.back();
}

CoursePoint CourseManager::GetDirection(float z) const
{
	const float delta = 0.1f;

	CoursePoint ahead = GetPosition(z + delta);
	CoursePoint behind = GetPosition(z - delta);

	float dx = ahead.x - behind.x;
	float dz = ahead.z - behind.z;

	// ベクトルを正規化
	float length = sqrtf(dx * dx + dz * dz);

	if (length == 0.0f)
	{
		return { 0.0f, 1.0f };
	}

	return { dx / length, dz / length };
}

CoursePoint CourseManager::GetRight(float z) const
{
	CoursePoint direction = GetDirection(z);

	return {
		direction.z,
		-direction.x
	};
}