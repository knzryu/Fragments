#include "CourseManager.h"

#include <cmath>
#include <algorithm>

// コースの初期化
void CourseManager::Initialize()
{
    controlPoints.clear();

    // 乱数を初期化
    std::random_device rd;
    randomEngine.seed(rd());

    // 区間情報を初期化
    segmentStartX = 0.0f;
    targetX = 0.0f;

    segmentPointCount = 4;
    generatedPointsInSegment = 0;

    isFirstCurve = true;
    currentCurveType = CurveType::Straight;

    // 最初の制御点
    controlPoints.push_back({ 0.0f, -60.0f });
    controlPoints.push_back({ 0.0f, -30.0f });
    controlPoints.push_back({ 0.0f,   0.0f });

    // 前方のコースを生成
    for (int i = 0; i < 17; i++)
    {
        GeneratePoint();
    }
}


// 制御点を追加する
void CourseManager::GeneratePoint()
{
    // 現在の区間が終了したら次の区間を決める
    if (generatedPointsInSegment >= segmentPointCount)
    {
        SelectNextCurve();
    }

    // 区間の進行割合
    generatedPointsInSegment++;

    float t =
        static_cast<float>(generatedPointsInSegment)
        / segmentPointCount;

    // SmoothStepで滑らかに補間する
    float smoothT = t * t * (3.0f - 2.0f * t);

    // 開始位置から目標位置まで移動
    float nextX =
        segmentStartX + (targetX - segmentStartX) * smoothT;

    // X座標をコースの範囲内に制限
    nextX = std::max(MIN_X, std::min(MAX_X, nextX));

    // Z座標は30ずつ進める
    float nextZ = controlPoints.back().z + 30.0f;

    controlPoints.push_back({ nextX, nextZ });
}


// 次の区間を決定する
void CourseManager::SelectNextCurve()
{
    // 現在の制御点を区間の開始地点にする
    segmentStartX = controlPoints.back().x;
    targetX = segmentStartX;

    generatedPointsInSegment = 0;

    // 最初のカーブは必ず右方向
    if (isFirstCurve)
    {
        currentCurveType = CurveType::Right;
        isFirstCurve = false;
    }
    else
    {
        // 直線・左・右をランダムに選択
        std::uniform_int_distribution<int> curveDist(0, 4);
        int curve = curveDist(randomEngine);

        switch (curve)
        {
        case 0:
            currentCurveType = CurveType::Straight;
            break;

        case 1:
        case 2:
            currentCurveType = CurveType::Left;
            break;

        case 3:
        case 4:
            currentCurveType = CurveType::Right;
            break;
        }
    }

    // 直線区間
    if (currentCurveType == CurveType::Straight)
    {
        targetX = segmentStartX;

        std::uniform_int_distribution<int> lengthDist(10, 14);
        segmentPointCount = lengthDist(randomEngine);

        return;
    }

    // カーブの移動距離
    std::uniform_real_distribution<float> distanceDist(
        MIN_CURVE_DISTANCE,
        MAX_CURVE_DISTANCE
    );

    float distance = distanceDist(randomEngine);

    // 横方向の移動可能距離
    float leftAvailable = segmentStartX - MIN_X;
    float rightAvailable = MAX_X - segmentStartX;

    // 左右の移動方向を決める
    bool turnLeft = (currentCurveType == CurveType::Left);

    // 左に十分移動できない場合は右へ切り替える
    if (turnLeft && leftAvailable < 3.5f)
    {
        turnLeft = false;
    }

    // 右に十分移動できない場合は左へ切り替える
    if (!turnLeft && rightAvailable < 3.5f)
    {
        turnLeft = true;
    }

    // 目標X座標を決定
    if (turnLeft)
    {
        currentCurveType = CurveType::Left;

        targetX = std::max(
            MIN_X,
            segmentStartX - distance
        );
    }
    else
    {
        currentCurveType = CurveType::Right;

        targetX = std::min(
            MAX_X,
            segmentStartX + distance
        );
    }

    // カーブ区間の長さ
    std::uniform_int_distribution<int> lengthDist(12, 18);
    segmentPointCount = lengthDist(randomEngine);
}


// プレイヤーの前方にコースを追加生成する
void CourseManager::UpdateCourse(float playerCourseZ)
{
    float generateLimit = playerCourseZ + 300.0f;

    while (controlPoints.back().z < generateLimit)
    {
        GeneratePoint();
    }
}


// 古い制御点を削除する
void CourseManager::RemoveOldPoints(float playerCourseZ)
{
    float removeLimit = playerCourseZ - 150.0f;

    // スプライン計算に必要な制御点を残す
    while (
        controlPoints.size() > 4 &&
        controlPoints[1].z < removeLimit
        )
    {
        controlPoints.erase(controlPoints.begin());
    }
}


// 制御点を取得する
const std::vector<CoursePoint>&
CourseManager::GetControlPoints() const
{
    return controlPoints;
}


// 指定したZ座標のコース位置を取得する
CoursePoint CourseManager::GetPosition(float z) const
{
    int count = static_cast<int>(controlPoints.size());

    for (int i = 0; i < count - 3; i++)
    {
        const CoursePoint& p1 = controlPoints[i + 1];
        const CoursePoint& p2 = controlPoints[i + 2];

        if (z >= p1.z && z <= p2.z)
        {
            float t = (z - p1.z) / (p2.z - p1.z);

            return Spline::CatmullRom(
                controlPoints[i],
                controlPoints[i + 1],
                controlPoints[i + 2],
                controlPoints[i + 3],
                t
            );
        }
    }

    return controlPoints.back();
}


// 指定したZ座標の進行方向を取得する
CoursePoint CourseManager::GetDirection(float z) const
{
    const float delta = 0.1f;

    CoursePoint ahead = GetPosition(z + delta);
    CoursePoint behind = GetPosition(z - delta);

    float dx = ahead.x - behind.x;
    float dz = ahead.z - behind.z;

    float length = std::sqrt(dx * dx + dz * dz);

    if (length == 0.0f)
    {
        return { 0.0f, 1.0f };
    }

    return {
        dx / length,
        dz / length
    };
}


// 指定したZ座標の右方向を取得する
CoursePoint CourseManager::GetRight(float z) const
{
    CoursePoint direction = GetDirection(z);

    return {
        direction.z,
        -direction.x
    };
}